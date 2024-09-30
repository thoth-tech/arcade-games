/**
 * @file opencl_wrapper.cpp
 * @brief Implements the OpenCLWrapper class for GPU-accelerated computations.
 *
 * This file contains the implementation of the OpenCLWrapper class, which
 * handles initialization of OpenCL, kernel execution, and memory management
 * for GPU-accelerated lighting and collision detection.
 */

#include "./include/types.h"
#include <fstream>
#include <iostream>

OpenCLWrapper::OpenCLWrapper() : gridWidth(0), gridHeight(0) {}

OpenCLWrapper::~OpenCLWrapper() {}

/**
 * @brief Initializes the OpenCL environment and compiles the kernels.
 */
void OpenCLWrapper::initialize() {
    try {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        cl::Platform platform = platforms[0];

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        cl::Device device = devices[0];

        std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        std::cout << "Device vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
        std::cout << "Device version: " << device.getInfo<CL_DEVICE_VERSION>() << std::endl;

        context = cl::Context(device);
        queue = cl::CommandQueue(context, device);

        std::string kernelSource = readKernelSource("lighting_kernels.cl");
        program = cl::Program(context, kernelSource);
        program.build({device});

        torchKernel = cl::Kernel(program, "calculate_torch_lighting");
        radialKernel = cl::Kernel(program, "calculate_radial_lighting");
        updateHeightsKernel = cl::Kernel(program, "update_heights");
        raycastKernel = cl::Kernel(program, "raycast");

        collisionBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, MAX_COLLISIONS * sizeof(cl_int2));

    } catch (cl::Error& e) {
        std::cerr << "OpenCL error: " << e.what() << " (" << e.err() << ")" << std::endl;
        exit(1);
    }
}

/**
 * @brief Initializes the grid data on the GPU.
 * @param initialGrid The initial grid state.
 */
void OpenCLWrapper::initializeGrid(const Grid& initialGrid) {
    gridWidth = initialGrid.width;
    gridHeight = initialGrid.height;
    createBuffers(gridWidth, gridHeight);

    std::vector<cl_int> gridHeights(gridWidth * gridHeight);
    for (int i = 0; i < gridWidth * gridHeight; ++i) {
        gridHeights[i] = static_cast<cl_int>(initialGrid.cells[i].height);
    }

    queue.enqueueWriteBuffer(gridHeightsBuffer, CL_TRUE, 0, gridWidth * gridHeight * sizeof(cl_int), gridHeights.data());
}

/**
 * @brief Creates OpenCL buffers for grid data.
 * @param width The width of the grid.
 * @param height The height of the grid.
 */
void OpenCLWrapper::createBuffers(int width, int height) {
    size_t gridSize = width * height;
    gridHeightsBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, gridSize * sizeof(int));
    lightLevelsBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, gridSize * sizeof(int));
    torchBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(Torch));
    radialLightsBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, MAX_RADIAL_LIGHTS * sizeof(RadialLight));
}

/**
 * @brief Adds a collision point to be processed.
 * @param x The x-coordinate of the collision point.
 * @param y The y-coordinate of the collision point.
 */
void OpenCLWrapper::addCollisionPoint(int x, int y) {
    collisionPoints.push_back({static_cast<cl_int>(x), static_cast<cl_int>(y)});
}

/**
 * @brief Updates the grid heights based on collision points.
 */
void OpenCLWrapper::updateGridHeights() {
    if (!collisionPoints.empty()) {
        queue.enqueueWriteBuffer(collisionBuffer, CL_TRUE, 0,
                                 collisionPoints.size() * sizeof(cl_int2), collisionPoints.data());

        updateHeightsKernel.setArg(0, gridHeightsBuffer);
        updateHeightsKernel.setArg(1, collisionBuffer);
        updateHeightsKernel.setArg(2, static_cast<int>(collisionPoints.size()));
        updateHeightsKernel.setArg(3, gridWidth);

        queue.enqueueNDRangeKernel(updateHeightsKernel, cl::NullRange,
                                   cl::NDRange(collisionPoints.size()));

        collisionPoints.clear();
    }
}

/**
 * @brief Calculates lighting for the entire grid.
 * @param lights The radial lights in the scene.
 * @param torch The player's torch.
 * @param torch_on Whether the torch is turned on.
 */
void OpenCLWrapper::calculateLighting(const std::vector<RadialLight>& lights, const Torch& torch, bool torch_on) {
    try {
        updateGridHeights();

        std::vector<cl_int> lightLevels(gridWidth * gridHeight, 0);
        queue.enqueueWriteBuffer(lightLevelsBuffer, CL_TRUE, 0, gridWidth * gridHeight * sizeof(cl_int), lightLevels.data());
        queue.enqueueWriteBuffer(radialLightsBuffer, CL_TRUE, 0, lights.size() * sizeof(RadialLight), lights.data());

        radialKernel.setArg(0, lightLevelsBuffer);
        radialKernel.setArg(1, gridHeightsBuffer);
        radialKernel.setArg(2, radialLightsBuffer);
        radialKernel.setArg(3, static_cast<cl_int>(lights.size()));
        radialKernel.setArg(4, static_cast<cl_int>(gridWidth));
        radialKernel.setArg(5, static_cast<cl_int>(gridHeight));

        queue.enqueueNDRangeKernel(radialKernel, cl::NullRange, cl::NDRange(gridWidth, gridHeight));

        if (torch_on) {
            queue.enqueueWriteBuffer(torchBuffer, CL_TRUE, 0, sizeof(Torch), &torch);

            torchKernel.setArg(0, lightLevelsBuffer);
            torchKernel.setArg(1, gridHeightsBuffer);
            torchKernel.setArg(2, torchBuffer);
            torchKernel.setArg(3, static_cast<cl_int>(gridWidth));
            torchKernel.setArg(4, static_cast<cl_int>(gridHeight));

            queue.enqueueNDRangeKernel(torchKernel, cl::NullRange, cl::NDRange(gridWidth, gridHeight));
        }

    } catch (cl::Error& e) {
        std::cerr << "OpenCL error in calculateLighting: " << e.what() << " (" << e.err() << ")" << std::endl;
        std::cerr << "Error occurred during: " << getOpenCLErrorDescription(e.err()) << std::endl;
    }
}

/**
 * @brief Reads the current grid heights from the GPU.
 * @param heights Vector to store the read heights.
 */
void OpenCLWrapper::readGridHeights(std::vector<int>& heights) const {
    heights.resize(gridWidth * gridHeight);
    queue.enqueueReadBuffer(gridHeightsBuffer, CL_TRUE, 0, gridWidth * gridHeight * sizeof(int), heights.data());
}

/**
 * @brief Reads the current light levels from the GPU.
 * @param levels Vector to store the read light levels.
 */
void OpenCLWrapper::readLightLevels(std::vector<int>& levels) const {
    levels.resize(gridWidth * gridHeight);
    queue.enqueueReadBuffer(lightLevelsBuffer, CL_TRUE, 0, gridWidth * gridHeight * sizeof(int), levels.data());
}

/**
 * @brief Performs a raycast to find a collision point.
 * @param start The start point of the ray.
 * @param end The end point of the ray.
 * @param hitPoint The resulting collision point (if any).
 */
void OpenCLWrapper::getCollisionPoint(const Vector2D& start, const Vector2D& end, Vector2D& hitPoint) const {
    cl_float2 clStart = {{static_cast<cl_float>(start.x), static_cast<cl_float>(start.y)}};
    cl_float2 clEnd = {{static_cast<cl_float>(end.x), static_cast<cl_float>(end.y)}};
    cl_float2 clHitPoint;

    cl::Buffer startBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2), &clStart);
    cl::Buffer endBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2), &clEnd);
    cl::Buffer hitPointBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float2));

    cl::Kernel localRaycastKernel(raycastKernel);

    localRaycastKernel.setArg(0, gridHeightsBuffer);
    localRaycastKernel.setArg(1, startBuffer);
    localRaycastKernel.setArg(2, endBuffer);
    localRaycastKernel.setArg(3, hitPointBuffer);
    localRaycastKernel.setArg(4, static_cast<cl_int>(gridWidth));
    localRaycastKernel.setArg(5, static_cast<cl_int>(gridHeight));

    cl::CommandQueue localQueue(queue);

    localQueue.enqueueNDRangeKernel(localRaycastKernel, cl::NullRange, cl::NDRange(1));
    localQueue.enqueueReadBuffer(hitPointBuffer, CL_TRUE, 0, sizeof(cl_float2), &clHitPoint);

    hitPoint.x = clHitPoint.s[0];
    hitPoint.y = clHitPoint.s[1];
}

/**
 * @brief Reads the OpenCL kernel source from a file.
 * @param filename The name of the file containing the kernel source.
 * @return The kernel source as a string.
 */
std::string OpenCLWrapper::readKernelSource(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open kernel file: " + filename);
    }
    return std::string(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
}

/**
 * @brief Provides a description for OpenCL error codes.
 * @param error The OpenCL error code.
 * @return A string describing the error.
 */
std::string OpenCLWrapper::getOpenCLErrorDescription(cl_int error) {
    switch (error) {
        case CL_INVALID_KERNEL_ARGS: return "Invalid kernel arguments";
        case CL_INVALID_WORK_DIMENSION: return "Invalid work dimension";
        case CL_INVALID_WORK_GROUP_SIZE: return "Invalid work group size";
        case CL_INVALID_WORK_ITEM_SIZE: return "Invalid work item size";
        case CL_INVALID_GLOBAL_OFFSET: return "Invalid global offset";
        case CL_OUT_OF_RESOURCES: return "Out of resources";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE: return "Memory object allocation failure";
        case CL_INVALID_EVENT_WAIT_LIST: return "Invalid event wait list";
        case CL_OUT_OF_HOST_MEMORY: return "Out of host memory";
        default: return "Unknown error";
    }
}

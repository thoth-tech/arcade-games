# GPU-Managed Dynamic Lighting + Grid State

![](out.gif)

## Overview

This demo leverages OpenCL to manage the game’s grid directly on the GPU. The goal (apart from the obvious lighting performance gains) is to avoid the high bandwidth overhead of transferring data between the CPU and GPU every frame by keeping the grid in GPU memory. This should theoretically reduce I/O bottlenecks and improve performance.

## Reality

In practice, this approach isn't as efficient as it sounds. The main issue lies in the fact that numerous small OpenCL kernels are launched every frame to handle tasks like collision detection, which then need to inform the CPU about game state. This frequent communication adds overhead and, (unsurprisingly), results in worse performance compared to simply sending grid data to the GPU frame-by-frame.

## Optimizations and Future Direction

A more efficient solution would be to offload all state management, including collision detection, to the GPU. By sending only input data to the GPU and keeping all game logic there, the need for constant back-and-forth communication with the CPU would be eliminated.

To fully implement this, the demo requires OpenGL context sharing with SDL (as the **major** bottleneck is still the single threaded SDL draw calls). This would allow OpenGL to handle the rendering directly on the GPU using a shared framebuffer, drastically improving performance. However, this implementation is only partially complete at this stage.

## Conclusion

While the demo demonstrates GPU-based grid management, the current implementation is limited by frequent CPU-GPU interactions. Optimizing this by moving more logic to the GPU and using OpenGL for rendering through a shared framebuffer is the key to unlocking the performance gains of this approach. I got a fair way through extending Splashkit's BE API to expose this functionality but had a few personal setbacks this semester and so ultimately have not finished it.



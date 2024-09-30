#define LIGHT_LEVELS 5
#define PLAYER_HEIGHT 10
#define TORCH_HEIGHT 30
#define M_PI 3.14159265358979323846f

typedef struct {
    double x, y;
} Vector2D;

typedef struct {
    Vector2D position;
    double intensity;
    double radius;
    Vector2D velocity;
    int height;
} RadialLight;

typedef struct {
    Vector2D position;
    Vector2D direction;
    double base_radius;
    double current_radius;
} Torch;

bool has_clear_path(__global const int* grid_heights, int x1, int y1, int z1, int x2, int y2, int z2, int grid_width, int grid_height) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int dz = z2 - z1;
    int x = x1;
    int y = y1;
    float z = (float)z1 + 0.1f;  // Start slightly above the terrain
    int n = 1 + dx + dy;
    int x_inc = (x2 > x1) ? 1 : -1;
    int y_inc = (y2 > y1) ? 1 : -1;
    float z_inc = (float)dz / n;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;

    for (int i = 0; i < n; ++i) {
        if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
            int cell_height = grid_heights[y * grid_width + x];
            if (z < cell_height) {
                return false;
            }
        }

        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
        z += z_inc;
    }

    return true;
}

__kernel void update_heights(__global int* grid_heights,
                             __global const int2* collision_points,
                             const int num_collisions,
                             const int grid_width) {
    int gid = get_global_id(0);
    if (gid < num_collisions) {
        int2 collision = collision_points[gid];
        int index = collision.y * grid_width + collision.x;
        grid_heights[index] = 1;
    }
}

__kernel void calculate_radial_lighting(
    __global int* light_levels,
    __global const int* grid_heights,
    __global const RadialLight* lights,
    int num_lights,
    int grid_width,
    int grid_height
) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x >= grid_width || y >= grid_height) return;

    int index = y * grid_width + x;
    int cell_height = grid_heights[index];
    int max_light_level = 0;

    for (int i = 0; i < num_lights; ++i) {
        float dx = (float)(x - lights[i].position.x);
        float dy = (float)(y - lights[i].position.y);
        float distance_squared = dx*dx + dy*dy;
        float radius = (float)lights[i].radius;

        if (distance_squared > radius * radius) {
            continue;
        }

        if (has_clear_path(grid_heights, x, y, cell_height,
                           (int)lights[i].position.x, (int)lights[i].position.y, lights[i].height,
                           grid_width, grid_height)) {
            int light_level = (int)lights[i].intensity;
            max_light_level = max(max_light_level, light_level);
        }
    }

    light_levels[index] = max_light_level;
}

__kernel void calculate_torch_lighting(
    __global int* light_levels,
    __global const int* grid_heights,
    __constant Torch* torch,
    int grid_width,
    int grid_height
) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x >= grid_width || y >= grid_height) return;

    int index = y * grid_width + x;
    int cell_height = grid_heights[index];

    float dx = (float)(x - torch->position.x);
    float dy = (float)(y - torch->position.y);
    float distance_squared = dx*dx + dy*dy;
    float current_radius = (float)torch->current_radius;
    float max_torch_radius = current_radius * 2.0f;

    if (distance_squared <= max_torch_radius * max_torch_radius) {
        float rotated_dx = dx * (float)torch->direction.x + dy * (float)torch->direction.y;
        float rotated_dy = -dx * (float)torch->direction.y + dy * (float)torch->direction.x;

        float ellipse_distance = current_radius * 1.2f;
        float ellipse_width = current_radius * 1.2f;
        float ellipse_height = current_radius * 0.8f;
        float ellipse_factor = (float)(pow(rotated_dx - ellipse_distance, 2) / pow(ellipse_width / 2, 2)
                               + pow(rotated_dy, 2) / pow(ellipse_height / 2, 2));

        int torch_light_level = 0;
        bool is_lit = false;

        if (ellipse_factor <= 1.1f) {
            torch_light_level = LIGHT_LEVELS;
            is_lit = true;
        } else if (sqrt(distance_squared) <= current_radius && rotated_dx >= 0) {
            float angle = atan2(fabs(rotated_dy), rotated_dx);
            float max_angle = atan2(ellipse_height / 2, ellipse_distance) + 0.05f;
            if (angle <= max_angle) {
                torch_light_level = cell_height <= PLAYER_HEIGHT ? LIGHT_LEVELS / 2 : LIGHT_LEVELS;
                is_lit = true;
            }
        }

        if (is_lit && has_clear_path(grid_heights, x, y, cell_height,
                                     (int)torch->position.x, (int)torch->position.y, TORCH_HEIGHT,
                                     grid_width, grid_height)) {
            atomic_max(&light_levels[index], torch_light_level);
        }
    }
}

__kernel void raycast(__global const int* grid_heights,
                      __constant float2* start,
                      __constant float2* end,
                      __global float2* hit_point,
                      const int grid_width,
                      const int grid_height) {
    float2 s = *start;
    float2 e = *end;

    float dx = fabs(e.x - s.x);
    float dy = fabs(e.y - s.y);
    int x = (int)s.x;
    int y = (int)s.y;
    int n = 1 + (int)(dx + dy);
    int x_inc = (e.x > s.x) ? 1 : -1;
    int y_inc = (e.y > s.y) ? 1 : -1;
    float error = dx - dy;
    dx *= 2;
    dy *= 2;

    for (; n > 0; --n) {
        if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
            int cell_height = grid_heights[y * grid_width + x];
            if (cell_height > 1) { // Assuming HeightLevel::FLOOR == 1
                *hit_point = (float2)(x, y);
                return;
            }
        }

        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
    }

    // No collision found
    *hit_point = (float2)(-1, -1);
}

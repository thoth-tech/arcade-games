#include "./include/types.h"
#include <cmath>


void create_particles(std::vector<Particle>& particles, const Vector2D& hit_point, const Vector2D& normal, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> vel_dist(0.5, 2.5);
    std::uniform_int_distribution<> lifetime_dist(10, 25);
    std::uniform_real_distribution<> angle_dist(-PI/25, PI/25);  // +/- 30 degrees

    // Calculate the base angle for the particles (now in the same direction as the normal)
    double base_angle = std::atan2(normal.y, normal.x);

    for (int i = 0; i < count; ++i) {
        Particle p;
        p.position = hit_point;

        // Apply random angle deviation
        double angle = base_angle + angle_dist(gen);

        double velocity_magnitude = vel_dist(gen);
        p.velocity.x = std::cos(angle) * velocity_magnitude;
        p.velocity.y = std::sin(angle) * velocity_magnitude;

        p.lifetime = lifetime_dist(gen);
        p.particle_color = rgba_color(255, 255, 255, 255);
        p.velocity_decay = 0.7;  // 5% velocity decrease per frame
        particles.push_back(p);
    }
}

void update_particles(std::vector<Particle>& particles) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->position.x += it->velocity.x;
        it->position.y += it->velocity.y;
        it->lifetime--;

        // Apply velocity decay
        it->velocity.x *= it->velocity_decay;
        it->velocity.y *= it->velocity_decay;

        if (it->lifetime <= 0) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
}

void render_particles(const std::vector<Particle>& particles) {
    for (const auto& p : particles) {
        double alpha = static_cast<double>(p.lifetime) / 40.0;
        color particle_color = rgba_color(
                red_of(p.particle_color),
                green_of(p.particle_color),
                blue_of(p.particle_color),
                static_cast<int>(alpha * 255)
        );
        fill_circle(particle_color, p.position.x * CELL_SIZE, p.position.y * CELL_SIZE, 1);
    }
}

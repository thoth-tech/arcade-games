#include "state_management.h"
#include "globals.h"


void particle_update(Particle& p) {
    p.vel.y += 0.2;
    p.pos.x += p.vel.x;
    p.pos.y += p.vel.y;
    p.ttl--;

    // Increase opacity based on the remaining ttl
    float alpha = 1.0f - (static_cast<float>(p.ttl) / static_cast<float>(p.max_ttl));
    p.clr.a = static_cast<uint8_t>(alpha * 255);
    // Decrease size based on the remaining ttl
    p.size = static_cast<int>(p.original_size * (1.0f - (alpha/2.0f)));
}

void update_particles(GameState& g) {
    for (auto& p : g.particles) {
        particle_update(p);
    }

    // Remove dead particles
    g.particles.erase(std::remove_if(g.particles.begin(), g.particles.end(),
                                     [](const Particle& p) { return p.ttl <= 0 || p.pos.y > WINDOW_HEIGHT; }),
                      g.particles.end());
}

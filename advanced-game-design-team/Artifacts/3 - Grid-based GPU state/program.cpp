/**
 * @file program.cpp
 * @brief Main program file for the lighting demo game.
 *
 * This file contains the main game loop and initialization functions.
 * It sets up the game environment, handles user input, and manages
 * the overall game state.
 */

#include "include/types.h"
#include "splashkit.h"
#include <deque>


std::vector<RadialLight> create_radial_lights(int num_lights, int grid_width, int grid_height) {
    std::vector<RadialLight> lights;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dis(0, grid_width - 1);
    std::uniform_int_distribution<> y_dis(0, grid_height - 1);
    std::uniform_int_distribution<> intensity_dis(1, LIGHT_LEVELS);
    std::uniform_real_distribution<> radius_dis(10.0, 30.0);
    std::uniform_int_distribution<> height_dis(static_cast<int>(HeightLevel::CEILING), static_cast<int>(HeightLevel::RADIAL));

    for (int i = 0; i < num_lights; ++i) {
        RadialLight light{
                {static_cast<double>(x_dis(gen)), static_cast<double>(y_dis(gen))},
                static_cast<double>(intensity_dis(gen)),
                radius_dis(gen),
                {1, 0.5},
                height_dis(gen)
        };
        lights.push_back(light);
    }

    return lights;
}

void render_frame(const OpenCLWrapper& openclWrapper, const Player& player, const std::vector<Particle>& particles, bool torch_on) {
    clear_screen(COLOR_BLACK);
    render_grid(openclWrapper);
    render_player(player);
    render_particles(particles);
    draw_crosshair();

    draw_text("Health: " + std::to_string(player.health), COLOR_WHITE, 10, 10);
    draw_text("Torch: " + std::string(torch_on ? "ON" : "OFF"), COLOR_WHITE, 10, 30);
}

int main() {
    try {
        open_window("Lighting Demo", SCREEN_WIDTH, SCREEN_HEIGHT);
        hide_mouse();
        load_sound_effect("gunshot", "gun_shot_1.wav");
        load_sound_effect("hit", "bullet_hit_1.wav");

        OpenCLWrapper openclWrapper;
        openclWrapper.initialize();

        Grid initialGrid = create_grid(GRID_WIDTH, GRID_HEIGHT);
        openclWrapper.initializeGrid(initialGrid);

        Player player = {{GRID_WIDTH / 2.0, GRID_HEIGHT / 2.0}, {0, 0}, 0, 100};
        std::vector<RadialLight> radial_lights = create_radial_lights(MAX_RADIAL_LIGHTS, GRID_WIDTH, GRID_HEIGHT);
        Torch torch = {{player.position.x, player.position.y}, {1, 0}, TORCH_RADIUS, TORCH_RADIUS};

        std::vector<Bullet> bullets;
        std::vector<Particle> particles;

        auto start_time = std::chrono::high_resolution_clock::now();
        auto last_frame_time = start_time;

        const int BENCHMARK_FRAMES = 60;
        std::deque<double> frame_times;

        bool torch_on = true;

        while (!quit_requested() && player.health > 0) {
            auto frame_start = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> delta_duration = frame_start - last_frame_time;
            double delta_time = delta_duration.count();
            last_frame_time = frame_start;

            double total_time = std::chrono::duration<double>(frame_start - start_time).count();

            process_events();

            update_player(player, openclWrapper);
            update_torch(torch, player, total_time);
            update_bullets(bullets, particles, openclWrapper);
            update_particles(particles);
            update_radial_light_movers(radial_lights, openclWrapper.getGridWidth(), openclWrapper.getGridHeight(), delta_time);

            if (mouse_down(LEFT_BUTTON) && player.cooldown == 0) {
                create_bullet(bullets, player);
            }

            if (key_typed(T_KEY)) {
                torch_on = !torch_on;
            }

            update_grid_lighting(radial_lights, torch, torch_on, openclWrapper);

            render_frame(openclWrapper, player, particles, torch_on);
//            render_bullets(bullets);

            auto frame_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> frame_duration = frame_end - frame_start;

            frame_times.push_back(frame_duration.count());
            if (frame_times.size() > BENCHMARK_FRAMES) {
                frame_times.pop_front();
            }

            double average_frame_time = std::accumulate(frame_times.begin(), frame_times.end(), 0.0) / frame_times.size();
            double fps = 1000.0 / average_frame_time;

            draw_text("Avg Frame Time: " + std::to_string(average_frame_time) + " ms | FPS: " + std::to_string(fps), COLOR_WHITE, 10, SCREEN_HEIGHT - 30);

            refresh_screen(100);
        }

    } catch (const std::exception& e) {
        write_line("An exception occurred: " + string(e.what()));
    } catch (...) {
        write_line("An unknown exception occurred.");
    }

    return 0;
}

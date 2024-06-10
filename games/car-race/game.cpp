#include "splashkit.h"
#include "game.h"
#include <cmath>
#include <vector>
// for the time functions, cars to spawn
#include <chrono>
#include <thread>
using namespace std;
using std::vector;

int MIN_SPEED = 2.0;

int MAX_SPEED = 6.0;

int CARS_COUNT = 3;

int BARRICADE_COUNT = 0;

int POWERUP_COUNT = 3;

int CARS_SPAWN_DURATION = 3; // Time between each car collection

int BARRICADE_SPAWN_DURATION = 5; // Time between each barricade collection

int POWERUP_SPAWN_DURATION = 5; // Time between each powerup collection

auto last_call_time = std::chrono::steady_clock::now(); // Track the last time the function was called
// Function to set up the game map
void map_setup(game_data &game)
{
    int laneSpacing = 100;
    int leftEdgePosition = screen_width() / 2 - laneSpacing * 7 / 2;

    // Create road edge bitmap
    bitmap roadEdge = create_bitmap("roadEdge", 10, screen_height());
    draw_line_on_bitmap(roadEdge, COLOR_WHITE, 0, 0, 0, screen_height(), option_line_width(10));
    setup_collision_mask(roadEdge);

    // Set up road markings
    point_2d abc = point_at(leftEdgePosition, 0);
    game.game_over = false;
    abc = point_at(leftEdgePosition + (laneSpacing * 7) - 5, 0);
    bitmap road = create_bitmap("roadmarkings", 10, screen_height());
    for (int i = 0; i < screen_height(); i += 20)
    {
        draw_line_on_bitmap(road, COLOR_WHITE, 1, i, 1, i + 10, option_line_width(8));
    }

    // Create road cells bitmap
    bitmap roadCells = create_bitmap("roadCells", 50, screen_height());
    for (int i = 0; i < 7; i++)
    {
        draw_bitmap_on_bitmap(roadCells, road, i * 10, i * 4);
    }

    bitmap_set_cell_details(roadCells, 10, screen_height(), 5, 1, 5);
    setup_collision_mask(roadCells);
    animation_script roadLineAnimation = load_animation_script("roadAnimation", "roadAnimation.txt");
    for (int i = 0; i < 6; i++)
    {
        game.whitemarks[i] = create_sprite(roadCells, roadLineAnimation);
        sprite_start_animation(game.whitemarks[i], "moving");
        abc = point_at(leftEdgePosition + (laneSpacing * (i + 1)), 0);
        sprite_set_position(game.whitemarks[i], abc);
    }
}

// Function to generate a random number within a given range
double random_range(double min, double max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}

game_data new_game()
{
    game_data game;
    game.car = new_car(RED, 0, screen_width() / 2, screen_height() - 200);
    map_setup(game);
    game.level = 1;
    game.shield = false;
    game.speed_boost = false;
    game.paused = false;
    game.score = 0;

    game.twoPlayer = false;
    game.secondPlayer = new_car(YELLOW, 0, screen_width() / 2 + 100, screen_height() - 200);
    sprite_start_animation(game.car.car_sprite, "straight");
    play_sound_effect("carmotor", 1, 0.1);
    return game;
}

// Function to randomly spawn obstacle cars
void randomCars(game_data &game)
{
    int xLocations[] = {150, 250, 350, 450, 550, 650}; // Array to store the x locations
    car_model carModels[] = {POLICE, BLACK};           // Array of car models

    // Randomly select x locations for the cars
    for (int i = 0; i < CARS_COUNT; i++)
    {
        int randomIndex = rand() % CARS_COUNT; // Generate a random index between 0 and 6
        int x = xLocations[randomIndex];
        car_model model = carModels[rand() % 2];              // Randomly select a car model
        int speed = random_range(MIN_SPEED, MAX_SPEED);       // Randomly select a speed
        car_data laner_cars = new_car(model, speed, x, -300); // Create a car sprite at the selected x location
        game.cars.push_back(laner_cars);                      // Add the car to the game's car vector
    }
}
void randomPowerups(game_data &game)
{
    int xLocations[] = {50, 150, 250, 350, 450, 550, 650};     // Array to store the x locations
    powerup_type powerupTypes[] = {SHIELD, SPEED_BOOST, COIN}; // Array of powerup types

    // Randomly select x locations for the powerups
    for (int i = 0; i < POWERUP_COUNT; i++)
    {
        int randomIndex = rand() % 7; // Generate a random index between 0 and 6
        int x = xLocations[randomIndex];
        powerup_type type = powerupTypes[rand() % 4];             // Randomly select a powerup type
        int speed = random_range(MIN_SPEED - 2, MAX_SPEED - 2);   // Randomly select a speed
        powerup_data powerup = new_powerup(type, speed, x, -300); // Create a powerup sprite at the selected x location
        game.powerups.push_back(powerup);                         // Add the powerup to the game's powerup vector
    }
}

// Function to create a new game
void randomBarricades(game_data &game)
{
    for (int i = 0; i < BARRICADE_COUNT; i++)
    {
        barricade_data barricade = new_barricade(1, 50);
        game.barricades.push_back(barricade);
    }
}
// Function to check for collisions between the player's car and powerups
void check_collisions_powerup(game_data &game)
{
    if (game.twoPlayer)
    {
        for (int i = 0; i < game.powerups.size(); i++)
        {
            if (sprite_collision(game.car.car_sprite, game.powerups[i].powerup_sprite) || sprite_collision(game.secondPlayer.car_sprite, game.powerups[i].powerup_sprite))
            {
                switch (game.powerups[i].type)
                {
                case SHIELD:
                    game.shield = true;
                    break;
                case SPEED_BOOST:
                    game.speed_boost = true;
                    for (int j = 0; j < game.cars.size(); j++)
                    {
                        game.cars[j].speed = 8;
                    }
                    break;
                case SLOW_DOWN:
                    write_line("slow down");
                    for (int j = 0; j < game.cars.size(); j++)
                    {
                        game.cars[j].speed = 1;
                    }
                    break;
                case COIN:
                    game.score += 100;
                    break;
                }
                std::string powersounds[] = {"powerup1", "powerup2", "powerup3"};
                int randomIndex = rand() % 3;
                play_sound_effect(powersounds[randomIndex], 1);
                // Remove the powerup from the vector
                game.powerups.erase(game.powerups.begin() + i);
                // Decrement i to account for the removed element
                i--;
            }
        }
    }
    else
    {
        for (int i = 0; i < game.powerups.size(); i++)
        {
            if (sprite_collision(game.car.car_sprite, game.powerups[i].powerup_sprite))
            {
                switch (game.powerups[i].type)
                {
                case SHIELD:
                    game.shield = true;
                    break;
                case SPEED_BOOST:
                    game.speed_boost = true;
                    for (int j = 0; j < game.cars.size(); j++)
                    {
                        game.cars[j].speed = 8;
                    }
                    break;
                case SLOW_DOWN:
                    write_line("slow down");
                    for (int j = 0; j < game.cars.size(); j++)
                    {
                        game.cars[j].speed = 1;
                    }
                    break;
                case COIN:
                    game.score += 100;
                    break;
                }
                
                std::string powersounds[] = {"powerup1", "powerup2", "powerup3"};
                int randomIndex = rand() % 3;
                play_sound_effect(powersounds[randomIndex], 1);
                // Remove the powerup from the vector
                game.powerups.erase(game.powerups.begin() + i);
                // Decrement i to account for the removed element
                i--;
            }
        }
    }
}
void check_collisions_barricade(game_data &game)
{
    if (game.twoPlayer)
    {
        for (int i = 0; i < game.barricades.size(); i++)
        {
            if (sprite_collision(game.car.car_sprite, game.barricades[i].barricade_sprite) ||
                sprite_collision(game.secondPlayer.car_sprite, game.barricades[i].barricade_sprite))
            {
                play_sound_effect("carcrash", 1);
                delay(1500);
                game.game_over = true;
            }
        }
    }
    else
    {
        for (int i = 0; i < game.barricades.size(); i++)
        {
            if (sprite_collision(game.car.car_sprite, game.barricades[i].barricade_sprite))
            {
                play_sound_effect("carcrash", 1);
                delay(1500);
                game.game_over = true;
            }
        }
    }
}
// Function to handle user input
void handleInput(game_data &game)
{
    if (key_down(A_KEY))
    {
        sprite_set_x(game.car.car_sprite, sprite_x(game.car.car_sprite) - 5);
        sprite_start_animation(game.car.car_sprite, "left");
    }
    else if (key_down(D_KEY))
    {
        sprite_set_x(game.car.car_sprite, sprite_x(game.car.car_sprite) + 5);
        sprite_start_animation(game.car.car_sprite, "right");
    }
    else if (key_typed(R_KEY))
    {
        switch_car_skin(game.car);
    }
    else
    {
        sprite_start_animation(game.car.car_sprite, "straight");
    }
    if (game.twoPlayer)
    {
        if (key_down(LEFT_KEY))
        {
            sprite_set_x(game.secondPlayer.car_sprite, sprite_x(game.secondPlayer.car_sprite) - 5);
            sprite_start_animation(game.secondPlayer.car_sprite, "left");
        }
        else if (key_down(RIGHT_KEY))
        {
            sprite_set_x(game.secondPlayer.car_sprite, sprite_x(game.secondPlayer.car_sprite) + 5);
            sprite_start_animation(game.secondPlayer.car_sprite, "right");
        }
        else if (key_typed(NUM_0_KEY))
        {
            switch_car_skin(game.secondPlayer);
        }
        else
        {
            sprite_start_animation(game.secondPlayer.car_sprite, "straight");
        }
    }
}

// Function to draw the game
void draw_game(game_data &game)
{
    clear_screen(COLOR_GRAY);

    for (int i = 0; i < 6; i++)
    {
        draw_sprite(game.whitemarks[i]);
    }

    for (int j = 0; j < game.cars.size(); j++)
    {
        draw_car(game.cars[j]);
    }

    for (int j = 0; j < game.powerups.size(); j++)
    {
        draw_powerup(game.powerups[j]);
    }

    for (int j = 0; j < game.barricades.size(); j++)
    {
        draw_barricade(game.barricades[j]);
    }

    if (game.level % 2 == 1)
    {
        draw_bitmap("game", 0, 0, option_to_screen());
    }
    else
    {
        draw_bitmap("game2", 0, 0, option_to_screen());
    }
    if (game.finished)
    {
        draw_sprite(game.finish_line);
    }

    draw_text(to_string((int)game.score), COLOR_WHITE, "digi", 50, 650, 10, option_to_screen());
    draw_car(game.car);
    if (game.twoPlayer)
    {
        draw_car(game.secondPlayer);
    }
}

// Function to check and limit the car's position within the screen
void input_check_positions(game_data &game)
{
    if (sprite_x(game.car.car_sprite) <= 50)
    {
        sprite_set_x(game.car.car_sprite, 50);
    }
    else if (sprite_x(game.car.car_sprite) >= 650)
    {
        sprite_set_x(game.car.car_sprite, 650);
    }
}

// Function to remove cars that have gone out of range
void out_range(game_data &game)
{
    for (int j = 0; j < game.cars.size(); j++)
    {
        if (sprite_y(game.cars[j].car_sprite) > 600)
        {
            // Remove the element from the vector
            game.cars.erase(game.cars.begin() + j);
            // Decrement j to account for the removed element
            j--;
        }
    }
    out_range_powerup(game.powerups);
    out_range_barricade(game.barricades);
}

// Function to check for collisions between the player's car and obstacle cars
void check_collisions(game_data &game)
{
    if (game.twoPlayer)
    {
        for (int i = 0; i < game.cars.size(); i++)
        {
            if (sprite_collision(game.car.car_sprite, game.cars[i].car_sprite) || sprite_collision(game.secondPlayer.car_sprite, game.cars[i].car_sprite))
            {
                if (game.shield)
                {
                    game.shield = false;
                    game.cars.erase(game.cars.begin() + i);
                    continue;
                }
                else
                {
                    play_sound_effect("carcrash", 1);
                    delay(1500);
                    game.game_over = true;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < game.cars.size(); i++)
        {
            if (sprite_collision(game.car.car_sprite, game.cars[i].car_sprite))
            {
                if (game.shield)
                {
                    game.shield = false;
                    game.cars.erase(game.cars.begin() + i);
                    continue;
                }
                else
                {
                    play_sound_effect("carcrash", 1);
                    delay(1500);
                    game.game_over = true;
                }
            }
        }
    }
}

bool game_playing = false;
sprite new_finish_line()
{
    bitmap finish_line = bitmap_named("finish");
    sprite new_sprite = create_sprite(finish_line);
    sprite_set_x(new_sprite, 50);
    sprite_set_y(new_sprite, -600);
    sprite_set_dy(new_sprite, 3);
    return new_sprite;
}
void level_up(game_data &game)
{
    // if score is just above the level up score
    if (game.score >= 500 && game.score <= 6000)
    {
        game.finish_line = new_finish_line();
        game.finished = true;
    }
    if (game.score >= 1000 && game.score <= 1100)
    {
        game.finish_line = new_finish_line();
        game.finished = true;
    }
    if (game.score >= 1500 && game.score <= 1600)
    {
        game.finish_line = new_finish_line();
        game.finished = true;
    }
}
// Function to update the game state
void update_game(game_data &game)
{

    // Adjust car speed based on the level
    MIN_SPEED = 2.0 + (game.level - 1) * 0.5; // Increase minimum speed gradually
    MAX_SPEED = 6.0 + (game.level - 1) * 1.0; // Increase maximum speed gradually

    // Adjust car spawn frequency based on the level
    CARS_SPAWN_DURATION = 5 - (game.level - 1); // Decrease spawn duration gradually
    if (CARS_SPAWN_DURATION < 2)
        CARS_SPAWN_DURATION = 2; // Minimum spawn duration of 2 seconds

    // Adjust the number of cars based on the level
    CARS_COUNT = 3 + (game.level - 1); // Increase the number of cars gradually
    if (CARS_COUNT > 6)
        CARS_COUNT = 6; // Maximum of 8 cars

    game.score += 0.3;
    update_car(game.car);
    if (game.twoPlayer)
    {
        update_car(game.secondPlayer);
    }
    out_range(game);
    for (int j = 0; j < game.cars.size(); j++)
    {
        update_car(game.cars[j]);
    }
    for (int i = 0; i < 6; i++)
    {
        update_sprite(game.whitemarks[i]);
    }
    for (int j = 0; j < game.powerups.size(); j++)
    {
        update_powerup(game.powerups[j]);
    }
    for (int j = 0; j < game.barricades.size(); j++)
    {
        update_barricade(game.barricades[j]);
    }
    level_up(game);
    if (game.finished)
    {

        update_sprite(game.finish_line);

        if (sprite_y(game.finish_line) > sprite_y(game.car.car_sprite) + 100)
        {
            game.level++;
            game.finished = false;

            game.cars.clear();
            game.powerups.clear();
            game.barricades.clear();
        }
    }
    input_check_positions(game);
    handleInput(game);

    // Call the randomCars function every CARS_SPAWN_DURATION seconds
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_call_time).count();
    if (elapsed_time >= CARS_SPAWN_DURATION)
    {
        randomCars(game);

        if (game.level * rnd(5) % 10 == 0 && BARRICADE_COUNT < 4)
        {
            BARRICADE_COUNT++;
        }
        if (game.level * rnd(5) % 10 == 0 && POWERUP_COUNT < 5)
        {
            POWERUP_COUNT++;
        }

        randomPowerups(game);

        randomBarricades(game);

        last_call_time = current_time;
    }
}

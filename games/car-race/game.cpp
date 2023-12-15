#include "splashkit.h"
#include "game.h"
#include <cmath>
#include <vector>
// for the time functions, cars to spawn
#include <chrono>
#include <thread>
using namespace std;
using std::vector;
#define MIN_SPEED 5.0         // min speed of obstacle car
#define MAX_SPEED 10.0        // max speed of obstacle car
#define CARS_COUNT 7          // total obstacle cars to be spwaned
#define CARS_SPAWN_DURATION 3 // Time between each car collection
void map_setup(game_data &game)
{
    int laneSpacing = 100;
    int leftEdgePosition = screen_width() / 2 - laneSpacing * 7 / 2;

    bitmap roadEdge = create_bitmap("roadEdge", 10, screen_height());
    draw_line_on_bitmap(roadEdge, COLOR_WHITE, 0, 0, 0, screen_height(), option_line_width(10));
    setup_collision_mask(roadEdge);

    point_2d abc = point_at(leftEdgePosition, 0);
    game.game_over = false;
    abc = point_at(leftEdgePosition + (laneSpacing * 7) - 5, 0);
    bitmap road = create_bitmap("roadmarkings", 10, screen_height());
    for (int i = 0; i < screen_height(); i += 20)
    {
        draw_line_on_bitmap(road, COLOR_WHITE, 1, i, 1, i + 10, option_line_width(8));
    }

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

double random_range(double min, double max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}
// Function to randomly spawn obstacle cars
void randomCars(game_data &game)
{
    int xLocations[] = {50, 150, 250, 350, 450, 550, 650}; // Array to store the x locations
    car_model carModels[] = {POLICE, BLACK};               // Array of car models

    // Randomly select x locations for the cars
    for (int i = 0; i < CARS_COUNT; i++)
    {
    int randomIndex = rand() % CARS_COUNT; // Generate a random index between 0 and 6
    int x = xLocations[randomIndex];
    if (x!=9999) // prevent the usage of the changed spawn location, which is changed 
    {            // to prevent clipping from multi NPC car lanes
        car_model model = carModels[rand() % 2]; // Randomly select a car model
        int speed = random_range(MIN_SPEED, MAX_SPEED); // Randomly select a speed
        car_data laner_cars = new_car(model, speed, x, -300); // Create a car sprite at the selected x location
        game.cars.push_back(laner_cars); // Add the car to the game's car vector
        xLocations[randomIndex]=9999; // change the spawn location to prevent npc cars clipping through eachother
    }
   }
}
auto last_call_time = std::chrono::steady_clock::now(); // Track the last time the function was called
game_data new_game()
{
    game_data game;
    game.car = new_car(RED, 0, screen_width() / 2, screen_height() - 200);
    map_setup(game);
    game.score = 0;
    sprite_start_animation(game.car.car_sprite, "straight");
    play_sound_effect("carmotor", 1, 0.7);
    return game;
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
    else if (key_down(W_KEY))
    {
        sprite_set_y(game.car.car_sprite, sprite_y(game.car.car_sprite) - 5);
        sprite_start_animation(game.car.car_sprite, "straight");
    }
    else if (key_down(S_KEY))
    {
        sprite_set_y(game.car.car_sprite, sprite_y(game.car.car_sprite) + 5);
        sprite_start_animation(game.car.car_sprite, "straight");
    }
    else
    {
        sprite_start_animation(game.car.car_sprite, "straight");
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

    draw_bitmap("game", 0, 0, option_to_screen());

    draw_text(to_string((int)game.score), COLOR_WHITE, "digi", 50, 650, 10, option_to_screen());
    draw_car(game.car);
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

    if (sprite_y(game.car.car_sprite) <= 30)
    {
        sprite_set_y(game.car.car_sprite, 30);
    }
    else if (sprite_y(game.car.car_sprite) >= screen_height() - 175)
    {
        sprite_set_y(game.car.car_sprite, screen_height() -175);
    }

}
// Function to remove cars that have gone out of range
void out_range(game_data &game)
{
    for (int j = 0; j < game.cars.size(); j++)
    {
        if (sprite_y(game.cars[j].car_sprite) > 600)
        {
            // remove the element from this vector
            game.cars.erase(game.cars.begin() + j);
            // Decrement j to account for the removed element
            j--;
        }
    }
}
// Function to check for collisions between the player's car and obstacle cars
void check_collisions(game_data &game)
{
    for (int i = 0; i < game.cars.size(); i++)
    {
        if (sprite_collision(game.car.car_sprite, game.cars[i].car_sprite))
        {
            play_sound_effect("carcrash", 1);
            delay(1500);
            game.game_over = true;
        }
    }
}
// Function to update the game state
void update_game(game_data &game)
{
    game.score += 0.3;
    update_car(game.car);
    for (int j = 0; j < game.cars.size(); j++)
    {
        update_car(game.cars[j]);
    }
    for (int i = 0; i < 6; i++)
    {
        update_sprite(game.whitemarks[i]);
    }

    input_check_positions(game);
    handleInput(game);
    // Call the randomCars function every CARS_SPAWN_DURATION seconds
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_call_time).count();
    if (elapsed_time >= CARS_SPAWN_DURATION)
    {
        randomCars(game);
        last_call_time = current_time;
    }
}

#include "hallwaylevel.h"
#include <iostream>
#include <sstream>

HallwayLevel::HallwayLevel()
    : original_width(0), original_height(0),
      scaled_width(0), scaled_height(0),
      scale_factor(0), y_offset(0),
      world_left_boundary(0), world_right_boundary(0),
      ground_level(0), ceiling_level(0),
      image_offset_x(0),
      nearby_interactable(nullptr),
      show_boundaries(true)
{}

void HallwayLevel::load()
{
    background = load_bitmap("hallway_bg", "assets/envs/Apartment Hallway.png");
    if (!background) {
        write_line("ERROR: Could not load hallway background!");
        return;
    }

    original_width  = bitmap_width(background);
    original_height = bitmap_height(background);
    write_line("Loaded Apartment Hallway.png - Dimensions: " +
               std::to_string(original_width) + "x" + std::to_string(original_height));

    const float TARGET_HEIGHT = 450.0f;
    scale_factor = TARGET_HEIGHT / original_height;
    scaled_height = original_height * scale_factor;
    scaled_width  = original_width  * scale_factor;

    // ---------- Explicit world boundaries (independent of image) ----------
    world_left_boundary  = 10.0f;                         // you can change this
    world_right_boundary = 1490.0f;                 // or set any value, e.g. 1400
    // ---------------------------------------------------------------------

    const float FLOOR_OFFSET = 150.0f * scale_factor;   // pixels from image bottom to floor
    ground_level  = scaled_height - FLOOR_OFFSET;
    ceiling_level = ground_level - 400.0f;               // playable height

    image_offset_x = 125.0f;    // left edge of image aligns with world_left_boundary
    y_offset = (600.0f - scaled_height) / 2.0f;         // vertical centering

    write_line("World boundaries: left=" + std::to_string(world_left_boundary) +
               " right=" + std::to_string(world_right_boundary) +
               " ground=" + std::to_string(ground_level) +
               " ceiling=" + std::to_string(ceiling_level));

    setup_interactables();
    write_line("Hallway level loaded successfully");
}

void HallwayLevel::unload()
{
    nearby_interactable = nullptr;
    interactables.clear();
}

void HallwayLevel::update(Player& player)
{
    for (auto& i : interactables) i.update();
}

void HallwayLevel::draw(const point_2d& camera_pos) const
{
    clear_screen(COLOR_BLACK);

    // Draw background image using image_offset_x
    float image_world_x = world_left_boundary + image_offset_x;
    float draw_x = image_world_x - camera_pos.x;
    float draw_y = y_offset - camera_pos.y;
    drawing_options opts = option_scale_bmp(scale_factor, scale_factor);
    draw_bitmap(background, draw_x, draw_y, opts);

    for (const auto& i : interactables)
        i.draw(camera_pos, y_offset);

    if (show_boundaries)
        draw_boundaries(camera_pos);
}

void HallwayLevel::draw_boundaries(const point_2d& camera_pos) const
{
    auto wsx = [&](float world_x) { return world_x - camera_pos.x; };
    auto wsy = [&](float world_y) { return world_y - camera_pos.y + y_offset; };

    color fl_ceil = COLOR_RED;
    color wall    = COLOR_GREEN;

    // Floor
    float sf = wsy(ground_level);
    draw_line(fl_ceil, 0, sf, 800, sf);
    draw_text("FLOOR", fl_ceil, 10, sf - 20);

    // Ceiling
    float sc = wsy(ceiling_level);
    draw_line(fl_ceil, 0, sc, 800, sc);
    draw_text("CEILING", fl_ceil, 10, sc + 5);

    // Left wall
    float sl = wsx(world_left_boundary);
    if (sl >= -50 && sl <= 850) {
        draw_line(wall, sl, 0, sl, 600);
        draw_text("LEFT WALL", wall, sl + 5, 100);
    }

    // Right wall
    float sr = wsx(world_right_boundary);
    if (sr >= -50 && sr <= 850) {
        draw_line(wall, sr, 0, sr, 600);
        draw_text("RIGHT WALL", wall, sr - 80, 100);
    } else {
        draw_text("RIGHT WALL: " + std::to_string((int)world_right_boundary), wall, 600, 100);
    }
}

void HallwayLevel::check_boundaries(Player& player)
{
    point_2d pos = player.get_position();
    const float HALF_WIDTH = Player::HITBOX_WIDTH / 2.0f;

    // Left wall
    const float LEFT_WALL = world_left_boundary + HALF_WIDTH;
    if (pos.x < LEFT_WALL) pos.x = LEFT_WALL;

    // Right wall
    const float RIGHT_WALL = world_right_boundary - HALF_WIDTH;
    if (pos.x > RIGHT_WALL) pos.x = RIGHT_WALL;

    // Floor
    if (pos.y > ground_level) {
        pos.y = ground_level;
        if (!player.get_is_grounded() && player.get_state() == STATE_FALL)
            player.set_state(STATE_IDLE);
        player.set_grounded(true);
        player.on_land();   // <-- NEW: resets double jump & dash cooldown
    }

    // Ceiling
    if (pos.y < ceiling_level) {
        pos.y = ceiling_level;
        player.stop_vertical_movement();
    }

    player.set_position(pos);
}

void HallwayLevel::check_interactions(const Player& player)
{
    nearby_interactable = nullptr;
    for (auto& i : interactables) {
        if (i.check_collision(player.get_position())) {
            nearby_interactable = &i;
            break;
        }
    }
}

void HallwayLevel::setup_interactables()
{
    interactables.clear();
    nearby_interactable = nullptr;

    // Doors placed in world coordinates (relative to boundaries)
    interactables.push_back(Interactable("door1", 90.0f,  ground_level - 85.0f, 40.0f, 60.0f, "Apartment 101"));
    interactables.push_back(Interactable("door2", 250.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 102"));
    interactables.push_back(Interactable("door3", 400.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 103"));
    interactables.push_back(Interactable("door5", 890.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 105"));
    interactables.push_back(Interactable("door6", 1060.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 106"));
    interactables.push_back(Interactable("door7", 1220.0f, ground_level - 85.0f, 40.0f, 60.0f, "Apartment 107"));
    interactables.push_back(Interactable("hexit", 1445.0f, ground_level - 25.0f, 40.0f, 60.0f, "Exit To Lobby"));
}

void HallwayLevel::trigger_interaction(const std::string& id)
{
    for (auto& i : interactables) {
        if (i.get_id() == id && !i.get_is_triggered()) {
            i.trigger();
            break;
        }
    }
}
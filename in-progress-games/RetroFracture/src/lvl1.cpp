#include "lvl1.h"
#include <iostream>
#include <sstream>
#include <algorithm>   // for std::clamp if needed

Level1::Level1()
    : original_width(0), original_height(0),
      scaled_width(0), scaled_height(0),
      scale_factor(0), y_offset(0),
      world_left_boundary(0), world_right_boundary(0),
      ground_level(0), ceiling_level(0),
      image_offset_x(0),
      global_ground_level(1000.0f),    // <-- moved before show_boundaries
      exit_door("exit_door", 100.0f, 610.0f, 40.0f, 60.0f, "EXIT TO HALLWAY"),
      door_triggered(false),
      show_boundaries(true)
{
}

void Level1::load()
{
    background = load_bitmap("lvl1_bg", "assets/envs/lvl1_platform.png");
    if (!background) {
        write_line("ERROR: Could not load apartment background!");
        return;
    }

    original_width  = bitmap_width(background);
    original_height = bitmap_height(background);
    write_line("Loaded lvl1_platform.png - Dimensions: " +
               std::to_string(original_width) + "x" + std::to_string(original_height));

    // Scale image to fit screen height
    const float TARGET_HEIGHT = 600.0f;
    scale_factor = TARGET_HEIGHT / original_height;
    scaled_height = original_height * scale_factor;
    scaled_width  = original_width  * scale_factor;

    // World boundaries
    world_left_boundary  = 0.0f;
    world_right_boundary = scaled_width;          // room width = image width
    ceiling_level = 50.0f;

    // Image placement
    image_offset_x = 335.0f;
    y_offset = (600.0f - scaled_height) / 2.0f;  // likely 0

    // ---------- NEW: define the walkable ground polyline ----------
    ground_path.clear();
    // !! IMPORTANT – adapt these points to match YOUR lvl1_platform.png after scaling !!
    // Points must be sorted by increasing x, each point is a spot on the TOP surface of the platform.
    // Example (a flat floor with a slope in the middle):
    ground_path.push_back(point_at(0, 445));
    ground_path.push_back(point_at(595, 445));
    ground_path.push_back(point_at(595, 375));
    ground_path.push_back(point_at(1020, 375));
    ground_path.push_back(point_at(1020, 600));
    ground_path.push_back(point_at(1235, 600));
    ground_path.push_back(point_at(1235, 390));
    ground_path.push_back(point_at(1550, 390));

    // Fallback level for gaps (player will fall if x outside the path)
    global_ground_level = 1500.0f;               // way below screen, instant death later

    // ---------- NEW: define vertical interior walls ----------
    wall_segments.clear();
    // Example walls – place them where your image has unpassable vertical obstacles.
    // (x, y_top, y_bottom) – y_top < y_bottom because y increases downward.
    // Wall thickness is 10 pixels (half_width = 5).
    wall_segments.push_back(WallSegment(595.0f, 375.0f, 600.0f));
    wall_segments.push_back(WallSegment(1020.0f, 375.0f, 600.0f));
    wall_segments.push_back(WallSegment(1235.0f, 390.0f, 600.0f));

    // ---------- Ground level for camera ----------
    // Use the floor at the player start position (x = 100).
    const float SPAWN_X = 100.0f;
    ground_level = get_ground_y(SPAWN_X);

    // ---------- Exit door ----------
    float door_x = 100.0f;
    float floor_at_door = get_ground_y(door_x);
    exit_door = Interactable("exit_door", door_x, floor_at_door - 85.0f, 40.0f, 60.0f, "EXIT TO HALLWAY");

    write_line("Apartment level loaded with polyline ground.");
    write_line("Floor at door (x=100): " + std::to_string(floor_at_door));
}

// --- unload, update, draw unchanged except for boundary debug (see draw_boundaries) ---
void Level1::unload() {}
void Level1::update(Player& player) {}

void Level1::draw(const Player& player, const point_2d& camera_pos) const
{
    clear_screen(COLOR_BLACK);

    // Background
    float image_world_x = world_left_boundary + image_offset_x;
    float draw_x = image_world_x - camera_pos.x;
    float draw_y = y_offset - camera_pos.y;
    drawing_options opts = option_scale_bmp(scale_factor, scale_factor);
    draw_bitmap(background, draw_x, draw_y, opts);

    // Exit door
    exit_door.draw(camera_pos, y_offset);

    // Debug: draw boundaries (now also shows polyline ground and walls)
    if (show_boundaries)
        draw_boundaries(camera_pos);
}

void Level1::draw_boundaries(const point_2d& camera_pos) const
{
    auto wsx = [&](float world_x) { return world_x - camera_pos.x; };
    auto wsy = [&](float world_y) { return world_y - camera_pos.y + y_offset; };

    color fl_ceil = COLOR_RED;
    color wall    = COLOR_GREEN;

    // Ceiling
    float sc = wsy(ceiling_level);
    draw_line(fl_ceil, 0, sc, 800, sc);
    draw_text("CEILING", fl_ceil, 10, sc + 5);

    // Left / right world boundaries
    float sl = wsx(world_left_boundary);
    if (sl >= -50 && sl <= 850) {
        draw_line(wall, sl, 0, sl, 600);
        draw_text("LEFT WALL", wall, sl + 5, 100);
    }
    float sr = wsx(world_right_boundary);
    if (sr >= -50 && sr <= 850) {
        draw_line(wall, sr, 0, sr, 600);
        draw_text("RIGHT WALL", wall, sr - 80, 100);
    }

    // Draw polyline ground (debug)
    if (ground_path.size() > 1) {
        for (size_t i = 0; i < ground_path.size() - 1; ++i) {
            point_2d p1 = point_at(wsx(ground_path[i].x), wsy(ground_path[i].y));
            point_2d p2 = point_at(wsx(ground_path[i+1].x), wsy(ground_path[i+1].y));
            draw_line(COLOR_YELLOW, p1.x, p1.y, p2.x, p2.y);
        }
    }

    // Draw vertical walls (debug)
    for (const auto& w : wall_segments) {
        float wx = wsx(w.x);
        float wy1 = wsy(w.y_top);
        float wy2 = wsy(w.y_bottom);
        draw_line(COLOR_MAGENTA, wx, wy1, wx, wy2);
    }
}

// ---------- NEW: get_ground_y implementation ----------
float Level1::get_ground_y(float x) const
{
    if (ground_path.size() < 2)
        return global_ground_level;

    // Find the segment that contains x
    for (size_t i = 0; i < ground_path.size() - 1; ++i) {
        float x1 = ground_path[i].x;
        float x2 = ground_path[i+1].x;
        if (x >= x1 && x <= x2) {
            float t = (x - x1) / (x2 - x1);
            float y1 = ground_path[i].y;
            float y2 = ground_path[i+1].y;
            return y1 + t * (y2 - y1);
        }
    }
    // x is outside the defined path
    return global_ground_level;
}

void Level1::check_boundaries(Player& player)
{
    point_2d pos = player.get_position();
    const float HALF_WIDTH = Player::HITBOX_WIDTH / 2.0f;
    const float HITBOX_HEIGHT = Player::HITBOX_HEIGHT;

    // ---------- World left/right boundaries ----------
    const float LEFT_WALL  = world_left_boundary + HALF_WIDTH;
    const float RIGHT_WALL = world_right_boundary - HALF_WIDTH;
    if (pos.x < LEFT_WALL)  pos.x = LEFT_WALL;
    if (pos.x > RIGHT_WALL) pos.x = RIGHT_WALL;

    // ---------- Floor (polyline) ----------
    float floor_y = get_ground_y(pos.x);
    if (pos.y >= floor_y)
    {
        pos.y = floor_y;
        if (!player.get_is_grounded() && player.get_state() == STATE_FALL)
            player.set_state(STATE_IDLE);
        player.set_grounded(true);
        player.on_land();   // <-- NEW
    }
    // else: player is above floor – let gravity work, do not clamp.

    // ---------- Ceiling ----------
    if (pos.y < ceiling_level)
    {
        pos.y = ceiling_level;
        player.stop_vertical_movement();
    }

    // ---------- NEW: interior vertical walls ----------
    float player_left   = pos.x - HALF_WIDTH;
    float player_right  = pos.x + HALF_WIDTH;
    float player_top    = pos.y - HITBOX_HEIGHT;
    float player_bottom = pos.y;

    for (const auto& w : wall_segments)
    {
        float wall_left  = w.x - w.half_width;
        float wall_right = w.x + w.half_width;
        if (player_bottom <= w.y_top || player_top >= w.y_bottom)
            continue;
        if (player_right > wall_left && player_left < wall_right)
        {
            if (pos.x < w.x)
                pos.x = wall_left - HALF_WIDTH;
            else
                pos.x = wall_right + HALF_WIDTH;
            player_left  = pos.x - HALF_WIDTH;
            player_right = pos.x + HALF_WIDTH;
        }
    }

    player.set_position(pos);
}

void Level1::check_interactions(const Player& player)
{
    door_triggered = exit_door.check_collision(player.get_position());
}
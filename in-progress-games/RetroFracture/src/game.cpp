#include "game.h"
#include <iostream>

Game::Game()
    : quit_game(false),
      current_level(LEVEL_HALLWAY),
      camera_pos(point_at(0, 0)),
      nearby_interaction_prompt(""),
      show_hitbox(false)
{
    enter_hallway();
}

void Game::enter_hallway()
{
    current_level = LEVEL_HALLWAY;
    hallway.load();

    const float START_X = 1435.0f;
    player.set_position(point_at(START_X, hallway.get_ground_level()));
    player.set_grounded(true);
    player.set_state(STATE_IDLE);

    // Disable advanced abilities in the hallway
    player.enable_double_jump(false);
    player.enable_dash(false);

    camera_pos = point_at(START_X - 400.0f, hallway.get_ground_level() - 300.0f);
    if (camera_pos.x < hallway.get_world_left_boundary())
        camera_pos.x = hallway.get_world_left_boundary();
    if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;

    nearby_interaction_prompt = "";
    write_line("Entered hallway level");
}

void Game::enter_apartment101()
{
    current_level = LEVEL_APARTMENT_101;
    apartment101.load();
    apartment101.reset_door();

    const float START_X = 100.0f;
    player.set_position(point_at(START_X, apartment101.get_ground_level()));
    player.set_grounded(true);
    player.set_state(STATE_IDLE);
    player.on_land();   // reset jump / dash status

    // Enable abilities for this level
    player.enable_double_jump(true);
    player.enable_dash(true);

    camera_pos = point_at(START_X - 400.0f, apartment101.get_ground_level() - 300.0f);
    nearby_interaction_prompt = "";
    write_line("Entered Apartment 101 (Level 1)");
}

void Game::run()
{
    while (!quit_game && !quit_requested()) {
        process_events();
        handle_input();
        update();
        draw();
        refresh_screen(60);
    }
}

void Game::update()
{
    player.update();
    update_camera();

    nearby_interaction_prompt = "";

    switch (current_level) {
        case LEVEL_HALLWAY:
            hallway.check_boundaries(player);
            hallway.check_interactions(player);
            {
                Interactable* nearby = hallway.get_nearby_interactable();
                if (nearby && !nearby->get_is_triggered())
                    nearby_interaction_prompt = "Press E to enter " + nearby->get_label();
            }
            break;

        case LEVEL_APARTMENT_101:
            apartment101.check_boundaries(player);
            apartment101.check_interactions(player);
            if (apartment101.is_door_triggered())
                nearby_interaction_prompt = "Press E to exit to hallway";
            break;
    }
}

void Game::update_camera()
{
    const float CAMERA_LERP_FACTOR = 0.1f;
    point_2d player_pos = player.get_position();

    float target_x = player_pos.x - 400.0f;
    float target_y = player_pos.y - 300.0f;

    if (current_level == LEVEL_APARTMENT_101) {
        const float APARTMENT_CAMERA_Y_OFFSET = -180.0f;
        target_y += APARTMENT_CAMERA_Y_OFFSET;
    }

    camera_pos.x += (target_x - camera_pos.x) * CAMERA_LERP_FACTOR;
    camera_pos.y += (target_y - camera_pos.y) * CAMERA_LERP_FACTOR;

    if (current_level == LEVEL_HALLWAY) {
        const float SCREEN_WIDTH = 800.0f;
        float world_left  = hallway.get_world_left_boundary();
        float world_right = hallway.get_world_right_boundary();

        if (camera_pos.x < world_left)
            camera_pos.x = world_left;
        if (camera_pos.x > world_right - SCREEN_WIDTH)
            camera_pos.x = world_right - SCREEN_WIDTH;

        float min_camera_y = hallway.get_ground_level() - 450.0f;
        if (camera_pos.y < min_camera_y) camera_pos.y = min_camera_y;
    }

    float max_camera_y = get_current_ground_level() - 200.0f;
    if (camera_pos.y > max_camera_y) camera_pos.y = max_camera_y;
    if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;
}

point_2d Game::world_to_screen(point_2d world_pos) const
{
    float background_y_offset = get_current_background_y_offset();
    return point_at(world_pos.x - camera_pos.x,
                    world_pos.y - camera_pos.y + background_y_offset);
}

float Game::get_current_ground_level() const
{
    switch (current_level) {
        case LEVEL_HALLWAY:      return hallway.get_ground_level();
        case LEVEL_APARTMENT_101: return apartment101.get_ground_level();
        default:                 return 500.0f;
    }
}

float Game::get_current_background_y_offset() const
{
    switch (current_level) {
        case LEVEL_HALLWAY:      return hallway.get_y_offset();
        case LEVEL_APARTMENT_101: return 0.0f;
        default:                 return 0.0f;
    }
}

void Game::draw()
{
    switch (current_level) {
        case LEVEL_HALLWAY:
            hallway.draw(camera_pos);
            break;
        case LEVEL_APARTMENT_101:
            apartment101.draw(player, camera_pos);
            break;
    }

    point_2d screen_pos = world_to_screen(player.get_position());
    player.draw_at(screen_pos);

    if (show_hitbox) {
        rectangle hitbox = player.get_bounding_box();
        point_2d tl = world_to_screen(point_at(hitbox.x, hitbox.y));
        point_2d tr = world_to_screen(point_at(hitbox.x + hitbox.width, hitbox.y));
        point_2d br = world_to_screen(point_at(hitbox.x + hitbox.width, hitbox.y + hitbox.height));
        point_2d bl = world_to_screen(point_at(hitbox.x, hitbox.y + hitbox.height));
        color hc = COLOR_RED;
        draw_line(hc, tl.x, tl.y, tr.x, tr.y);
        draw_line(hc, tr.x, tr.y, br.x, br.y);
        draw_line(hc, br.x, br.y, bl.x, bl.y);
        draw_line(hc, bl.x, bl.y, tl.x, tl.y);
    }

    bool in_hallway = is_in_hallway();
    hud.draw_controls(in_hallway);
    hud.draw_debug_info(player, camera_pos, in_hallway);

    std::string level_name = in_hallway ? "Apartment Hallway" : "Apartment 101";
    hud.draw_level_info(level_name, in_hallway);
    hud.draw_interaction_prompt(nearby_interaction_prompt, in_hallway);
}

void Game::handle_input()
{
    bool in_hallway = is_in_hallway();

    // --- movement (A/D) ---
    if (!player.get_is_dashing()) {
        if (key_down(A_KEY)) {
            player.move_left();
        } else if (key_down(D_KEY)) {
            player.move_right();
        } else {
            player.stop_moving();
        }
    }

    // --- jump / double jump ---
    if (key_typed(W_KEY)) player.jump();

    // --- dash ---
    if (key_typed(LEFT_SHIFT_KEY)) {
        player.dash();
    }

    // --- interaction ---
    if (key_typed(E_KEY)) {
        if (in_hallway) {
            Interactable* nearby = hallway.get_nearby_interactable();
            if (nearby && !nearby->get_is_triggered()) {
                hallway.trigger_interaction(nearby->get_id());
                if (nearby->get_id() == "door1") enter_apartment101();
                else if (nearby->get_id() == "door2") write_line("Apartment 102 (coming soon!)");
                else if (nearby->get_id() == "door3") write_line("Apartment 103 (coming soon!)");
                else if (nearby->get_id() == "door5") write_line("Apartment 105 (coming soon!)");
                else if (nearby->get_id() == "door6") write_line("Apartment 106 (coming soon!)");
                else if (nearby->get_id() == "door7") write_line("Apartment 107 (coming soon!)");
                else if (nearby->get_id() == "hexit") write_line("Exit to lobby (coming soon!)");
            }
        } else {
            if (apartment101.is_door_triggered()) {
                current_level = LEVEL_HALLWAY;
                const float DOOR_X = 80.0f;
                player.set_position(point_at(DOOR_X, hallway.get_ground_level()));
                player.set_grounded(true);
                player.set_state(STATE_IDLE);
                camera_pos = point_at(DOOR_X - 400.0f, hallway.get_ground_level() - 300.0f);
                if (camera_pos.x < hallway.get_world_left_boundary())
                    camera_pos.x = hallway.get_world_left_boundary();
                if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;
                nearby_interaction_prompt = "";
                write_line("Exited to hallway at apartment 101 door");
            }
        }
    }

    if (key_down(ESCAPE_KEY)) {
        if (!in_hallway) {
            current_level = LEVEL_HALLWAY;
            const float DOOR_X = 80.0f;
            player.set_position(point_at(DOOR_X, hallway.get_ground_level()));
            player.set_grounded(true);
            player.set_state(STATE_IDLE);
            camera_pos = point_at(DOOR_X - 400.0f, hallway.get_ground_level() - 300.0f);
            if (camera_pos.x < hallway.get_world_left_boundary())
                camera_pos.x = hallway.get_world_left_boundary();
            if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;
            nearby_interaction_prompt = "";
            write_line("Returned to hallway at apartment 101 door (ESC)");
        } else {
            quit_game = true;
        }
    }

    if (key_typed(R_KEY)) {
        if (in_hallway) {
            const float START_X = 1435.0f;
            player.set_position(point_at(START_X, hallway.get_ground_level()));
            player.set_grounded(true);
            player.set_state(STATE_IDLE);
            camera_pos = point_at(START_X - 400.0f, hallway.get_ground_level() - 300.0f);
            if (camera_pos.x < hallway.get_world_left_boundary())
                camera_pos.x = hallway.get_world_left_boundary();
            if (camera_pos.y < -100.0f) camera_pos.y = -100.0f;
            hallway.setup_interactables();
        } else {
            const float START_X = 100.0f;
            player.set_position(point_at(START_X, apartment101.get_ground_level()));
            player.set_grounded(true);
            player.set_state(STATE_IDLE);
            camera_pos = point_at(START_X - 400.0f, apartment101.get_ground_level() - 300.0f);
            player.on_land();   // reset abilities after reset
        }
        nearby_interaction_prompt = "";
    }

    if (key_typed(H_KEY)) {
        show_hitbox = !show_hitbox;
        write_line("Player hitbox: " + std::string(show_hitbox ? "ON" : "OFF"));
    }
}
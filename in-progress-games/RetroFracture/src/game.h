#ifndef GAME_H
#define GAME_H

#include "splashkit.h"
#include "player.h"
#include "hud.h"
#include "hallwaylevel.h"
#include "lvl1.h"

class Game {
private:
    Player player;
    bool quit_game;

    enum CurrentLevel {
        LEVEL_HALLWAY,
        LEVEL_APARTMENT_101
    };
    CurrentLevel current_level;
    point_2d camera_pos;

    HallwayLevel hallway;
    Level1 apartment101;

    HUD hud;
    std::string nearby_interaction_prompt;
    bool show_hitbox;

public:
    Game();

    void run();
    void update();
    void draw();
    void handle_input();

    void enter_hallway();
    void enter_apartment101();

    void update_camera();
    point_2d world_to_screen(point_2d world_pos) const;

    float get_current_ground_level() const;
    float get_current_background_y_offset() const;
    bool is_in_hallway() const { return current_level == LEVEL_HALLWAY; }
};

#endif
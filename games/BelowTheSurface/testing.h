#include "splashkit.h"
#include "player.h"
#include <memory>

#pragma once

void draw_hitbox(rectangle hitbox)
{
    draw_rectangle(COLOR_GREEN, hitbox);
};

void test_camera_on(shared_ptr<Player> player)
{
    center_camera_on(player->get_player_sprite(), 0, 0);
};
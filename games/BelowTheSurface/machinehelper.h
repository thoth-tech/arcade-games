#include "splashkit.h"

#pragma once

void start_animation(bool facing_left, sprite enemy_sprite,string left_anim, string right_anim)
{
    if(facing_left)
        sprite_start_animation(enemy_sprite, left_anim);
    else
        sprite_start_animation(enemy_sprite, right_anim);
}

void set_proper_direction(sprite sprite, string animation)
{
    string lower = to_lowercase(animation);
    if(sprite_animation_name(sprite) != lower)
        sprite_start_animation(sprite, animation);
}
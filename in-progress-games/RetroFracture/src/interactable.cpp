#include "interactable.h"
#include "player.h"
#include <iostream>

Interactable::Interactable(std::string id, float x, float y, float width, float height, std::string label)
    : id(id), x(x), y(y), width(width), height(height),
      is_active(true), is_triggered(false), label(label)
{}

void Interactable::draw(point_2d camera_pos, float background_y_offset) const
{
    if (!is_active) return;

    float screen_x = x - camera_pos.x;
    float screen_y = y - camera_pos.y + background_y_offset;

    if (is_triggered) {
        draw_rectangle(COLOR_GREEN, screen_x, screen_y, width, height);
    } else {
        draw_rectangle(COLOR_YELLOW, screen_x, screen_y, width, height);
        draw_rectangle(COLOR_BLACK, screen_x + 2, screen_y + 2, width - 4, height - 4);
    }

    if (!label.empty() && !is_triggered) {
        float text_width_estimate = label.length() * 8.0f;
        float label_x = screen_x + (width - text_width_estimate) / 2;
        draw_text(label, COLOR_WHITE, label_x, screen_y - 20);
    }

    if (!is_triggered) {
        float center_x = screen_x + width / 2 - 10;
        float center_y = screen_y + height / 2 - 10;
        draw_text("[E]", COLOR_CYAN, center_x, center_y);
    }
}

void Interactable::update()
{}

bool Interactable::check_collision(point_2d player_pos) const
{
    if (!is_active || is_triggered) return false;

    const float HALF_WIDTH = Player::HITBOX_WIDTH / 2.0f;
    const float HEIGHT     = Player::HITBOX_HEIGHT;

    float player_left   = player_pos.x - HALF_WIDTH;
    float player_right  = player_pos.x + HALF_WIDTH;
    float player_top    = player_pos.y - HEIGHT;
    float player_bottom = player_pos.y;

    float interact_right  = x + width;
    float interact_bottom = y + height;

    return !(player_left > interact_right ||
             player_right < x ||
             player_top > interact_bottom ||
             player_bottom < y);
}

void Interactable::trigger()
{
    if (!is_active || is_triggered) return;
    is_triggered = true;
    write_line("Interactable triggered: " + id + " (" + label + ")");
}

void Interactable::reset()
{
    is_triggered = false;
    is_active = true;
}
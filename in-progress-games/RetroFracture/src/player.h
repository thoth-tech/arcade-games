#ifndef PLAYER_H
#define PLAYER_H

#include "splashkit.h"
#include <map>
#include <string>

enum PlayerState {
    STATE_IDLE,
    STATE_RUN,
    STATE_JUMP,
    STATE_FALL,
    STATE_DASH          // new
};

struct Animation {
    bitmap anim_bitmap;
    int frame_count;
    int frame_width;
    int frame_height;
    int animation_speed;
    bool loops;
    std::string name;
};

class Player {
public:
    static constexpr float HITBOX_WIDTH  = 60.0f;
    static constexpr float HITBOX_HEIGHT = 90.0f;

    Player();

    void update();
    void draw();
    void draw_at(point_2d screen_position);
    void set_state(PlayerState new_state);
    PlayerState get_state() const { return current_state; }

    void move_left();
    void move_right();
    void jump();
    void stop_moving();
    void stop_vertical_movement();

    // --- new abilities ---
    void dash();
    void on_land();                        // call when player touches ground
    void enable_double_jump(bool enable);
    void enable_dash(bool enable);
    bool get_is_dashing() const { return is_dashing; }
    // --------------------

    point_2d get_position() const { return position; }
    void set_position(point_2d new_pos) { position = new_pos; }
    bool get_is_grounded() const { return is_grounded; }
    void set_grounded(bool grounded) { is_grounded = grounded; }
    bool get_facing_right() const { return facing_right; }

    rectangle get_bounding_box() const;

    int get_current_frame_index() const { return current_frame_index; }
    std::string get_current_animation_name() const {
        return current_animation ? current_animation->name : "None";
    }

private:
    point_2d position;
    float speed;
    float jump_force;
    float gravity;
    float vertical_velocity;
    bool is_grounded;
    bool facing_right;

    PlayerState current_state;
    int current_frame_index;
    int animation_timer;
    std::map<PlayerState, Animation> animations;
    Animation* current_animation;

    // --- double jump ---
    bool double_jump_enabled;
    bool double_jump_available;    // true when grounded, consumed by second jump

    // --- dash ---
    bool dash_enabled;
    bool is_dashing;
    int   dash_direction;          // 1 = right, -1 = left
    float dash_speed;
    int   dash_frames;
    int   dash_timer;
    int   dash_cooldown;
    int   dash_cooldown_timer;

    void setup_animations();
    void load_animation(const std::string& filename, PlayerState state,
                       int frame_count, int frame_width, int frame_height,
                       int speed, bool loops);
    void update_animation();
};

#endif
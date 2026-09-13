#include "player.h"

Player::Player()
    : position(point_at(0, 0)),
      speed(5.0f),
      jump_force(-12.0f),
      gravity(1.0f),
      vertical_velocity(0),
      is_grounded(true),
      facing_right(true),
      current_state(STATE_IDLE),
      current_frame_index(0),
      animation_timer(0),
      current_animation(nullptr),
      double_jump_enabled(false),
      double_jump_available(false),
      dash_enabled(false),
      is_dashing(false),
      dash_direction(1),
      dash_speed(15.0f),
      dash_frames(10),          // lasts ~0.17s at 60fps
      dash_timer(0),
      dash_cooldown(30),        // half second cooldown
      dash_cooldown_timer(0)
{
    setup_animations();
    set_state(STATE_IDLE);
}

rectangle Player::get_bounding_box() const
{
    float left = position.x - HITBOX_WIDTH / 2.0f;
    float top  = position.y - HITBOX_HEIGHT;
    return rectangle_from(left, top, HITBOX_WIDTH, HITBOX_HEIGHT);
}

void Player::setup_animations()
{
    load_animation("assets/sprites/striker/spr_StrikerIdle_strip.png",
                   STATE_IDLE, 8, 96, 96, 10, true);
    load_animation("assets/sprites/striker/spr_StrikerRun_strip.png",
                   STATE_RUN, 8, 96, 96, 6, true);
    load_animation("assets/sprites/striker/spr_StrikerJump_strip.png",
                   STATE_JUMP, 12, 96, 96, 8, false);
    load_animation("assets/sprites/striker/spr_StrikerJump_strip.png",
                   STATE_FALL, 12, 96, 96, 8, false);
    load_animation("assets/sprites/striker/spr_StrikerDash_strip.png",
                   STATE_DASH, 8, 96, 96, 6, false);
    write_line("Player animations loaded successfully");
}

void Player::load_animation(const std::string& filename, PlayerState state,
                            int frame_count, int frame_width, int frame_height,
                            int speed, bool loops)
{
    Animation anim;
    std::string bitmap_name = "anim_" + std::to_string(state);
    anim.anim_bitmap = load_bitmap(bitmap_name, filename);
    anim.frame_count = frame_count;
    anim.frame_width = frame_width;
    anim.frame_height = frame_height;
    anim.animation_speed = speed;
    anim.loops = loops;

    switch(state) {
        case STATE_IDLE: anim.name = "Idle"; break;
        case STATE_RUN:  anim.name = "Run"; break;
        case STATE_JUMP: anim.name = "Jump"; break;
        case STATE_FALL: anim.name = "Fall"; break;
        case STATE_DASH: anim.name = "Dash"; break;
        default: anim.name = "Unknown";
    }
    animations[state] = anim;
}

void Player::update()
{
    // --- gravity & vertical movement ---
    if (!is_grounded && !is_dashing) {   // no gravity during dash (optional)
        vertical_velocity += gravity;
        position.y += vertical_velocity;
        if (vertical_velocity > 0 && current_state == STATE_JUMP)
            set_state(STATE_FALL);
    }

    // --- dash logic ---
    if (is_dashing) {
        // move horizontally
        position.x += dash_direction * dash_speed;
        dash_timer--;
        if (dash_timer <= 0) {
            is_dashing = false;
            dash_cooldown_timer = dash_cooldown;
            // revert to appropriate state
            if (is_grounded)
                set_state(STATE_IDLE);
            else
                set_state(STATE_FALL);
        }
    } else {
        // cooldown tick
        if (dash_cooldown_timer > 0)
            dash_cooldown_timer--;
    }

    update_animation();
}

void Player::stop_vertical_movement()
{
    vertical_velocity = 0;
}

void Player::update_animation()
{
    if (!current_animation) return;
    animation_timer++;
    if (animation_timer >= current_animation->animation_speed) {
        animation_timer = 0;
        current_frame_index++;
        if (current_frame_index >= current_animation->frame_count) {
            if (current_animation->loops)
                current_frame_index = 0;
            else {
                current_frame_index = current_animation->frame_count - 1;
                if (current_state == STATE_JUMP && !is_grounded)
                    set_state(STATE_FALL);
            }
        }
    }
}

void Player::set_state(PlayerState new_state)
{
    if (current_state == new_state && current_animation != nullptr) return;
    current_state = new_state;
    if (animations.find(new_state) != animations.end()) {
        current_animation = &animations[new_state];
        current_frame_index = 0;
        animation_timer = 0;
        if (new_state == STATE_FALL)
            current_frame_index = 6;
    }
}

void Player::draw()
{
    draw_at(position);
}

void Player::draw_at(point_2d screen_position)
{
    if (!current_animation) return;

    int frame_x = current_frame_index * current_animation->frame_width;
    rectangle frame_rect = rectangle_from(frame_x, 0,
                                         current_animation->frame_width,
                                         current_animation->frame_height);
    float draw_x = screen_position.x - current_animation->frame_width / 2;
    float draw_y = screen_position.y - current_animation->frame_height;

    drawing_options opts = option_part_bmp(frame_rect);
    if (!facing_right)
        opts = option_scale_bmp(-1.0, 1.0, opts);
    draw_bitmap(current_animation->anim_bitmap, draw_x, draw_y, opts);
}

// ---------- movement ----------
void Player::move_left()
{
    position.x -= speed;
    facing_right = false;
    if (is_grounded && current_state != STATE_JUMP && current_state != STATE_FALL)
        set_state(STATE_RUN);
}

void Player::move_right()
{
    position.x += speed;
    facing_right = true;
    if (is_grounded && current_state != STATE_JUMP && current_state != STATE_FALL)
        set_state(STATE_RUN);
}

void Player::jump()
{
    if (is_grounded) {
        vertical_velocity = jump_force;
        is_grounded = false;
        double_jump_available = double_jump_enabled;   // reset double jump
        set_state(STATE_JUMP);
    }
    else if (double_jump_enabled && double_jump_available && !is_dashing) {
        // second jump (allowed in air, but not during dash to avoid exploits)
        vertical_velocity = jump_force;
        double_jump_available = false;
        set_state(STATE_JUMP);
    }
}

void Player::stop_moving()
{
    if (is_grounded && current_state == STATE_RUN)
        set_state(STATE_IDLE);
}

// ---------- new abilities ----------
void Player::dash()
{
    if (!dash_enabled || is_dashing || dash_cooldown_timer > 0)
        return;

    is_dashing = true;
    dash_timer = dash_frames;
    dash_direction = facing_right ? 1 : -1;
    set_state(STATE_DASH);
}

void Player::on_land()
{
    double_jump_available = double_jump_enabled;
    dash_cooldown_timer = 0;                // dash instantly recharges on landing
}

void Player::enable_double_jump(bool enable)
{
    double_jump_enabled = enable;
    if (!enable) double_jump_available = false;
}

void Player::enable_dash(bool enable)
{
    dash_enabled = enable;
    if (!enable) {
        is_dashing = false;
        dash_timer = 0;
        dash_cooldown_timer = 0;
    }
}
#include "splashkit.h"
#include "playerinput.h"
#include "block.h"
#include <memory>
#pragma once

// Player Physics Variables
//#define MAX_JUMP_HEIGHT 127
//#define JUMP_MOMENTUM_RATE 20
#define MAX_FALL_SPEED 9
#define FALL_RATE 0.8
#define MAX_RUN_SPEED 5
#define RUN_ACCEL 0.0981
#define JUMP_START_SPEED 9.81
#define JUMP_RISE_LOSS 0.1
#define FALL_SIDE_MOMENTUM 0.072
#define CLIMB_SPEED 3

class Player;
class PlayerState
{
protected:
    Player *player;
    string player_state;

public:
    virtual ~PlayerState(){
        // write_line("Deleting Player");
    };

    void set_state(Player *player, string player_state)
    {
        this->player = player;
        this->player_state = player_state;
    };

    string get_type()
    {
        return this->player_state;
    };

    virtual void update() = 0;
    virtual void get_input() = 0;
};

class Player
{
    private:
        PlayerState *state;
        sprite player_sprite;
        point_2d position;
        bool facing_left;
        bool on_floor;
        bool on_ladder;
        rectangle hitbox;
        bool is_dead = false;
        bool has_won = false;
        std::shared_ptr<Block> held_pipe;
        bool is_holding_pipe = false;
        int id;

    public:
        int max_jump_height = 127;
        int jump_momentum_rate = 8;
        player_input input;
        int player_lives = 3;
        int player_health = 3;

        Player(PlayerState *state, sprite player_sprite, point_2d initial_position, bool facing_left, player_input input) : state(nullptr)
        {
            this->change_state(state, "Initial");
            this->player_sprite = player_sprite;
            this->position = initial_position;
            this->facing_left = facing_left;
            this->on_floor = true;
            this->on_ladder = false;
            this->input = input;
            sprite_set_position(player_sprite, this->position);
            make_hitbox();
        };

        ~Player()
        {
            delete state;
        };

        void change_state(PlayerState *new_state, string type)
        {
            if (this->state != nullptr)
                delete this->state;
            this->state = new_state;
            this->state->set_state(this, type);
        };

        void update()
        {
            this->state->update();
        };

        void get_input()
        {
            this->state->get_input();
        };

        // Create the hitbox of the player_sprite.
        void make_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x;
            hitbox.y = this->position.y + 5;
            hitbox.height = sprite_height(this->player_sprite) - 5;
            hitbox.width = sprite_width(this->player_sprite);
            this->hitbox = hitbox;
        };

        // Update the current hitbox of the player sprite.
        void update_hitbox()
        {
            point_2d current_position = sprite_position(this->player_sprite);
            this->hitbox.x = current_position.x;
            this->hitbox.y = current_position.y + 5;
        };

        // Returns the player_sprite.
        sprite get_player_sprite()
        {
            return this->player_sprite;
        };

        // Returns the player_hitbox.
        rectangle get_player_hitbox()
        {
            return this->hitbox;
        };

        // Returns the state type.
        string get_player_state_type()
        {
            return this->state->get_type();
        }

        // Returns true or false if the player is facing left or not.
        bool is_facing_left()
        {
            return this->facing_left;
        };

        // Variable facing_left will be set to bool facing_left true or false.
        void set_facing_left(bool facing_left)
        {
            this->facing_left = facing_left;
        };

        // Returns true or false if the player is on the floor.
        bool is_on_floor()
        {
            return this->on_floor;
        };

        // Returns true or false if the player is on the ladder.
        bool is_on_ladder()
        {
            return this->on_ladder;
        };

        // Variable on_floor will be set to bool new_value true or false. 
        void set_on_floor(bool new_value)
        {
            this->on_floor = new_value;
        };

        // Variable on_ladder will be set to bool new_value true or false.
        void set_on_ladder(bool new_value)
        {
            this->on_ladder = new_value;
        };

        // Responsible for moving the player up or down depending on the float value.
        void set_player_dy(float value)
        {
            sprite_set_dy(this->player_sprite, value);
        };

        // Responsible for moving the player left or right depending on the float value.
        void set_player_dx(float value)
        {
            sprite_set_dx(this->player_sprite, value);
        };

        // Returns true or false if the player is dead or not.
        bool is_player_dead()
        {
            return this->is_dead;
        };

        // Variable is_dead will be set to bool is_dead either true or false.
        void set_dead(bool is_dead)
        {
            this->is_dead = is_dead;
        };

        // Returns true or false if the player has won or not.
        bool has_player_won()
        {
            return this->has_won;
        };

        // Variable is_dead will be set to bool is_dead either true or false.
        void set_player_won(bool status)
        {
            this->has_won = status;
        };

        // Returns the state type.
        string get_state_type()
        {
            return this->state->get_type();
        };

        // Returns the position of the player.
        point_2d get_player_position()
        {
            return this->position;
        };

        // Returns true or false if the player is holding a pipe or not.
        bool with_pipe()
        {
            return this->is_holding_pipe;
        };

        // Variable is_holding_pipe is set to bool new_value true or false.
        void set_with_pipe(bool new_value)
        {
            this->is_holding_pipe = new_value;
        };

        bool pick_pipe(std::shared_ptr<Block> pipe)
        {
            if (this->is_holding_pipe)
            {
                return false;
            }

            //write_line("Player Holding Holdable pipe with id: " + std::to_string(pipe->get_cell()));

            this->held_pipe = pipe;
            this->held_pipe->set_picked_up(true);
            set_with_pipe(true);
            return this->is_holding_pipe;
        };

        void place_pipe(std::shared_ptr<Block> empty)
        {
            empty = this->held_pipe;
            this->is_holding_pipe = false;
        };

        // Returns the id of the player.
        int get_player_id()
        {
            return this->id;
        };

        // Set the player id.
        void set_player_id(int id)
        {
            this->id = id;
        };

        std::shared_ptr<Block> get_held_pipe()
        {
            return this->held_pipe;
        };

        void set_player_sprite(sprite new_sprite)
        {
            this->player_sprite = new_sprite;
        };
};

// Idle State Class
class IdleState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        IdleState(){};

        ~IdleState(){};

        void update() override;
        void get_input() override;
};

// Run State Class
class RunState : public PlayerState
{
    private:
        bool run_once = false;
        float dx;

    public:
        RunState(float dx)
        {
            this->dx = dx;
        };

        ~RunState(){};

        void update() override;
        void get_input() override;
};

// Jumping State Class
class JumpRiseState : public PlayerState
{
    private:
        bool run_once = false;
        float initial_y;
        float max_jump_height;

    public:
        JumpRiseState(){};

        ~JumpRiseState(){};

        void update() override;
        void get_input() override;
};

// Falling State Class
class JumpFallState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        JumpFallState(){};

        ~JumpFallState(){};

        void update() override;
        void get_input() override;
};

// Dancing State Class
class DanceState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        DanceState(){};

        ~DanceState(){};

        void update() override;
        void get_input() override;
};

// Attacking State Class
class AttackState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        AttackState(){};

        ~AttackState(){};

        void update() override;
        void get_input() override;
};

// Hurt State Class
class HurtState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        HurtState(){};

        ~HurtState(){};

        void update() override;
        void get_input() override;
};

// Climbing State Class
class ClimbState : public PlayerState
{
    private:
        bool run_once = false;
        bool is_moving = false;

    public:
        ClimbState(){};

        ~ClimbState(){};

        void update() override;
        void get_input() override;
};

// Dying State Class
class DyingState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        DyingState(){};

        ~DyingState(){};

        void update() override;
        void get_input() override;
};

// Spawn State Class
class SpawningState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        SpawningState(){};

        ~SpawningState(){};

        void update() override;
        void get_input() override;
};

// Crouch State Class
class CrouchState : public PlayerState
{
private:
    bool run_once = false;

public:
    CrouchState(){};

    ~CrouchState(){};

    void update() override;
    void get_input() override;
};

void sprite_fall(sprite sprite)
{
    if (sprite_dy(sprite) < MAX_FALL_SPEED)
        sprite_set_dy(sprite, sprite_dy(sprite) + FALL_RATE);
}

void animation_routine(Player *player, string left_anim, string right_anim)
{
    if (player->is_facing_left())
        sprite_start_animation(player->get_player_sprite(), left_anim);
    else
        sprite_start_animation(player->get_player_sprite(), right_anim);
}

void sprite_update_routine_continuous(sprite player_sprite)
{
    draw_sprite(player_sprite);
    if (sprite_animation_has_ended(player_sprite))
        sprite_replay_animation(player_sprite);
    update_sprite(player_sprite);
}

void player_draw_pipe(Player *player)
{
    if(player->with_pipe())
    {
        point_2d position = center_point(player->get_player_sprite());
        bitmap pipe = player->get_held_pipe()->get_bitmap();
        drawing_options opts = option_defaults();
        opts.draw_cell = player->get_held_pipe()->get_cell();
        opts.scale_x = 0.7;
        opts.scale_y = 0.7;
        point_2d pipe_center = bitmap_cell_center(pipe);
        position.x = position.x - pipe_center.x;
        position.y = position.y - pipe_center.y;
        draw_bitmap(pipe, position.x, position.y, opts);
    }
}

void IdleState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        player->set_on_ladder(false);
        if (player->is_on_floor())
        {
            sprite_set_dx(player_sprite, 0);
            sprite_set_dy(player_sprite, 0);
        }
        animation_routine(player, "LeftIdle", "RightIdle");
        run_once = true;
    }

    if (player->is_on_floor())
    {
        sprite_set_dx(player_sprite, 0);
        sprite_set_dy(player->get_player_sprite(), 0);
    }
    else
        sprite_fall(player->get_player_sprite());

    player_draw_pipe(player);

    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void IdleState::get_input()
{
    if (key_down(player->input.left_key))
    {
        this->player->set_facing_left(true);
        this->player->change_state(new RunState(0), "RunLeft");
    }
    if (key_down(player->input.right_key))
    {
        this->player->set_facing_left(false);
        this->player->change_state(new RunState(0), "RunRight");
    }
    if ((key_typed(player->input.jump_key) || key_typed(player->input.jump_key2)) && player->is_on_floor())
    {
        this->player->change_state(new JumpRiseState, "JumpRise");
    }
    if (key_typed(Z_KEY))
    {
        this->player->change_state(new DanceState, "Dance");
    }
    if (key_typed(player->input.attack_key))
    {
        this->player->change_state(new AttackState, "Attack");
    }
    if (key_down(player->input.crouch_key))
    {
        this->player->change_state(new CrouchState, "Crouch");
    }
}

void RunState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        if (dx == 0)
        {
            if (player->is_facing_left())
                sprite_set_dx(player_sprite, -1);
            else
                sprite_set_dx(player_sprite, 1);
        }

        animation_routine(player, "LeftRun", "RightRun");
        run_once = true;
    }

    if (player->is_facing_left())
    {
        if (sprite_dx(player->get_player_sprite()) > -MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) - RUN_ACCEL);
    }
    else
    {
        if (sprite_dx(player->get_player_sprite()) < MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) + RUN_ACCEL);
    }

    if (player->is_on_floor())
    {
        sprite_set_dy(player->get_player_sprite(), 0);
    }
    else
        sprite_fall(player->get_player_sprite());

    player_draw_pipe(player);

    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void RunState::get_input()
{
    if (key_released(player->input.left_key) || key_released(player->input.right_key))
    {
        this->player->change_state(new IdleState, "Idle");
    }
    if ((key_typed(player->input.jump_key) || key_typed(player->input.jump_key2)) && player->is_on_floor())
    {
        this->player->change_state(new JumpRiseState, "JumpRise");
    }
}

void JumpRiseState::update()
{
    if (!run_once)
    {
        if (!sound_effect_playing("Jump"))
            play_sound_effect("Jump");
        initial_y = sprite_y(player->get_player_sprite());
        sprite_set_dy(player->get_player_sprite(), -JUMP_START_SPEED);
        animation_routine(player, "LeftJump", "RightJump");
        this->max_jump_height = this->player->max_jump_height + abs((this->player->jump_momentum_rate * sprite_dx(player->get_player_sprite())));
        run_once = true;
    }

    this->player->set_on_floor(false);

    // write_line(sprite_dy(player->get_player_sprite()));
    if (sprite_dy(player->get_player_sprite()) < 0)
        sprite_set_dy(player->get_player_sprite(), sprite_dy(player->get_player_sprite()) + JUMP_RISE_LOSS);

    player_draw_pipe(player);

    sprite_update_routine_continuous(this->player->get_player_sprite());

    float current_y = sprite_y(player->get_player_sprite());

    if ((initial_y - current_y) > max_jump_height)
    {
        sprite_set_dy(player->get_player_sprite(), 0);
        this->player->change_state(new JumpFallState, "JumpFall");
    }
}

void JumpRiseState::get_input()
{
    if (key_down(player->input.left_key))
    {
        if (sprite_dx(player->get_player_sprite()) > -MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) - RUN_ACCEL);
    }
    if (key_down(player->input.right_key))
    {
        if (sprite_dx(player->get_player_sprite()) < MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) + RUN_ACCEL);
    }
}

void JumpFallState::update()
{
    if (!run_once)
    {
        if (!sound_effect_playing("Jumpfall"))
            play_sound_effect("Jumpfall");
        sprite_set_dy(player->get_player_sprite(), 0);
        run_once = true;
    }

    player_draw_pipe(player);
    sprite_update_routine_continuous(this->player->get_player_sprite());

    sprite_fall(player->get_player_sprite());

    if (player->is_on_floor())
    {
        sprite_set_dy(player->get_player_sprite(), 0);
        if (player->is_facing_left() && key_down(player->input.left_key) && player->is_on_floor())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunLeft");
        else if (!player->is_facing_left() && key_down(player->input.right_key) && player->is_on_floor())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunRight");
        else
            this->player->change_state(new IdleState, "Idle");
    }
}

void JumpFallState::get_input()
{
    if (player->is_on_floor())
    {
        if (key_down(player->input.left_key) && player->is_facing_left())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunLeft");
        else if (key_down(player->input.right_key) && !player->is_facing_left())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunRight");
        else
            this->player->change_state(new IdleState, "Idle");
    }
    else
    {
        if (key_down(player->input.left_key))
        {
            if (sprite_dx(player->get_player_sprite()) > -MAX_RUN_SPEED)
                sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) - FALL_SIDE_MOMENTUM);
        }
        if (key_down(player->input.right_key))
        {
            if (sprite_dx(player->get_player_sprite()) < MAX_RUN_SPEED)
                sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) + FALL_SIDE_MOMENTUM);
        }
    }
}

void DanceState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        sprite_set_dx(player_sprite, 0);
        sprite_set_dy(player_sprite, 0);
        sprite_start_animation(player->get_player_sprite(), "Dance");
        run_once = true;
    }

    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void DanceState::get_input()
{
    if (key_typed(Z_KEY))
    {
        this->player->change_state(new IdleState, "Idle");
    }//this is a test please don't use this for actual actions!
}

void AttackState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        if (!sound_effect_playing("Attack"))
            play_sound_effect("Attack");
        sprite_set_dx(player_sprite, 0);
        if (player->is_on_floor())
            sprite_set_dy(player_sprite, 0);
        animation_routine(player, "LeftAttack", "RightAttack");
        run_once = true;
    }
    player_draw_pipe(player);
    draw_sprite(player_sprite);
    if (player->is_on_floor())
        sprite_set_dy(player->get_player_sprite(), 0);
    else
        sprite_fall(player->get_player_sprite());

    if (sprite_animation_has_ended(player_sprite))
        this->player->change_state(new IdleState, "Idle");
    update_sprite(player_sprite);
}

void AttackState::get_input()
{
}

void CrouchState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        sprite_set_dx(player_sprite, 0);
        sprite_set_dy(player_sprite, 0);
        animation_routine(player, "LeftCrouch", "RightCrouch");
        run_once = true;
    }
    player_draw_pipe(player);

    if (player->is_on_floor())
    {
        sprite_set_dx(player_sprite, 0);
        sprite_set_dy(player->get_player_sprite(), 0);
    }
    else
        sprite_fall(player->get_player_sprite());

    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void CrouchState::get_input()
{
     if (key_released(player->input.crouch_key))
    {
        this->player->change_state(new IdleState, "Idle");
    }
}

void HurtState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        if (!sound_effect_playing("Hurt"))
            play_sound_effect("Hurt");
        sprite_set_dx(player_sprite, 0);
        animation_routine(player, "LeftFall", "RightFall");
        run_once = true;
    }

    if(!this->player->is_on_floor())
        sprite_fall(player->get_player_sprite());
    else
        sprite_set_dy(player_sprite, 0);

    draw_sprite(player_sprite);
    if (sprite_animation_has_ended(player_sprite))
        this->player->change_state(new IdleState, "Idle");
    update_sprite(player_sprite);
}

void HurtState::get_input()
{
}

// ClimbState Update 
void ClimbState::update()
{
    if (!run_once)
    {
        is_moving = true;
        sprite_set_dx(player->get_player_sprite(), 0);
        sprite_set_dy(player->get_player_sprite(), 0);
        sprite_start_animation(this->player->get_player_sprite(), "Climb");
        run_once = true;
    }

    player_draw_pipe(player);
    sprite_update_routine_continuous(this->player->get_player_sprite());

    if (!player->is_on_ladder())
    {
        this->player->change_state(new IdleState, "Idle");
    }
}

// ClimbState Get Input Checks
void ClimbState::get_input()
{
    if (key_down(player->input.left_key))
    {
        if (!is_moving)
        {
            sprite_start_animation(this->player->get_player_sprite(), "Climb");
            is_moving = true;
        }
        sprite_set_dx(player->get_player_sprite(), -CLIMB_SPEED);
    }
    else if (key_down(player->input.right_key))
    {
        if (!is_moving)
        {
            sprite_start_animation(this->player->get_player_sprite(), "Climb");
            is_moving = true;
        }
        sprite_set_dx(player->get_player_sprite(), CLIMB_SPEED);
    }
    else if (key_down(player->input.jump_key) || key_down(player->input.jump_key2))
    {
        if (!is_moving)
        {
            sprite_start_animation(this->player->get_player_sprite(), "Climb");
            is_moving = true;
        }
        sprite_set_dy(player->get_player_sprite(), -CLIMB_SPEED);
    }
    else if (key_down(player->input.crouch_key))
    {
        if (!is_moving)
        {
            sprite_start_animation(this->player->get_player_sprite(), "Climb");
            is_moving = true;
        }
        sprite_set_dy(player->get_player_sprite(), CLIMB_SPEED);

        if(player->is_on_floor())
        {
            this->player->change_state(new IdleState, "Idle");
        }
    }
    if (key_released(player->input.jump_key) || key_released(player->input.jump_key2) || key_released(player->input.crouch_key))
    {
        is_moving = false;
        sprite_start_animation(this->player->get_player_sprite(), "ClimbIdle");
        sprite_set_dy(player->get_player_sprite(), 0);
    }
    if(key_released(player->input.left_key) || key_released(player->input.right_key))
    {
        is_moving = false;
        sprite_start_animation(this->player->get_player_sprite(), "ClimbIdle");
        sprite_set_dx(player->get_player_sprite(), 0);
    }
}

void DyingState::update()
{
    string dying_timer = "";
    if(this->player->get_player_id() == 1 || this->player->get_player_id() == 3)
        dying_timer = "DyingTimerP1";
    else
        dying_timer = "DyingTimerP2";
    
    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        if (!sound_effect_playing("Dead"))
            play_sound_effect("Dead");
        this->player->player_lives -= 1;
        start_timer(dying_timer);
        sprite_set_dx(player_sprite, 0);
        animation_routine(player, "LeftDying", "RightDying");
        run_once = true;
    }

    if(!this->player->is_on_floor())
        sprite_fall(player_sprite);
    else
        sprite_set_dy(player_sprite, 0);

    int time = timer_ticks(dying_timer) / 1000;

    if(time < 2)
         sprite_update_routine_continuous(player_sprite);
    else
    {
        stop_timer(dying_timer);
        this->player->change_state(new SpawningState, "Spawn");
    }
}

void DyingState::get_input()
{
}

void SpawningState::update()
{
    string spawn_timer = "";
    if(this->player->get_player_id() == 1 || this->player->get_player_id() == 3)
        spawn_timer = "SpawnTimerP1";
    else
        spawn_timer = "SpawnTimerP2";

    sprite player_sprite = this->player->get_player_sprite();
    if (!run_once)
    {
        sprite_set_position(player_sprite, this->player->get_player_position());
        this->player->player_health = 3;
        start_timer(spawn_timer);
        this->player->set_facing_left(false);
        sprite_set_dx(player_sprite, 0);
        sprite_set_dy(player_sprite, 0);
        animation_routine(player, "LeftSpawn", "RightSpawn");
        run_once = true;
    }

    if(!this->player->is_on_floor())
        sprite_fall(player_sprite);
    else
        sprite_set_dy(player_sprite, 0);

    int time = timer_ticks(spawn_timer) / 1000;

    if(time < 1)
         sprite_update_routine_continuous(player_sprite);
    else
    {
        stop_timer(spawn_timer);
        this->player->change_state(new IdleState, "Idle");
    }
        
}

void SpawningState::get_input()
{
}
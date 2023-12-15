#include "splashkit.h"
#include "player.h"
#include "machinehelper.h"
#include <memory>

class SnakeMachine;

class SnakeMachineState
{
    protected:
        SnakeMachine *snake;
        string snake_state;

    public:
        virtual ~SnakeMachineState()
        {};

        void set_state(SnakeMachine *snake, string snake_state)
        {
            this->snake = snake;
            this->snake_state = snake_state;
        };

        string get_type()
        {
            return this->snake_state;
        };

        virtual void update() = 0;
};

class SnakeMachine
{
    private:
        SnakeMachineState *state;
        sprite enemy_sprite;
        bool facing_left;
        vector<std::shared_ptr<Player>> level_players;
        

    public:
        SnakeMachine(SnakeMachineState *state, sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : state(nullptr)
        {
            this->enemy_sprite = enemy_sprite;
            this->level_players = level_players;
            sprite_start_animation(enemy_sprite, "LeftRun");
            this->change_state(state, "Idle");
        };

        ~SnakeMachine()
        {
            delete state;
        };

        void change_state(SnakeMachineState *new_state, string type)
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

        bool get_facing_left()
        {
            return facing_left;
        };

        void set_facing_left(bool new_value)
        {
            this->facing_left = new_value;
        };

        sprite get_sprite()
        {
            return this->enemy_sprite;
        };

        vector<std::shared_ptr<Player>> get_level_players()
        {
            return this->level_players;
        };

        string get_state_type()
        {
            return this->state->get_type();
        };
};

class SnakeIdle : public SnakeMachineState
{
    private:
        bool run_once = false;

    public:
        SnakeIdle(){};

        ~SnakeIdle(){};

        void update() override;
};

class SnakeCharge : public SnakeMachineState
{
    private:
        double charge_time = 0.0;
        bool run_once = false;

    public:
        SnakeCharge(){};

        ~SnakeCharge(){};

        void update() override;
};

void SnakeIdle::update()
{
    sprite snake_sprite = this->snake->get_sprite();
    vector<std::shared_ptr<Player>> players = this->snake->get_level_players();

    sprite_set_dx(snake_sprite, 0);

    if(this->snake->get_facing_left())
        set_proper_direction(snake_sprite, "LeftIdle");
    else
        set_proper_direction(snake_sprite, "RightIdle");

    for(int i = 0; i < players.size(); i++)
    {
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d snake_pos = to_screen(sprite_position(snake_sprite));

        double x_dist = snake_pos.x - player_pos.x; 
        double y_dist = snake_pos.y - player_pos.y;
        if(abs(x_dist) < 320 && abs(y_dist) < 17)
        {
            this->snake->change_state(new SnakeCharge, "Charge");
            break;
        }
    }
}

void SnakeCharge::update()
{
    sprite snake = this->snake->get_sprite();

    if(this->snake->get_facing_left())
    {
        set_proper_direction(snake, "LeftRun");
        sprite_set_dx(snake, 10);
    }
    else
    {
        set_proper_direction(snake, "RightRun");
        sprite_set_dx(snake, -10);
    }

    charge_time += 0.03;
    if(charge_time > 2)
        this->snake->change_state(new SnakeIdle, "Idle");
}
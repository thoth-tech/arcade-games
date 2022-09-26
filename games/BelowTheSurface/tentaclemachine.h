#include "splashkit.h"
#include "player.h"
#include "machinehelper.h"
#include <memory>
#include <vector>

class TentacleMachine;

class TentacleMachineState
{
    protected:
        TentacleMachine *tentacle;
        string tentacle_state;

    public:
        virtual ~TentacleMachineState()
        {};

        void set_state(TentacleMachine *tentacle, string tentacle_state)
        {
            this->tentacle = tentacle;
            this->tentacle_state = tentacle_state;
        };

        string get_type()
        {
            return this->tentacle_state;
        };

        virtual void update() = 0;
};

class TentacleMachine
{
    private:
        TentacleMachineState *state;
        sprite enemy_sprite;
        bool facing_left;
        vector<std::shared_ptr<Player>> level_players;
        

    public:
        TentacleMachine(TentacleMachineState *state, sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : state(nullptr)
        {
            this->enemy_sprite = enemy_sprite;
            this->level_players = level_players;
            sprite_start_animation(enemy_sprite, "Idle");
            this->change_state(state, "Idle");
        };

        ~TentacleMachine()
        {
            delete state;
        };

        void change_state(TentacleMachineState *new_state, string type)
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

class TentacleIdle : public TentacleMachineState
{
    private:
        bool run_once = false;

    public:
        TentacleIdle(){};

        ~TentacleIdle(){};

        void update() override;
};

class TentaclePopUp : public TentacleMachineState
{
    private:
        bool run_once = false;

    public:
        TentaclePopUp(){};

        ~TentaclePopUp(){};

        void update() override;
};

class TentaclePopDown : public TentacleMachineState
{
    private:
        bool run_once = false;

    public:
        TentaclePopDown(){};

        ~TentaclePopDown(){};

        void update() override;
};

class TentacleWiggle : public TentacleMachineState
{
    private:
        bool run_once = false;

    public:
        TentacleWiggle(){};

        ~TentacleWiggle(){};

        void update() override;
};

void TentacleIdle::update()
{
    sprite tentacle_sprite = this->tentacle->get_sprite();
    vector<std::shared_ptr<Player>> players = this->tentacle->get_level_players();

    set_proper_direction(tentacle_sprite, "Idle");
    
    for(int i = 0; i < players.size(); i++)
    {
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d tent_pos = to_screen(sprite_position(tentacle_sprite));

        double x_dist = tent_pos.x - player_pos.x; 
        //double y_dist = tent_pos.y - player_pos.y;
        if(abs(x_dist) < 200)
        {
            this->tentacle->change_state(new TentaclePopUp, "PopUp");
            break;
        }
    }
}

void TentaclePopUp::update()
{
    sprite tentacle_sprite = this->tentacle->get_sprite();

    if(sprite_animation_has_ended(tentacle_sprite))
        this->tentacle->change_state(new TentacleWiggle, "Wiggle");
    else
        set_proper_direction(tentacle_sprite, "PopUp");
}

void TentaclePopDown::update()
{
    sprite tentacle_sprite = this->tentacle->get_sprite();

    if(sprite_animation_has_ended(tentacle_sprite))
        this->tentacle->change_state(new TentacleIdle, "Idle");
    else
        set_proper_direction(tentacle_sprite, "PopDown");
}

void TentacleWiggle::update()
{
    sprite tentacle_sprite = this->tentacle->get_sprite();
    vector<std::shared_ptr<Player>> players = this->tentacle->get_level_players();

    set_proper_direction(tentacle_sprite, "Wiggle");

    for(int i = 0; i < players.size(); i++)
    {
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d tent_pos = to_screen(sprite_position(tentacle_sprite));

        double x_dist = tent_pos.x - player_pos.x; 
        if(abs(x_dist) > 200)
        {
            this->tentacle->change_state(new TentaclePopDown, "PopDown");
            break;
        }
    }
}
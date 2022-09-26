#include "splashkit.h"
#include "player.h"
#include "machinehelper.h"
#include <memory>
#include <vector>
#include <random>

class FlyMachine;

class FlyMachineState
{
    protected:
        FlyMachine *fly;
        string fly_state;

    public:
        virtual ~FlyMachineState()
        {};

        void set_state(FlyMachine *fly, string fly_state)
        {
            this->fly = fly;
            this->fly_state = fly_state;
        };

        string get_type()
        {
            return this->fly_state;
        };

        virtual void update() = 0;
};

class FlyMachine
{
    private:
        FlyMachineState *state;
        sprite enemy_sprite;
        bool facing_left;
        bool flying_up = true;
        
    public:
        FlyMachine(FlyMachineState *state, sprite enemy_sprite) : state(nullptr)
        {
            this->enemy_sprite = enemy_sprite;
            sprite_start_animation(enemy_sprite, "LeftFly");
            this->change_state(state, "Fly");
        };

        ~FlyMachine()
        {
            delete state;
        };

        void change_state(FlyMachineState *new_state, string type)
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

        string get_state_type()
        {
            return this->state->get_type();
        };

        void set_flying_up(bool new_value)
        {
            this->flying_up = new_value;
        };

        bool get_flying_up()
        {
            return this->flying_up;
        };
};

class FlySide : public FlyMachineState
{
    private:
        bool run_once = false;

    public:
        FlySide(){};

        ~FlySide(){};

        void update() override;
};

class FlyVertical : public FlyMachineState
{
    private:
        bool run_once = false;

    public:
        FlyVertical(){};

        ~FlyVertical(){};

        void update() override;
};

class FlyRandom : public FlyMachineState
{
    private:
        bool run_once = false;

    public:
        FlyRandom(){};

        ~FlyRandom(){};

        void update() override;
};

void FlySide::update()
{
    sprite fly_sprite = this->fly->get_sprite();

    if(this->fly->get_facing_left())
    {
        sprite_set_dx(fly_sprite, 4);
        set_proper_direction(fly_sprite, "LeftFly");
    }
    else
    {
        sprite_set_dx(fly_sprite, -4);
        set_proper_direction(fly_sprite, "RightFly");
    }
}

void FlyVertical::update()
{
    sprite fly_sprite = this->fly->get_sprite();

    if(this->fly->get_flying_up())
    {
        sprite_set_dy(fly_sprite, -4);
    }
    else
    {
        sprite_set_dy(fly_sprite, 4);
    }

    if(this->fly->get_facing_left())
        set_proper_direction(fly_sprite, "LeftFly");
    else
        set_proper_direction(fly_sprite, "RightFly");
}

void FlyRandom::update()
{
    sprite fly_sprite = this->fly->get_sprite();

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 7.0);
    double rand_vert = dist(mt);
    double rand_side = dist(mt);

    if(this->fly->get_flying_up())
    {
        sprite_set_dy(fly_sprite, -rand_vert);
    }
    else
    {
        sprite_set_dy(fly_sprite, rand_vert);
    }

    if(this->fly->get_facing_left())
    {
        sprite_set_dx(fly_sprite, rand_side);
        set_proper_direction(fly_sprite, "LeftFly");
    }
    else
    {
        sprite_set_dx(fly_sprite, -rand_side);
        set_proper_direction(fly_sprite, "RightFly");
    }
}


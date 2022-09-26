#include "splashkit.h"
#include "snakemachine.h"
#include "blobmachine.h"
#include "bossmachine.h"
#include "ratmachine.h"
#include "flymachine.h"
#include "tentaclemachine.h"
#include <random>

#pragma once

class Behaviour
{
    protected:
        sprite enemy_sprite;
        bool facing_left = true;
        bool flying_up = false;
        bool on_floor = true;
        bool is_flying = false;
        bool once = false;
        
    public:
        Behaviour(sprite enemy_sprite)
        {
            this->enemy_sprite = enemy_sprite;
        };
        ~Behaviour(){};
        virtual void update() = 0;

        void update_animation(string left_anim, string right_anim)
        {
            if(facing_left)
                sprite_start_animation(enemy_sprite, left_anim);
            else
                sprite_start_animation(enemy_sprite, right_anim);
        }

        void set_on_floor(bool new_value)
        {
            this->on_floor = new_value;
        };

        void set_facing_left(bool new_value)
        {
            this->once = false;
            this->facing_left = new_value;
        };

        bool is_on_floor()
        {
            return this->on_floor;
        };

        bool get_is_flying()
        {
            return this->is_flying;
        };

        bool get_flying_up()
        {
            return this->flying_up;
        };

        void set_flying_up(bool new_value)
        {
            this->flying_up = new_value;
        };

        void set_y_value(float val)
        {
            sprite_set_y(enemy_sprite, val + (64 - sprite_height(enemy_sprite)));
        };

        void fall_to_ground()
        {
            if(on_floor)
                sprite_set_dy(enemy_sprite, 0);
            else
                sprite_set_dy(enemy_sprite, 8);
        };

        virtual string get_state_type()
        {
            return "";
        };

        virtual void set_state(int state)
        {

        };
};

class RoachBehaviour : public Behaviour
{
    public:
        RoachBehaviour(sprite enemy_sprite) : Behaviour(enemy_sprite)
        {
            if(facing_left)
                sprite_start_animation(enemy_sprite, "LeftRun");
            else
                sprite_start_animation(enemy_sprite, "RightRun");
            
        };
        ~RoachBehaviour()
        {
        };
        void update() override
        {
            if(facing_left)
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, 3);
            }
            else
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, -3);
            }

            fall_to_ground();
        };
};

class BlobBehaviour : public Behaviour
{
    private:
        vector<std::shared_ptr<Player>> level_players;
        std::shared_ptr<BlobMachine> blob_machine;

    public:
        BlobBehaviour(sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : Behaviour(enemy_sprite)
        {
            this->level_players = level_players;
            std::shared_ptr<BlobMachine> machine(new BlobMachine(new BlobMove, enemy_sprite, level_players));
            this->blob_machine = machine;
        };
        
        ~BlobBehaviour()
        {
        };

        void update() override
        {
            this->blob_machine->set_facing_left(facing_left);
            fall_to_ground();
            this->blob_machine->update();
        };
};

class SnakeBehaviour : public Behaviour
{
    private:
        vector<std::shared_ptr<Player>> level_players;
        std::shared_ptr<SnakeMachine> snake_machine;

    public:
        SnakeBehaviour(sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : Behaviour(enemy_sprite)
        {
            this->level_players = level_players;
            this->on_floor = false;
            std::shared_ptr<SnakeMachine> machine(new SnakeMachine(new SnakeIdle, enemy_sprite, level_players));
            this->snake_machine = machine;
        };
        ~SnakeBehaviour()
        {
        };
        void update() override
        {
            if(this->snake_machine->get_state_type() == "Idle")
                face_player();
            
            this->snake_machine->set_facing_left(facing_left);
            fall_to_ground();
            this->snake_machine->update();
        };
        
        void face_player()
        {
            bool change_direction = false;
            for(int i = 0; i < level_players.size(); i++)
            {
                if(change_direction)
                    continue;
                    
                point_2d player_center = to_screen(center_point(level_players[i]->get_player_sprite()));
                point_2d enemy_center = to_screen(center_point(enemy_sprite));

                double dist = player_center.x - enemy_center.x;

                if(abs(dist) < 256)
                {
                    if(enemy_center.x < player_center.x)
                        facing_left = true;
                    else
                        facing_left = false;
                }
                else if(abs(dist) < 300)
                {
                    if(enemy_center.x < player_center.x)
                        facing_left = true;
                    else
                        facing_left = false;

                    change_direction = true;
                }
            }
        };
};

class RatBehaviour : public Behaviour
{
    private:
        vector<std::shared_ptr<Player>> level_players;
        std::shared_ptr<RatMachine> rat_machine;

    public:
        RatBehaviour(sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : Behaviour(enemy_sprite)
        {
            this->level_players = level_players;
            //this->on_floor = false;
            std::shared_ptr<RatMachine> machine(new RatMachine(new RatCrawl, enemy_sprite, level_players));
            this->rat_machine = machine;
        };
        ~RatBehaviour()
        {
        };
        void update() override
        {
            random_actions();
            this->rat_machine->set_facing_left(facing_left);
            fall_to_ground();
            this->rat_machine->update();
        };

        void random_actions()
        {
            if(this->rat_machine->get_state_type() == "Crawl")
            {
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_real_distribution<double> dist(0.0, 1000.0);
                double choice1 = dist(mt);
                double choice2 = dist(mt);

                if(choice1 > 990)
                {
                    this->rat_machine->change_state(new RatIdle, "Idle");
                }
                if(choice2 > 980)
                {
                    if(this->facing_left)
                        this->facing_left = false;
                    else
                        this->facing_left = true;
                }
            }
        }
};

class WaterRatBehaviour : public Behaviour
{
    private:
        std::shared_ptr<BossMachine> boss_machine;
        vector<std::shared_ptr<Player>> level_players;
        bool backwards_once = true;

    public:
        WaterRatBehaviour(sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : Behaviour(enemy_sprite)
        {
            this->level_players = level_players;
            std::shared_ptr<BossMachine> machine(new BossMachine(new BossIdle, enemy_sprite, level_players));
            this->boss_machine = machine;
        };
        ~WaterRatBehaviour()
        {
        };
        void update() override
        {
            backwards_behaviour();
            this->boss_machine->set_facing_left(facing_left);
            fall_to_ground();
            this->boss_machine->update();
        };

        void backwards_behaviour()
        {
            if(this->boss_machine->get_state_type() == "MoveBackwards")
            {
                if(backwards_once)
                {
                    if(this->facing_left)
                        this->facing_left = false;
                    else
                        this->facing_left = true;

                    backwards_once = false;
                }
            }
            else
            {
                face_player();
                backwards_once = true;
            }
        };

        void face_player()
        {
            point_2d player_center = to_screen(center_point(level_players[0]->get_player_sprite()));
            point_2d enemy_center = to_screen(center_point(enemy_sprite));

            if(enemy_center.x < player_center.x)
                facing_left = true;
            else
                facing_left = false;
        };

        string get_state_type()
        {
            return this->boss_machine->get_state_type();
        };

        void set_state(int state)
        {
            if(state == 0)
            {
                this->boss_machine->change_state(new BossDying, "Dying");
            }
        }
};

class FlyBehaviour : public Behaviour
{
    private:
        std::shared_ptr<FlyMachine> fly_machine;
        string type;

    public:
        FlyBehaviour(sprite enemy_sprite, string type) : Behaviour(enemy_sprite)
        {
            this->type = type;
            this->is_flying = true;

            if(type == "Green")
            {
                std::shared_ptr<FlyMachine> machine(new FlyMachine(new FlySide, enemy_sprite));
                this->fly_machine = machine;
            }
            if(type == "Red")
            {
                std::shared_ptr<FlyMachine> machine(new FlyMachine(new FlyVertical, enemy_sprite));
                this->fly_machine = machine;
            }
            if(type == "Purp")
            {
                std::shared_ptr<FlyMachine> machine(new FlyMachine(new FlyRandom, enemy_sprite));
                this->fly_machine = machine;
            }
        };

        ~FlyBehaviour()
        {
        };

        void update() override
        {
            this->fly_machine->set_facing_left(facing_left);
            this->fly_machine->set_flying_up(flying_up);
            this->fly_machine->update();

            if(type == "Purp")
                face_random_direction();
        };

        void face_random_direction()
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0.0, 100.0);
            double choice1 = dist(mt);
            double choice2 = dist(mt);

            if(choice1 > 98)
            {
                if(facing_left)
                    facing_left = false;
                else
                    facing_left = true;
            }
            if(choice2 > 98)
            {
                if(flying_up)
                    flying_up = false;
                else
                    flying_up = true;
            }
        }
};

class TentacleBehaviour : public Behaviour
{
    private:
        std::shared_ptr<TentacleMachine> tentacle_machine;
        vector<std::shared_ptr<Player>> level_players;

    public:
        TentacleBehaviour(sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : Behaviour(enemy_sprite)
        {
            this->level_players = level_players;
            std::shared_ptr<TentacleMachine> machine(new TentacleMachine(new TentacleIdle, enemy_sprite, level_players));
            this->tentacle_machine = machine;
        };
        ~TentacleBehaviour()
        {
        };
        void update() override
        {
            this->tentacle_machine->set_facing_left(facing_left);
            fall_to_ground();
            this->tentacle_machine->update();
        };

};
#include "splashkit.h"
#include "behaviour.h"
#include <memory>

#pragma once

class Enemy
{
    protected:
        sprite enemy_sprite;
        point_2d position;
        rectangle hitbox;
        bool is_dead;
        int hp;
        bool is_vulnerable = true;
        bool is_boss = false;
        std::shared_ptr<Behaviour> ai;
        vector<std::shared_ptr<Player>> level_players;

    public:
        Enemy(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players)
        {
            this->is_dead = false;
            this->hp = 0;
            this->enemy_sprite = enemy_sprite;
            this->position = position;
            this->level_players = level_players;
            sprite_set_position(enemy_sprite, this->position);
            make_hitbox();
        };

        ~Enemy(){};

        virtual void update()
        {
            if(!is_dead)
            {
                if(rect_on_screen(hitbox))
                {
                    ai->update();
                    draw_sprite(enemy_sprite);
                    if(sprite_animation_has_ended(enemy_sprite))
                        sprite_replay_animation(enemy_sprite);
                    update_sprite(enemy_sprite);
                    update_hitbox();
                }
            }
        };

        virtual void make_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x;
            hitbox.y = this->position.y + 10;
            hitbox.height = sprite_height(this->enemy_sprite) - 10;
            hitbox.width = sprite_width(this->enemy_sprite);
            this->hitbox = hitbox;
        };

        virtual void update_hitbox()
        {
            point_2d current_position = sprite_position(this->enemy_sprite);
            this->hitbox.x = current_position.x;
            this->hitbox.y = current_position.y + 10;
        };

        rectangle get_enemy_hitbox()
        {
            return this->hitbox;
        };

        std::shared_ptr<Behaviour> get_ai()
        {
            return this->ai;
        };

        sprite get_enemy_sprite()
        {
            return this->enemy_sprite;
        };

        void set_dead(bool new_value)
        {
            this->is_dead = new_value;
        };

        bool get_dead()
        {
            return this->is_dead;
        };

        void set_vulnerable(bool new_value)
        {
            this->is_vulnerable = new_value;
        };

        bool get_vulnerable()
        {
            return this->is_vulnerable;
        };

        bool get_is_boss()
        {
            return this->is_boss;
        };

        // Returns current enemy hp.
        int get_hp()
        {
            return this->hp;
        };

        // Decrements enemy hp. 
        void take_damage(int decrement)
        {
            this->hp -= decrement;
        };

        string get_state_type()
        {
            return this->ai->get_state_type();
        };

        string test_collision(rectangle one)
        {
            string collision = "None";
            double dx = (one.x + one.width/2) - (this->hitbox.x + this->hitbox.width/2);
            double dy = (one.y + one.height/2) - (this->hitbox.y + this->hitbox.height/2);
            double width = (one.width + this->hitbox.width)/2;
            double height = (one.height + this->hitbox.height)/2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if(abs(dx) <= width && abs(dy) <= height)
            {
                if(crossWidth>=crossHeight)
                {
                    if(crossWidth > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    //Bias so player has a better chance of hitting them while above
                    if(crossWidth - 200 > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };
};

class Roach : public Enemy
{
    public:
        Roach(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players) : Enemy (enemy_sprite, position, level_players)
        {
            std::shared_ptr<Behaviour> ai(new RoachBehaviour(enemy_sprite));
            this->ai = ai;
            point_2d pos = this->position;
            pos.y = pos.y + 32;
            sprite_set_position(enemy_sprite, pos);
            
            make_hitbox();
        };

        ~Roach(){};
};

class Blob : public Enemy
{
    public:
        Blob(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players) : Enemy (enemy_sprite, position, level_players)
        {
            std::shared_ptr<Behaviour> ai(new BlobBehaviour(enemy_sprite, level_players));
            this->ai = ai;
            this->hp = 3; // Blob has 3 hit points. If blob gets jumped on 3 times. It dies.
            point_2d pos = this->position;
            sprite_set_position(enemy_sprite, pos);

            make_hitbox();
        };

        ~Blob(){};
};

class Snake : public Enemy
{
    public:
        Snake(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players) : Enemy (enemy_sprite, position, level_players)
        {
            std::shared_ptr<Behaviour> ai(new SnakeBehaviour(enemy_sprite, level_players));
            this->ai = ai;
            point_2d pos = this->position;
            //pos.y = pos.y + 32;
            sprite_set_position(enemy_sprite, pos);

            make_hitbox();
        };

        ~Snake(){};
};

class Rat : public Enemy
{
    public:
        Rat(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players) : Enemy (enemy_sprite, position, level_players)
        {
            std::shared_ptr<Behaviour> ai(new RatBehaviour(enemy_sprite, level_players));
            this->ai = ai;
            point_2d pos = this->position;
            pos.y = pos.y + 32;
            sprite_set_position(enemy_sprite, pos);

            make_hitbox();
        };

        ~Rat(){};
};

class WaterRat : public Enemy
{
    private:
        bool is_dying = false;

    public:
        WaterRat(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players) : Enemy (enemy_sprite, position, level_players)
        {
            std::shared_ptr<Behaviour> ai(new WaterRatBehaviour(enemy_sprite, level_players));
            this->ai = ai;
            this->hp = 5;
            this->is_boss = true;
            this->is_vulnerable = false;
            point_2d pos = this->position;
            sprite_set_position(enemy_sprite, pos);

            make_hitbox();
        };

        ~WaterRat(){};

        void update_hitbox()
        {
            point_2d current_position = sprite_position(this->enemy_sprite);
            this->hitbox.x = current_position.x;
            this->hitbox.y = current_position.y;

            if(this->ai->get_state_type() == "Move" || this->ai->get_state_type() == "MoveBackwards" || this->ai->get_state_type() == "Idle" || this->ai->get_state_type() == "Dying")
            {
                this->is_vulnerable = true;
                this->hitbox.height = sprite_height(enemy_sprite)/2 + 3;
                this->hitbox.y += sprite_height(enemy_sprite)/2 - 3;
            }
            else
            {
                this->is_vulnerable = false;
                this->hitbox.height = sprite_height(this->enemy_sprite);
            }
            check_health();
        };

        void update()
        {
            ai->update();
            draw_sprite(enemy_sprite);
            if(sprite_animation_has_ended(enemy_sprite))
                sprite_replay_animation(enemy_sprite);
            update_sprite(enemy_sprite);
            update_hitbox();
        };

        void check_health()
        {
            if(hp < 1)
            {
                is_dying = true;
                hp = 99;
            }

            if(is_dying)
            {
                this->ai->set_state(0);
                is_dying = false;
                is_dead = true;
            }
        };
};

class Fly : public Enemy
{
    public:
        Fly(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players, string type) : Enemy (enemy_sprite, position, level_players)
        {
            std::shared_ptr<Behaviour> ai(new FlyBehaviour(enemy_sprite, type));
            this->ai = ai;
            point_2d pos = this->position;
            sprite_set_position(enemy_sprite, pos);

            make_hitbox();
        };

        ~Fly(){};

        void make_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x;
            hitbox.y = this->position.y;
            hitbox.height = sprite_height(this->enemy_sprite);
            hitbox.width = sprite_width(this->enemy_sprite);
            this->hitbox = hitbox;
        };

        void update_hitbox()
        {
            point_2d current_position = sprite_position(this->enemy_sprite);
            this->hitbox.x = current_position.x;
            this->hitbox.y = current_position.y;
        };
};

class Tentacle : public Enemy
{
    public:
        Tentacle(sprite enemy_sprite, point_2d position, vector<std::shared_ptr<Player>> level_players) : Enemy (enemy_sprite, position, level_players)
        {
            std::shared_ptr<Behaviour> ai(new TentacleBehaviour(enemy_sprite, level_players));
            this->ai = ai;
            point_2d pos = this->position;
            sprite_set_position(enemy_sprite, pos);

            make_hitbox();
        };

        ~Tentacle(){};
};
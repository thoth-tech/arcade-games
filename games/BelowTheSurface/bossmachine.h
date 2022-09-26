#include "splashkit.h"
#include "player.h"
#include "machinehelper.h"
#include <memory>
#include <vector>
#include <random>

class BossMachine;

class BossMachineState
{
    protected:
        BossMachine *boss;
        string boss_state;

    public:
        virtual ~BossMachineState()
        {};

        void set_state(BossMachine *boss, string boss_state)
        {
            this->boss = boss;
            this->boss_state = boss_state;
        };

        string get_type()
        {
            return this->boss_state;
        };

        virtual void update() = 0;
};

class BossMachine
{
    private:
        BossMachineState *state;
        sprite enemy_sprite;
        bool facing_left;
        vector<std::shared_ptr<Player>> level_players;
        
    public:
        BossMachine(BossMachineState *state, sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : state(nullptr)
        {
            this->enemy_sprite = enemy_sprite;
            this->level_players = level_players;
            sprite_start_animation(enemy_sprite, "LeftRun");
            this->change_state(state, "Idle");
        };

        ~BossMachine()
        {
            delete state;
        };

        void change_state(BossMachineState *new_state, string type)
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

class BossIdle : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossIdle(){};

        ~BossIdle(){};

        void update() override;
};

class BossMoveBackwards : public BossMachineState
{
    private:
        bool run_once = false;
        double timer = 0;
        double timer_length = 0;

    public:
        BossMoveBackwards(){};

        ~BossMoveBackwards(){};

        void update() override;
};

class BossMove : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossMove(){};

        ~BossMove(){};

        void update() override;
};

class BossRise : public BossMachineState
{
    private:
        bool run_once = false;
        int rise_type;

    public:
        BossRise(int rise_type)
        {
            this->rise_type = rise_type;
        };

        ~BossRise(){};

        void update() override;
};

class BossSpotPlayer : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossSpotPlayer(){};

        ~BossSpotPlayer(){};

        void update() override;
};

class BossBattleCry : public BossMachineState
{
    private:
        bool run_once = false;
        int battle_cry_type;

    public:
        BossBattleCry(int battle_cry_type)
        {
            this->battle_cry_type = battle_cry_type;
        };

        ~BossBattleCry(){};

        void update() override;
};

class BossAttack : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossAttack(){};

        ~BossAttack(){};

        void update() override;
};

class BossDescend : public BossMachineState
{
    private:
        bool run_once = false;
        int lower_type;

    public:
        BossDescend(int lower_type)
        {
            this->lower_type = lower_type;
        };

        ~BossDescend(){};

        void update() override;
};

class BossDying : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossDying(){};

        ~BossDying(){};

        void update() override;
};

void BossIdle::update()
{
    sprite boss_sprite = this->boss->get_sprite();
    vector<std::shared_ptr<Player>> players = this->boss->get_level_players();

    if(this->boss->get_facing_left())
        set_proper_direction(boss_sprite, "LeftIdle");
    else
        set_proper_direction(boss_sprite, "RightIdle");

    for(int i = 0; i < players.size(); i++)
    {
        if (!sound_effect_playing("BossIdle"))
            play_sound_effect("BossIdle");
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d boss_pos = to_screen(sprite_position(boss_sprite));

        double x_dist = boss_pos.x - player_pos.x; 
        //double y_dist = boss_pos.y - player_pos.y;
        if(abs(x_dist) < 300)
        {
            this->boss->change_state(new BossRise(0), "Rise");
            break;
        }
    }
}

void BossMove::update()
{
    sprite boss_sprite = this->boss->get_sprite();
    vector<std::shared_ptr<Player>> players = this->boss->get_level_players();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 4);
        set_proper_direction(boss_sprite, "LeftRun");
    }
    else
    {
        sprite_set_dx(boss_sprite, -4);
        set_proper_direction(boss_sprite, "RightRun");
    }

    for(int i = 0; i < players.size(); i++)
    {
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d boss_pos = to_screen(sprite_position(boss_sprite));

        double x_dist = boss_pos.x - player_pos.x; 
        //double y_dist = boss_pos.y - player_pos.y;
        if(abs(x_dist) < 50)
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0.0, 100.0);

            double choice = dist(mt);
            if(choice < 80)
            {
                sprite_set_dx(boss_sprite, 0);
                this->boss->change_state(new BossRise(1), "RiseAttack");
                break;
            }
            else
            {
                this->boss->change_state(new BossMoveBackwards, "MoveBackwards");
            }
        }
    }
}

void BossMoveBackwards::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(!run_once)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0.3, 1.3);
        run_once = true;

        timer_length = dist(mt);
    }

    if(this->boss->get_facing_left())
        this->boss->set_facing_left(false);
    else
        this->boss->set_facing_left(true);

    if(!this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 1.3);
        set_proper_direction(boss_sprite, "RightRun");
    }
    else
    {
        sprite_set_dx(boss_sprite, -1.3);
        set_proper_direction(boss_sprite, "LeftRun");
    }

    this->timer += 0.01;

    if(timer > timer_length)
    {
        sprite_set_dx(boss_sprite, 0);
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0.0, 100.0);

        double choice = dist(mt);

        if(choice < 30)
        {
            this->boss->change_state(new BossRise(0), "Rise");
        }
        else if(choice > 33 && choice < 66)
        {
            this->boss->change_state(new BossSpotPlayer, "Spot");
        }
        else
        {
            this->boss->change_state(new BossMove, "Move");
        }
    }
}

void BossRise::update()
{
    if (!sound_effect_playing("BossRise"))
        play_sound_effect("BossRise");
    sprite boss_sprite = this->boss->get_sprite();
    sprite_set_dx(boss_sprite, 0);

    if(this->boss->get_facing_left())
        set_proper_direction(boss_sprite, "LeftRise");
    else
        set_proper_direction(boss_sprite, "RightRise");

    if(sprite_animation_has_ended(boss_sprite))
    {
        if(rise_type == 0)
            this->boss->change_state(new BossBattleCry(3), "BattleCry");
        if(rise_type == 1)
            this->boss->change_state(new BossAttack, "Attack");
    }
}

void BossSpotPlayer::update()
{
    if (!sound_effect_playing("BossSpot"))
        play_sound_effect("BossSpot");
    sprite boss_sprite = this->boss->get_sprite();
    vector<std::shared_ptr<Player>> players = this->boss->get_level_players();

    if(this->boss->get_facing_left())
        set_proper_direction(boss_sprite, "LeftSpotPlayer");
    else
        set_proper_direction(boss_sprite, "RightSpotPlayer");

    if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossDescend(0), "Descend");
}

void BossBattleCry::update()
{
    if (!sound_effect_playing("BossCry"))
        play_sound_effect("BossCry");
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
        set_proper_direction(boss_sprite, "LeftBattleCry");
    else
        set_proper_direction(boss_sprite, "RightBattleCry");

    if(sprite_animation_has_ended(boss_sprite))
    {
        if(battle_cry_type == 0)
            this->boss->change_state(new BossSpotPlayer, "SpotPlayer");
        if(battle_cry_type == 1)
            this->boss->change_state(new BossAttack, "Attack");
        if(battle_cry_type == 2)
            this->boss->change_state(new BossDescend(1), "Descend");
        if(battle_cry_type == 3)
            this->boss->change_state(new BossDescend(0), "Descend");
    }
}

void BossAttack::update()
{
    if(!run_once)
    {
        if (!sound_effect_playing("BossAttack"))
            play_sound_effect("BossAttack");
        run_once = true;
    }
    
    sprite boss_sprite = this->boss->get_sprite();
    vector<std::shared_ptr<Player>> players = this->boss->get_level_players(); 

    if(this->boss->get_facing_left())
        set_proper_direction(boss_sprite, "LeftAttack");
    else
        set_proper_direction(boss_sprite, "RightAttack");

    if(sprite_animation_has_ended(boss_sprite))
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0.0, 100.0);

        double choice = dist(mt);
        if(choice < 30)
        {
            this->boss->change_state(new BossDescend(1), "Descend");
        }
        else if(choice > 30 && choice < 50)
        {
            this->boss->change_state(new BossDescend(0), "Descend");
        }
        else if(choice > 50 && choice < 90)
        {
            this->boss->change_state(new BossAttack, "Attack");
        }
        else
        {
            this->boss->change_state(new BossBattleCry(2), "BattleCry");
        }
    }
}

void BossDescend::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
        set_proper_direction(boss_sprite, "LeftDescend");
    else
        set_proper_direction(boss_sprite, "RightDescend");

    if(sprite_animation_has_ended(boss_sprite))
    {
        if(lower_type == 0)
            this->boss->change_state(new BossMove, "Move");
        if(lower_type == 1)
            this->boss->change_state(new BossMoveBackwards, "MoveBackwards");
    }
        
}

void BossDying::update()
{
    if(!run_once)
    {
        if (!sound_effect_playing("BossDeath"))
            play_sound_effect("BossDeath");
        run_once = true;
    }
    
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "LeftDying");
    }
    else
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "RightDying");
    }
}
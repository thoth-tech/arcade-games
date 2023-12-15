#include "splashkit.h"
#include "player.h"
#include "machinehelper.h"
#include <memory>
#include <vector>

class BlobMachine;

class BlobMachineState
{
    protected:
        BlobMachine *blob;
        string blob_state;

    public:
        virtual ~BlobMachineState()
        {};

        void set_state(BlobMachine *blob, string blob_state)
        {
            this->blob = blob;
            this->blob_state = blob_state;
        };

        string get_type()
        {
            return this->blob_state;
        };

        virtual void update() = 0;
};

class BlobMachine
{
    private:
        BlobMachineState *state;
        sprite enemy_sprite;
        bool facing_left;
        vector<std::shared_ptr<Player>> level_players;
        

    public:
        BlobMachine(BlobMachineState *state, sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : state(nullptr)
        {
            this->enemy_sprite = enemy_sprite;
            this->level_players = level_players;
            sprite_start_animation(enemy_sprite, "LeftRun");
            this->change_state(state, "Idle");
        };

        ~BlobMachine()
        {
            delete state;
        };

        void change_state(BlobMachineState *new_state, string type)
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

class BlobMove : public BlobMachineState
{
    private:
        bool run_once = false;

    public:
        BlobMove(){};

        ~BlobMove(){};

        void update() override;
};

void BlobMove::update()
{
    sprite blob_sprite = this->blob->get_sprite();

    if(this->blob->get_facing_left())
    {
        sprite_set_dx(blob_sprite, 3);
        set_proper_direction(blob_sprite, "LeftRun");
    }
    else
    {
        sprite_set_dx(blob_sprite, -3);
        set_proper_direction(blob_sprite, "RightRun");
    }
}
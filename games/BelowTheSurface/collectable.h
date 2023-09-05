#include "splashkit.h"
#include <memory>

class Collectable
{
    protected:
        bitmap image;
        bool collected = false;
        point_2d position;
        rectangle hitbox;
        int cell;
        drawing_options opts;

    public:
        Collectable(bitmap cell_sheet, point_2d position)
        {
            this->image = cell_sheet;
            this->position = position;
            this->opts = option_defaults();
        };

        ~Collectable(){};

        void draw()
        {
            if(!collected)
            {
                draw_bitmap(image, position.x, position.y, opts);
                //draw_rectangle(COLOR_GREEN, this->hitbox);
            }
        };

        virtual void make_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x;
            hitbox.y = this->position.y;
            hitbox.height = bitmap_cell_height(this->image);
            hitbox.width = bitmap_cell_width(this->image);
            this->hitbox = hitbox;
        };

        virtual string collision(rectangle one)
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };

        virtual void effect(std::shared_ptr<Player> player) = 0;

        bool get_collected()
        {
            return this->collected;
        };

        void set_collected(bool new_value)
        {
            this->collected = new_value;
        };

        rectangle get_hitbox()
        {
            return this->hitbox;
        };
};

class HeartCollectable : public Collectable
{
    public:
        HeartCollectable(bitmap cell_sheet, point_2d position, int cell) : Collectable(cell_sheet, position)
        {
            this->cell = cell;
            this->opts.draw_cell = this->cell;
            make_hitbox();
        };

        ~HeartCollectable(){};

        void effect(std::shared_ptr<Player> player)
        {
            //write_line("Heart Effect");
            if(player->player_health < 3)
                player->player_health += 1;
            else
                collected = false;
        };

        void make_hitbox() override
        {
            rectangle hitbox;
            hitbox.x = this->position.x + 24;
            hitbox.y = this->position.y + 50;
            hitbox.height = 14;
            hitbox.width = 16;
            this->hitbox = hitbox;
        };
};
#include "splashkit.h"
#include "types.h"

#pragma once

class Block
{
    protected:
        bitmap image;
        point_2d position;
        drawing_options opts;
        double top;
        string type;
        rectangle hitbox;
        rectangle special_hitbox;
        bool is_turnable = false;
        bool is_picked_up = false;
        bool is_flowing = false;
        bool is_stopped = false;
        int cell;

    public:
        Block(bitmap cell_sheet, point_2d position)
        {
            this->image = cell_sheet;
            this->position = position;
            this->opts = option_defaults();
            this->top = position.y - bitmap_cell_height(image);
            make_hitbox();
        };

        ~Block(){
            // write_line("Deleting Block");
        };

        virtual void draw_block()
        {
            draw_bitmap(image, position.x, position.y, opts);
            //draw_rectangle(COLOR_GREEN,hitbox);
        };

        float get_top()
        {
            return this->top;
        };

        point_2d get_pos()
        {
            return this->position;
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

        virtual string test_collision(rectangle one) = 0;

        virtual string special_collision(rectangle one){return "";};

        rectangle get_block_hitbox()
        {
            return this->hitbox;
        };

        bool get_is_flowing()
        {
            return this->is_flowing;
        };

        int get_cell()
        {
            return this->cell;
        };

        void set_picked_up(bool new_value)
        {
            this->is_picked_up = new_value;
        };

        bool picked_up()
        {
            return this->is_picked_up;
        };

        void set_flowing(bool new_value)
        {
            this->is_flowing = new_value;
        };

        void set_stopped(bool new_value)
        {
            this->is_stopped = new_value;
        };

        void set_turnable(bool new_value)
        {
            this->is_turnable = new_value;
        };

        bool get_turnable()
        {
            return this->is_turnable;
        };

        bool get_is_stopped()
        {
            return this->is_stopped;
        };

        void change_cell_sheet(bitmap image)
        {
            this->image = image;
        };

        bitmap get_bitmap()
        {
            return this->image;
        };

        rectangle get_special_hitbox()
        {
            return this->special_hitbox;
        };
};

class SolidBlock : public Block
{
    public:
        SolidBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        };

        string test_collision(rectangle one) override
        {
            string collision = "None";
            double dx = (one.x + one.width / 2) - (this->hitbox.x + this->hitbox.width / 2);
            double dy = (one.y + one.height / 2) - (this->hitbox.y + this->hitbox.height / 2);
            double width = (one.width + this->hitbox.width) / 2;
            double height = (one.height + this->hitbox.height) / 2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if (abs(dx) <= width && abs(dy) <= height)
            {
                if (crossWidth >= crossHeight)
                {
                    if (crossWidth + 100 > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    // Gave a bias to top collision to avoid right edge stopping player during movement
                    if (crossWidth - 200 > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };
};

class HalfSolidBlockTop : public Block
{
    public:
        HalfSolidBlockTop(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->top = position.y - 64;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        };

        string test_collision(rectangle one) override
        {
            string collision = "None";
            double dx = (one.x + one.width / 2) - (this->hitbox.x + this->hitbox.width / 2);
            double dy = (one.y + one.height / 2) - (this->hitbox.y + this->hitbox.height / 2);
            double width = (one.width + this->hitbox.width) / 2;
            double height = (one.height + this->hitbox.height) / 2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if (abs(dx) <= width && abs(dy) <= height)
            {
                if (crossWidth >= crossHeight)
                {
                    if (crossWidth + 100 > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    // Gave a bias to top collision to avoid right edge stopping player during movement
                    if (crossWidth - 200 > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };
};

class HalfSolidBlockBottom : public Block
{
    public:
        HalfSolidBlockBottom(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->top = position.y - 32;
            position.y += 32;
            this->position = position;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
            make_hitbox();
        };

        string test_collision(rectangle one) override
        {
            string collision = "None";
            double dx = (one.x + one.width / 2) - (this->hitbox.x + this->hitbox.width / 2);
            double dy = (one.y + one.height / 2) - (this->hitbox.y + this->hitbox.height / 2);
            double width = (one.width + this->hitbox.width) / 2;
            double height = (one.height + this->hitbox.height) / 2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if (abs(dx) <= width && abs(dy) <= height)
            {
                if (crossWidth >= crossHeight)
                {
                    if (crossWidth > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    // Gave a bias to top collision to avoid right edge stopping player during movement
                    if (crossWidth > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };
};

class Ladder : public Block
{
    public:
        Ladder(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        };

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class PipeBlock : public Block
{
    public:
        PipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override { return "None"; };
};

class DecorativeBlock : public Block
{
    public:
        DecorativeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override { return "None"; };
};

class WaterBlock : public Block
{
    private:
        animation anim;
        int time = 0;

    public:
        WaterBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            //this->is_water = true;
            this->is_flowing = true;
            this->position = position;
            this->cell = cell;

            animation_script water_script = animation_script_named("CellAnim");
            animation anim;
            switch(this->cell)
            {
                case 0: {}
                case 1:
                {
                    anim = create_animation(water_script, "WaterFlow");
                    break;
                }
                case 2:
                case 3:
                {
                    anim = create_animation(water_script, "WaterTop");
                        break;
                }
                case 4: {}
                case 5: 
                {
                    anim = create_animation(water_script, "WaterTwoSide");
                        break;
                }
                case 6: {}
                case 7: 
                {
                    anim = create_animation(water_script, "WaterLeft");
                        break;
                }
                case 8: {}
                case 9: 
                {
                    anim = create_animation(water_script, "WaterRight");
                        break;
                }
                case 10: {}
                case 11: 
                {
                    anim = create_animation(water_script, "WaterLake");
                        break;
                }
                default:
                {
                    anim = create_animation(water_script, "WaterFlow");
                    break;
                }
            }
                
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
        }

        string test_collision(rectangle one) override
        {
            string collision = "None";
            double dx = (one.x + one.width / 2) - (this->hitbox.x + this->hitbox.width / 2);
            double dy = (one.y + one.height / 2) - (this->hitbox.y + this->hitbox.height / 2);
            double width = (one.width + this->hitbox.width) / 2;
            double height = (one.height + this->hitbox.height) / 2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if (abs(dx) <= width && abs(dy) <= height)
            {
                if (crossWidth >= crossHeight)
                {
                    if (crossWidth > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    // Gave a bias to top collision to avoid right edge stopping player during movement
                    if (crossWidth - 200 > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };

        void draw_block() override
        {
            if(!is_stopped)
            {
                draw_bitmap("Water", position.x, position.y, opts);
                update_animation(this->anim);
                if (animation_ended(this->anim))
                    restart_animation(this->anim);
                this->is_flowing = true;
                time = 0;
            }
            else
            {
                if(is_flowing)
                {
                    draw_bitmap("Water", position.x, position.y, opts);
                    update_animation(this->anim);
                    if (animation_ended(this->anim))
                        restart_animation(this->anim);

                    if(time > 10)
                    {
                        this->is_flowing = false;
                    }
                }

                if(time < 15)
                    time += 1;
            }
        } 
};

class ToxicBlock : public Block
{
    private:
        animation anim;

    public:
        ToxicBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->cell = cell;
            this->opts.draw_cell = this->cell;

            animation_script toxic_script = animation_script_named("CellAnim");
            animation anim = create_animation(toxic_script, "ToxicFlow");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
        }

        void draw_block() override
        {
            draw_bitmap("Toxic", position.x, position.y, opts);
            update_animation(this->anim);
            if (animation_ended(this->anim))
                restart_animation(this->anim);
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class DoorBlock : public Block
{
    private:
        animation anim;

    public:
        DoorBlock(bitmap cell_sheet, point_2d position) : Block(cell_sheet, position)
        {
            this->opts.draw_cell = this->cell;

            animation_script door_script = animation_script_named("CellAnim");
            animation anim = create_animation(door_script, "Door_Open");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
            make_hitbox();
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };

        void draw_block() override
        {
            draw_bitmap("Door", position.x, position.y, opts);
            update_animation(this->anim);
            if (animation_ended(this->anim))
                restart_animation(this->anim);
        }

        void make_hitbox() override
        {
            rectangle hitbox;
            hitbox.x = this->position.x + 10;
            hitbox.y = this->position.y + 20;
            hitbox.height = bitmap_cell_height(this->image) - 20;
            hitbox.width = bitmap_cell_width(this->image) - 30;
            this->hitbox = hitbox;
        };

        void open_portal()
        {
            animation_script door_script = animation_script_named("CellAnim");
            animation anim = create_animation(door_script, "Door_Portal");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
        }
};

class HoldablePipeBlock : public Block
{
    public:
        HoldablePipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };

        void draw_block() override
        {
            if(!is_picked_up)
                draw_bitmap(image, position.x, position.y, opts);
        };
};

class TurnablePipeBlock : public Block
{
    public:
        TurnablePipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_turnable = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class MultiTurnablePipeBlock : public Block
{
    public:
        MultiTurnablePipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_turnable = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class EmptyPipeBlock : public Block
{
    public:
        EmptyPipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_flowing = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
            make_special_hitbox();
        }

        void make_special_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x - 20;
            hitbox.y = this->position.y - 20;
            hitbox.height = bitmap_cell_height(this->image) + 40;
            hitbox.width = bitmap_cell_width(this->image) + 40;
            this->special_hitbox = hitbox;
        };

        // Collision to test distance from how far a player is and if holding pipe to place
        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };

        string special_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->special_hitbox)) && (rectangle_right(one) > rectangle_left(this->special_hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->special_hitbox)) && (rectangle_bottom(one) > rectangle_top(this->special_hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";

        };
};

class EmptyTurnBlock : public Block
{
    public:
        EmptyTurnBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_flowing = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        // Collision to test distance from how far a player is and if holding pipe to place
        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class EmptyMultiTurnBlock : public Block
{
    public:
        EmptyMultiTurnBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_flowing = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        // Collision to test distance from how far a player is and if holding pipe to place
        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class EdgeBlock : public Block
{
    public:
        EdgeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->position = position;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override
        {
            string collision = "None";
            double dx = (one.x + one.width / 2) - (this->hitbox.x + this->hitbox.width / 2);
            double dy = (one.y + one.height / 2) - (this->hitbox.y + this->hitbox.height / 2);
            double width = (one.width + this->hitbox.width) / 2;
            double height = (one.height + this->hitbox.height) / 2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if (abs(dx) <= width && abs(dy) <= height)
            {
                if (crossWidth >= crossHeight)
                {
                    if (crossWidth > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    // Gave a bias to top collision to avoid right edge stopping player during movement
                    if (crossWidth - 200 > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };
};
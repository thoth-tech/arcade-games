#include "splashkit.h"

class Button
{
    protected:
        bitmap button_bmp;
        point_2d position;
        int id;
        string text;
        font button_font;
        int font_size = 25;
        bool selected = false;
        color font_color = COLOR_BLACK;
        color selected_color = COLOR_RED;
    
    public:
        Button(bitmap button_bmp, int offset, int id, string text)
        {
            this->button_bmp = button_bmp;
            this->id = id;
            this->text = text;
            this->button_font = font_named("DefaultFont");
            point_2d pt = screen_center();
            position.x = pt.x - bitmap_width(this->button_bmp)/2;
            position.y = pt.y - bitmap_height(this->button_bmp)/2 + offset;
        };
        ~Button(){};

        void draw()
        {
            color text_color;
            point_2d center = bitmap_center(this->button_bmp);
            draw_bitmap(button_bmp, position.x, position.y, option_to_screen());
            if(!selected)
                text_color = font_color;
            else
                text_color = selected_color;

            draw_text(text, text_color, button_font, font_size, (center.x + position.x) - text_width(text, button_font, font_size)/2, (center.y + position.y) - text_height(text, button_font, font_size)/2, option_to_screen());
        };

        void set_selected(bool new_value)
        {
            this->selected = new_value;
        }

        int get_id()
        {
            return this->id;
        }
};

class SmallButton : public Button
{
    public:
        SmallButton(bitmap button_bmp, int offset, int id, string text, int font_size, color font_color, color selected_color) : Button(button_bmp, offset, id, text)
        {
            this->font_size = font_size;
            this->font_color = font_color;
            this->selected_color = selected_color;
        }
};
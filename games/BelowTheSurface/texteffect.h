#include "splashkit.h"
#include <stdio.h> 
#include <stdlib.h>
#include <vector>
#include <time.h>

class TextEffect
{
    private:
        vector<string> text;
        vector<double> positions;
        vector<bool> forward;
        int y_position;
        int x_position;
        double x_min;
        double x_max;
        font text_font;
        int font_size;

    public:
        TextEffect(vector<string> text, int x_position, int y_position, font text_font, int font_size)
        {
            this->text = text;
            this->x_position = x_position;
            this->y_position = y_position;
            this->text_font = text_font;
            this->font_size = font_size;

            process();
        };
        ~TextEffect(){};

        void process()
        {
            int max = -1000;
            srand(time(NULL));
            for(int i = 0; i < text.size(); i++)
            {
                if(max < text_width(text[i], text_font, font_size))
                    max = text_width(text[i], text_font, font_size)/4;
                    
                int random_pos = (rand() % 200 + 1) - 100;
                random_pos = x_position + random_pos;
                positions.push_back(random_pos);

                int rando = rand() % 2 + 0;
                if(rando == 1)
                    forward.push_back(true);
                else
                    forward.push_back(false);
            }

            this->x_max = x_position + max;
            this->x_min = x_position - max;
        };

        void update()
        {
            for(int i = 0; i < text.size(); i++)
            {
                if(forward[i])
                {
                    positions[i] += 1;

                    if(positions[i] > x_max + 200)
                        forward[i] = false;
                }
                else
                {
                    positions[i] -= 1;

                    if(positions[i] < x_min)
                        forward[i] = true;
                }

                draw_text(text[i], COLOR_WHITE, text_font, font_size, positions[i], y_position + (i * text_height(text[i], text_font, font_size)));
            }
        };
};
#include "splashkit.h"
#include <memory>
#include <vector>

#pragma once

class Letter
{
    private:
        string val;
        point_2d pos;
        bool selected = false;
        font font_type;
        int font_size = 25;
        color font_color;
        color select_color;
        int letter_width;
        int letter_height;

    public:
        Letter(string val, point_2d pos)
        {
            this->val = val;
            this->pos = pos;
            this->font_type = font_named("DefaultFont");
            this->font_color = COLOR_WHITE;
            this->select_color = COLOR_RED;
            this->letter_width = text_width(val, font_type, font_size);
            this->letter_height = text_height(val, font_type, font_size);
        };
        ~Letter(){};

        void draw()
        {
            if(selected)
            {
                draw_text(val, select_color, font_type, font_size, pos.x, pos.y);
            }
            else
            {
                draw_text(val, font_color, font_type, font_size, pos.x, pos.y);
            }
        };

        string get_value()
        {
            return this->val;
        };

        void set_value(string val)
        {
            this->val = val;
        };

        void set_selected(bool new_value)
        {
            this->selected = new_value;
        };

        bool is_selected()
        {
            return this->selected;
        };

        int get_letter_width()
        {
            return this->letter_width;
        }; 

        int get_letter_height()
        {
            return this->letter_height;
        };   
};

class Password
{
    private:
        vector<std::shared_ptr<Letter>> keyboard;
        vector<std::shared_ptr<Letter>> underscore;
        int selection = 0;
        int letter = 0;

    public:
        Password()
        {
            process_alphabet();
            process_underscore();
        };
        ~Password(){};

        void draw_element(vector<std::shared_ptr<Letter>> element)
        {
            for(int i = 0; i < element.size(); i++)
            {
                element[i]->draw();
            }
        };

        void process_alphabet()
        {
            string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ<~`";
            int row = 0;
            int col = 0;
            int letter_width = 0;
            int letter_height = 0;
            bool retest = true;

            for(int i = 0; i < alphabet.size(); i++)
            {
                if(i == 0){}
                else if(i % 6 == 0)
                {
                    row += 1;
                    col = 0;
                }

                char c = alphabet.at(i);
                string str(1, c);

                point_2d pos = screen_center();
                pos.x = pos.x*0.7;
                pos.y = pos.x*0.75;
                pos.x += col * letter_width * 3;
                pos.y += row * letter_height * 2;

                if(str == "<")
                {
                    str = "DEL";
                    pos.y += letter_height * 2;
                }
                if(str == "~")
                {
                    str = "ENTER";
                    pos.x -= col * letter_width * 3;
                    pos.x += col * letter_width * 4;
                    pos.y += letter_height * 2;
                }
                if(str == "`")
                {
                    str = "EXIT";
                    pos.x -= col * letter_width * 3;
                    pos.x += col * letter_width * 5;
                    pos.y += letter_height *2;
                }

                std::shared_ptr<Letter> alpha(new Letter(str, pos));
                if(retest)
                {
                    letter_height = alpha->get_letter_height();
                    letter_width = alpha->get_letter_width();
                    retest = false;
                }
                keyboard.push_back(alpha);

                col += 1;
            }
        };

        void process_underscore()
        {
            string letters = "_ _ _ _ _";

            int letter_width = 0;
            bool retest = true;
            for(int i = 0; i < letters.size(); i++)
            {
                char c = letters.at(i);
                string str(1, c);
                
                point_2d pos = screen_center();
                pos.x = pos.x*0.8;
                pos.x += (i * letter_width);
                pos.y -= 200;

                std::shared_ptr<Letter> alpha(new Letter(str, pos));
                if(retest)
                {
                    letter_width = alpha->get_letter_width();
                    retest = false;
                }
                underscore.push_back(alpha);
            }
        };

        string input()
        {
            string password = "";

            if(key_typed(D_KEY))
            {
                selection += 1;

                if(selection > keyboard.size() - 1)
                    selection = 0;
            }

            if(key_typed(A_KEY))
            {
                selection -= 1;

                if(selection < 0)
                    selection = keyboard.size() - 1;
            }

            if(key_typed(S_KEY))
            {
                selection += 6;

                if(selection > keyboard.size() - 1)
                {
                    if(selection < 30)
                        selection = 27;
                    else
                        selection = 0 + (selection - 30);
                }
            }

            if(key_typed(W_KEY))
            {
                selection -= 6;

                if(selection < 0)
                {
                    if(selection < -2)
                        selection = selection + 30;
                    else
                        selection = 27;
                }
            }

            if(key_typed(F_KEY) || key_typed(RETURN_KEY))
            {
                string select = "";
                for(int i = 0; i < keyboard.size(); i++)
                {
                    if(keyboard[i]->is_selected())
                    {
                        select = keyboard[i]->get_value();
                    }
                }
                if(select == "DEL")
                {
                    if (!sound_effect_playing("EnemyDead"))
                        play_sound_effect("EnemyDead");
                    if(letter > 0)
                        letter -= 2;
                    underscore[letter]->set_value("_");
                }
                else if(select == "ENTER")
                {
                    string temp = "";
                    for(int i = 0; i < underscore.size(); i += 2)
                        temp.append(underscore[i]->get_value());
                    
                    password = temp;
                }
                else if (select == "EXIT")
                {
                    password = "EXITEXITEXIT";
                }
                else if(letter < 10)
                {
                    if (!sound_effect_playing("Jump"))
                        play_sound_effect("Jump");
                    underscore[letter]->set_value(select);
                    letter += 2;
                }
            }

            return password;
        };

        string update()
        {
            clear_screen(COLOR_BLACK);
            draw_bitmap("MenubgDark", 0, 0, option_to_screen());
            
            for(int i = 0; i < keyboard.size(); i++)
            {
                if(selection == i)
                    keyboard[i]->set_selected(true);
                else
                    keyboard[i]->set_selected(false);
            }

            string password = input();
            draw_element(keyboard);
            draw_element(underscore);

            return password;
        }
};
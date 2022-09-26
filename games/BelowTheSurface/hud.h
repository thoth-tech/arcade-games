#include "splashkit.h"
#include "player.h"
#include <memory>

#pragma once

class HUD
{
    private:
        //Added the current level players so you have easy access to them
        vector<std::shared_ptr<Player>> level_players;

        void high_score()
        {
            // To Be Done.
        }

    public:
        HUD(vector<std::shared_ptr<Player>> level_players)
        {
            this->level_players = level_players;
        };
        ~HUD(){};

        void update()
        {
            draw_text("HEALTH: ", COLOR_WHITE, "DefaultFont", 9, 60, 100, option_to_screen());
            draw_text("LIVES: ", COLOR_WHITE, "DefaultFont", 9, 60, 70, option_to_screen());

            if(level_players.size() > 1 )
            {
                draw_bitmap("PinkEmptyBar", 125, 80, option_part_bmp(0, 0, 64, 32, option_to_screen()));
                draw_bitmap("PinkHealthBar", 125, 80, option_part_bmp(0, 0, 64/3*level_players[1]->player_health, 32, option_to_screen()));

                if(level_players[1]->player_lives==3)
                {                
                    draw_bitmap("PinkLive", 190, 55, option_to_screen());
                    draw_bitmap("PinkLive", 155, 55, option_to_screen());
                    draw_bitmap("PinkLive", 120, 55, option_to_screen());
                }
                if(level_players[1]->player_lives==2)
                {
                    draw_bitmap("PinkLive", 155, 55, option_to_screen());
                    draw_bitmap("PinkLive", 120, 55, option_to_screen());
                }
                if(level_players[1]->player_lives==1)
                {
                    draw_bitmap("PinkLive", 120, 55, option_to_screen());
                }

                draw_bitmap("BlueEmptyBar", 270, 80, option_part_bmp(0, 0, 64, 32, option_to_screen()));
                draw_bitmap("BlueHealthBar", 270, 80, option_part_bmp(0, 0, 64/3*level_players[0]->player_health, 32, option_to_screen()));

                if(level_players[0]->player_lives==3)
                {                
                    draw_bitmap("BlueLive", 335, 55, option_to_screen());
                    draw_bitmap("BlueLive", 300, 55, option_to_screen());
                    draw_bitmap("BlueLive", 265, 55, option_to_screen());
                }
                if(level_players[0]->player_lives==2)
                {
                    draw_bitmap("BlueLive", 300, 55, option_to_screen());
                    draw_bitmap("BlueLive", 265, 55, option_to_screen());
                }
                if(level_players[0]->player_lives==1)
                {
                    draw_bitmap("BlueLive", 265, 55, option_to_screen());
                } 
            }
            else
            {              
                draw_bitmap("PurpleEmptyBar", 125, 80, option_part_bmp(0, 0, 64, 32, option_to_screen()));
                draw_bitmap("PurpleHealthBar", 125, 80, option_part_bmp(0, 0, 64/3*level_players[0]->player_health, 32, option_to_screen()));
                   
                   if(level_players[0]->player_lives==3)
                {                
                    draw_bitmap("PurpleLive", 190, 55, option_to_screen());
                    draw_bitmap("PurpleLive", 155, 55, option_to_screen());
                    draw_bitmap("PurpleLive", 120, 55, option_to_screen());
                }
                if(level_players[0]->player_lives==2)
                {
                    draw_bitmap("PurpleLive", 155, 55, option_to_screen());
                    draw_bitmap("PurpleLive", 120, 55, option_to_screen());
                }
                if(level_players[0]->player_lives==1)
                {
                    draw_bitmap("PurpleLive", 120, 55, option_to_screen());
                }
            }
        };
};
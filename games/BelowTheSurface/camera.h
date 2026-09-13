#include "splashkit.h"
#include "player.h"
#include <memory>
#include <vector>
#include <iostream>

#pragma once

using namespace std;

class Camera
{
    private:
        double x_border_left = 0;
        double y_border_top = 0;
        double x_border_right;
        double y_border_bottom;
        bool first_update = true;
        vector<shared_ptr<Player>> players;

    public:
        // Camera(std::shared_ptr<Player> player, int tile_size, int map_height, int map_width)
        Camera(vector<shared_ptr<Player>> players, int tile_size, int map_height, int map_width)
        {
            this->players = players;
            this->x_border_right = tile_size * map_width;
            this->y_border_bottom = -(tile_size * map_height);
        };

        ~Camera(){};


        void update()
        {
            double mid_player_x = 0;
            double mid_player_y = 0;

            for(int i = 0; i < players.size(); i++)
            {
                if(players[i]->get_player_sprite())
                {
                    mid_player_x += (sprite_position(players[i]->get_player_sprite()).x);
                    mid_player_y += (sprite_position(players[i]->get_player_sprite()).y);
                }
            }

            mid_player_x = mid_player_x / players.size();
            mid_player_y = mid_player_y / players.size();

            point_2d mid_player = {mid_player_x, mid_player_y};

            // Target position (Where the camera should be)
            double sc_x = mid_player.x - (screen_width() / 2);
            double sc_y = mid_player.y - (screen_height() / 2);

            if (first_update)
            {
                move_camera_to(sc_x, sc_y);
                first_update = false;
            }
            else 
            {
                // Current position
                double cam_x = camera_x();
                double cam_y = camera_y();
                
                // Next eased position
                double smooth = 0.05;
                cam_x += (sc_x - cam_x) * smooth;
                cam_y += (sc_y - cam_y) * smooth;
    
                move_camera_to(cam_x, cam_y);
            }

            if(camera_x() < x_border_left)
                set_camera_x(x_border_left);
            if(camera_x() + screen_width() > x_border_right)
                set_camera_x(x_border_right - screen_width());
            if(camera_y() < y_border_top)
                set_camera_y(y_border_top);
            if(y_border_bottom > -camera_y() - screen_height())
                set_camera_y(abs(y_border_bottom) - screen_height());
        }
};

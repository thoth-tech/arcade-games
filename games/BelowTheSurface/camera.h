#include "splashkit.h"
#include "player.h"
#include <memory>
#pragma once

class Camera
{
    private:
        double x_border_left = 0;
        double y_border_top = 0;
        double x_border_right;
        double y_border_bottom;

    public:
        // Camera(std::shared_ptr<Player> player, int tile_size, int map_height, int map_width)
        Camera(int tile_size, int map_height, int map_width)
        {
            this->x_border_right = tile_size * map_width;
            this->y_border_bottom = -(tile_size * map_height);
        };

        ~Camera(){};


        void update(sprite s1, sprite s2)
        {
            point_2d s1_pos = sprite_position(s1);
            point_2d s2_pos = sprite_position(s2);
            
            double mid_player_x = (s1_pos.x + s2_pos.x) / 2;
            double mid_player_y = (s1_pos.y + s2_pos.y) / 2;

            point_2d mid_player = {mid_player_x, mid_player_y};

            double sc_x = mid_player.x + - (screen_width() / 2);
            double sc_y = mid_player.y + - (screen_height() / 2);

            move_camera_to(sc_x, sc_y);


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

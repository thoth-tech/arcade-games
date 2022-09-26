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
        std::shared_ptr<Player> player;

    public:
        Camera(std::shared_ptr<Player> player, int tile_size, int map_height, int map_width)
        {
            this->player = player;

            this->x_border_right = tile_size * map_width;
            this->y_border_bottom = -(tile_size * map_height);
        };

        ~Camera(){};


        void update()
        {
            center_camera_on(this->player->get_player_sprite(), 0, 0);

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

#include "splashkit.h"
#include <vector>

class Background
{
    protected:
        vector<bitmap> images;

    public:
        Background(){};
        ~Background(){};

        virtual void draw()
        {
            for(int i = 0; i < images.size(); i++)
            {
                draw_bitmap(images[i], 0, 0, option_to_screen());
            }
        };
};

class GreyBackground : public Background
{
    public:
        GreyBackground()
        {
            images.push_back(bitmap_named("GreyBackground"));
        }
};

class DarkBackground : public Background
{
    public:
        DarkBackground()
        {
            images.push_back(bitmap_named("DarkBackground"));
        }
};

class SkyBackground : public Background
{
    public:
        SkyBackground()
        {
            images.push_back(bitmap_named("BlueBackground"));
        }
};
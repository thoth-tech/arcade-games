#ifndef _BANNER_H_
#define _BANNER_H_

#include "timer.h"
#include "vector2.h"
#include "config_manager.h"
#include "resources_manager.h"

#include <SDL.h>

class Banner
{
public:
	Banner()
	{
		size_foreground = { 646, 215 };
		size_background = { 1282, 209 };

		timer_display.set_one_shot(true);
		timer_display.set_wait_time(5);
		timer_display.set_on_timeout(
			[&]()
			{
				is_end_display = true;
			});
	}

	~Banner() = default;

	void set_center_position(const Vector2& pos)
	{
		pos_center = pos;
	}

	void on_update(double delta)
	{
		timer_display.on_update(delta);

		const ResourcesManager::TexturePool& tex_pool
			= ResourcesManager::instance()->get_texture_pool();
		const ConfigManager* instance = ConfigManager::instance();

		tex_foreground = tex_pool.find(instance->is_game_win ? ResID::Tex_UIWinText : ResID::Tex_UILossText)->second;
		tex_background = tex_pool.find(ResID::Tex_UIGameOverBar)->second;
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Rect rect_dst;

		rect_dst.x = (int)(pos_center.x - size_background.x / 2);
		rect_dst.y = (int)(pos_center.y - size_background.y / 2);
		rect_dst.w = (int)size_background.x, rect_dst.h = (int)size_background.y;
		SDL_RenderCopy(renderer, tex_background, nullptr, &rect_dst);

		rect_dst.x = (int)(pos_center.x - size_foreground.x / 2);
		rect_dst.y = (int)(pos_center.y - size_foreground.y / 2);
		rect_dst.w = (int)size_foreground.x, rect_dst.h = (int)size_foreground.y;
		SDL_RenderCopy(renderer, tex_foreground, nullptr, &rect_dst);
	}

	bool check_end_dispaly()
	{
		return is_end_display;
	}

private:
	Vector2 pos_center;

	Vector2 size_foreground;
	Vector2 size_background;

	SDL_Texture* tex_foreground = nullptr;
	SDL_Texture* tex_background = nullptr;

	Timer timer_display;
	bool is_end_display = false;

};

#endif // !_BANNER_H_

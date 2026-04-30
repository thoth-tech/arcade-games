#ifndef _COIN_PROP_H_
#define _COIN_PROP_H_
#include "tile.h"
#include"timer.h"
#include "vector2.h"
#include "resources_manager.h"

#include <SDL.h>



class CoinProp
{
public:
	CoinProp()
	{
		timer_jump.set_one_shot(true);
		timer_jump.set_wait_time(interval_jump);
		timer_jump.set_on_timeout(
			[&]()
			{
				is_jumping = false;
			});

		timer_disappear.set_one_shot(true);
		timer_disappear.set_wait_time(interval_disappear);
		timer_disappear.set_on_timeout(
			[&]()
			{
				is_valid = false;
			});
		velocity.x = (rand() % 2 ? 1 : -1) * 2 * SIZE_TILE;//随机向左向右的方向
		velocity.y = -3 * SIZE_TILE;
	}

	~CoinProp() = default;

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void make_invalid()
	{
		is_valid = false;
	}

	bool can_remove()
	{
		return !is_valid;
	}

	void on_update(double delta)
	{
		timer_jump.on_update(delta);
		timer_disappear.on_update(delta);

		if (is_jumping)
		{
			velocity.y += gravity * delta;
		}
		else
		{
			velocity.x = 0;
			velocity.y = sin(SDL_GetTicks64() / 1000.0 * 4) * 30;
		}

		position += velocity * delta;
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Rect rect = { 0, 0, (int)size.x, (int)size.y };
		static SDL_Texture* tex_coin = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_Coin)->second;

		rect.x = (int)(position.x - size.x / 2);
		rect.y = (int)(position.y - size.y / 2);

		SDL_RenderCopy(renderer, tex_coin, nullptr, &rect);
	}
private:
	Vector2 position;
	Vector2 velocity;

	Timer timer_jump;
	Timer timer_disappear;

	bool is_valid = true;
	bool is_jumping = true;

	double gravity = 490;
	double interval_jump = 0.75;
	Vector2 size = { 16, 16 };
	double interval_disappear = 10;

};

#endif // !_COIN_PROP_H_


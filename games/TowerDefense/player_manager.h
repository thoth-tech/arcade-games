#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_

#include "tile.h"
#include "facing.h"
#include "vector2.h"
#include "manager.h"
#include "animation.h"
#include "coin_manager.h"
#include "enemy_manager.h"
#include "resources_manager.h"

#include <SDL.h>

class PlayerManager : public Manager<PlayerManager>
{
	friend class Manager<PlayerManager>;

public:
	void on_input(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				is_move_left = true;
				break;
			case SDLK_d:
				is_move_right = true;
				break;
			case SDLK_w:
				is_move_up = true;
				break;
			case SDLK_s:
				is_move_down = true;
				break;
			case SDLK_j:
				on_release_flash();
				break;
			case SDLK_k:
				on_release_impact();
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				is_move_left = false;
				break;
			case SDLK_d:
				is_move_right = false;
				break;
			case SDLK_w:
				is_move_up = false;
				break;
			case SDLK_s:
				is_move_down = false;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	void on_update(double delta)
	{
		timer_auto_increase_mp.on_update(delta);
		timer_release_flash_cd.on_update(delta);

		Vector2 direction =
			Vector2(is_move_right - is_move_left,
				is_move_down - is_move_up).normalize();
		velocity = direction * speed * SIZE_TILE;

		if (!is_releasing_flash && !is_releasing_impact)
		{
			position += velocity * delta;

			const SDL_Rect& rect_map = ConfigManager::instance()->rect_tile_map;
			if (position.x < rect_map.x) position.x = rect_map.x;
			if (position.x > rect_map.x + rect_map.w) position.x = rect_map.x + rect_map.w;
			if (position.y < rect_map.y) position.y = rect_map.y;
			if (position.y > rect_map.y + rect_map.h) position.y = rect_map.y + rect_map.h;

			if (velocity.y > 0) facing = Facing::Down;
			if (velocity.y < 0) facing = Facing::Up;
			if (velocity.x > 0) facing = Facing::Right;
			if (velocity.x < 0) facing = Facing::Left;

			switch (facing)
			{
			case Facing::Left:
				anim_current = &anim_idle_left;
				break;
			case Facing::Right:
				anim_current = &anim_idle_right;
				break;
			case Facing::Up:
				anim_current = &anim_idle_up;
				break;
			case Facing::Down:
				anim_current = &anim_idle_down;
				break;
			}
		}
		else
		{
			switch (facing)
			{
			case Facing::Left:
				anim_current = &anim_attack_left;
				break;
			case Facing::Right:
				anim_current = &anim_attack_right;
				break;
			case Facing::Up:
				anim_current = &anim_attack_up;
				break;
			case Facing::Down:
				anim_current = &anim_attack_down;
				break;
			}
		}

		anim_current->on_update(delta);

		if (is_releasing_flash)
		{
			anim_effect_flash_current->on_update(delta);

			EnemyManager::EnemyList& enemy_list
				= EnemyManager::instance()->get_enemy_list();
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->can_remove())
					continue;

				const Vector2& position = enemy->get_position();
				if (position.x >= rect_hitbox_flash.x
					&& position.x <= rect_hitbox_flash.x + rect_hitbox_flash.w
					&& position.y >= rect_hitbox_flash.y
					&& position.y <= rect_hitbox_flash.y + rect_hitbox_flash.h)
				{
					enemy->decrease_hp(ConfigManager::instance()->player_template.normal_attack_damage * delta);
				}
			}
		}

		if (is_releasing_impact)
		{
			anim_effect_impact_current->on_update(delta);

			EnemyManager::EnemyList& enemy_list
				= EnemyManager::instance()->get_enemy_list();
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->can_remove())
					continue;

				const Vector2& size = enemy->get_size();
				const Vector2& position = enemy->get_position();
				if (position.x >= rect_hitbox_impact.x
					&& position.x <= rect_hitbox_impact.x + rect_hitbox_impact.w
					&& position.y >= rect_hitbox_impact.y
					&& position.y <= rect_hitbox_impact.y + rect_hitbox_impact.h)
				{
					enemy->decrease_hp(ConfigManager::instance()->player_template.skill_damage * delta);
					enemy->slow_down();
				}
			}
		}

		CoinManager::CoinPropList& coin_prop_list = CoinManager::instance()->get_coin_prop_list();
		static const ResourcesManager::SoundPool& sound_pool = ResourcesManager::instance()->get_sound_pool();

		for (CoinProp* coin_prop : coin_prop_list)
		{
			if (coin_prop->can_remove())
				continue;

			const Vector2& pos_coin_prop = coin_prop->get_position();
			if (pos_coin_prop.x >= position.x - size.x / 2
				&& pos_coin_prop.x <= position.x + size.x / 2
				&& pos_coin_prop.y >= position.y - size.y / 2
				&& pos_coin_prop.y <= position.y + size.y / 2)
			{
				coin_prop->make_invalid();
				CoinManager::instance()->increase_coin(10);

				Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_Coin)->second, 0);
			}
		}
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point point;

		point.x = (int)(position.x - size.x / 2);
		point.y = (int)(position.y - size.y / 2);
		anim_current->on_render(renderer, point);

		if (is_releasing_flash)
		{
			point.x = rect_hitbox_flash.x;
			point.y = rect_hitbox_flash.y;
			anim_effect_flash_current->on_render(renderer, point);
		}

		if (is_releasing_impact)
		{
			point.x = rect_hitbox_impact.x;
			point.y = rect_hitbox_impact.y;
			anim_effect_impact_current->on_render(renderer, point);
		}
	}

	double get_current_mp() const
	{
		return mp;
	}

protected:
	PlayerManager()
	{
		timer_auto_increase_mp.set_one_shot(false);
		timer_auto_increase_mp.set_wait_time(0.1);
		timer_auto_increase_mp.set_on_timeout(
			[&]()
			{
				double interval = ConfigManager::instance()->player_template.skill_interval;
				mp = std::min(mp + 100 / (interval / 0.1), 100.0);
			});

		timer_release_flash_cd.set_one_shot(true);
		timer_release_flash_cd.set_wait_time(
			ConfigManager::instance()->player_template.skill_interval);
		timer_release_flash_cd.set_on_timeout(
			[&]()
			{
				can_release_flash = true;
			});

		const ResourcesManager::TexturePool& tex_pool
			= ResourcesManager::instance()->get_texture_pool();

		SDL_Texture* tex_player = tex_pool.find(ResID::Tex_Player)->second;

		anim_idle_up.set_loop(true); anim_idle_up.set_interval(0.1);
		anim_idle_up.set_frame_data(tex_player, 4, 8, { 4, 5, 6, 7 });
		anim_idle_down.set_loop(true); anim_idle_down.set_interval(0.1);
		anim_idle_down.set_frame_data(tex_player, 4, 8, { 0, 1, 2, 3 });
		anim_idle_left.set_loop(true); anim_idle_left.set_interval(0.1);
		anim_idle_left.set_frame_data(tex_player, 4, 8, { 8, 9, 10, 11 });
		anim_idle_right.set_loop(true); anim_idle_right.set_interval(0.1);
		anim_idle_right.set_frame_data(tex_player, 4, 8, { 12, 13, 14, 15 });

		anim_attack_up.set_loop(true); anim_attack_up.set_interval(0.1);
		anim_attack_up.set_frame_data(tex_player, 4, 8, { 20, 21 });
		anim_attack_down.set_loop(true); anim_attack_down.set_interval(0.1);
		anim_attack_down.set_frame_data(tex_player, 4, 8, { 16, 17 });
		anim_attack_left.set_loop(true); anim_attack_left.set_interval(0.1);
		anim_attack_left.set_frame_data(tex_player, 4, 8, { 24, 25 });
		anim_attack_right.set_loop(true); anim_attack_right.set_interval(0.1);
		anim_attack_right.set_frame_data(tex_player, 4, 8, { 28, 29 });

		anim_effect_flash_up.set_loop(false); anim_effect_flash_up.set_interval(0.1);
		anim_effect_flash_up.set_frame_data(tex_pool.find(ResID::Tex_EffectFlash_Up)->second, 5, 1, { 0, 1, 2, 3, 4 });
		anim_effect_flash_up.set_on_finished([&]() { is_releasing_flash = false; });
		anim_effect_flash_down.set_loop(false); anim_effect_flash_down.set_interval(0.1);
		anim_effect_flash_down.set_frame_data(tex_pool.find(ResID::Tex_EffectFlash_Down)->second, 5, 1, { 4, 3, 2, 1, 0 });
		anim_effect_flash_down.set_on_finished([&]() { is_releasing_flash = false; });
		anim_effect_flash_left.set_loop(false); anim_effect_flash_left.set_interval(0.1);
		anim_effect_flash_left.set_frame_data(tex_pool.find(ResID::Tex_EffectFlash_Left)->second, 1, 5, { 4, 3, 2, 1, 0 });
		anim_effect_flash_left.set_on_finished([&]() { is_releasing_flash = false; });
		anim_effect_flash_right.set_loop(false); anim_effect_flash_right.set_interval(0.1);
		anim_effect_flash_right.set_frame_data(tex_pool.find(ResID::Tex_EffectFlash_Right)->second, 1, 5, { 0, 1, 2, 3, 4 });
		anim_effect_flash_right.set_on_finished([&]() { is_releasing_flash = false; });

		anim_effect_impact_up.set_loop(false); anim_effect_impact_up.set_interval(0.1);
		anim_effect_impact_up.set_frame_data(tex_pool.find(ResID::Tex_EffectImpact_Up)->second, 5, 1, { 0, 1, 2, 3, 4 });
		anim_effect_impact_up.set_on_finished([&]() { is_releasing_impact = false; });
		anim_effect_impact_down.set_loop(false); anim_effect_impact_down.set_interval(0.1);
		anim_effect_impact_down.set_frame_data(tex_pool.find(ResID::Tex_EffectImpact_Down)->second, 5, 1, { 4, 3, 2, 1, 0 });
		anim_effect_impact_down.set_on_finished([&]() { is_releasing_impact = false; });
		anim_effect_impact_left.set_loop(false); anim_effect_impact_left.set_interval(0.1);
		anim_effect_impact_left.set_frame_data(tex_pool.find(ResID::Tex_EffectImpact_Left)->second, 1, 5, { 4, 3, 2, 1, 0 });
		anim_effect_impact_left.set_on_finished([&]() { is_releasing_impact = false; });
		anim_effect_impact_right.set_loop(false); anim_effect_impact_right.set_interval(0.1);
		anim_effect_impact_right.set_frame_data(tex_pool.find(ResID::Tex_EffectImpact_Right)->second, 1, 5, { 0, 1, 2, 3, 4 });
		anim_effect_impact_right.set_on_finished([&]() { is_releasing_impact = false; });

		const SDL_Rect& rect_map = ConfigManager::instance()->rect_tile_map;
		position.x = rect_map.x + rect_map.w / 2;
		position.y = rect_map.y + rect_map.h / 2;

		speed = ConfigManager::instance()->player_template.speed;

		size.x = 96, size.y = 96;
	}

	~PlayerManager() = default;

private:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;

	SDL_Rect rect_hitbox_flash = { 0 };
	SDL_Rect rect_hitbox_impact = { 0 };

	double mp = 100;

	double speed = 0;

	bool can_release_flash = true;
	bool is_releasing_flash = false;
	bool is_releasing_impact = false;

	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

	Animation anim_idle_up;
	Animation anim_idle_down;
	Animation anim_idle_left;
	Animation anim_idle_right;
	Animation anim_attack_up;
	Animation anim_attack_down;
	Animation anim_attack_left;
	Animation anim_attack_right;
	Animation* anim_current = &anim_idle_right;

	Animation anim_effect_flash_up;
	Animation anim_effect_flash_down;
	Animation anim_effect_flash_left;
	Animation anim_effect_flash_right;
	Animation* anim_effect_flash_current = nullptr;

	Animation anim_effect_impact_up;
	Animation anim_effect_impact_down;
	Animation anim_effect_impact_left;
	Animation anim_effect_impact_right;
	Animation* anim_effect_impact_current = nullptr;

	Timer timer_release_flash_cd;
	Timer timer_auto_increase_mp;

	Facing facing = Facing::Left;

private:
	void on_release_flash()
	{
		if (!can_release_flash || is_releasing_flash)
			return;

		switch (facing)
		{
		case Facing::Left:
			anim_effect_flash_current = &anim_effect_flash_left;
			rect_hitbox_flash.x = (int)(position.x - size.x / 2 - 300);
			rect_hitbox_flash.y = (int)(position.y - 68 / 2);
			rect_hitbox_flash.w = 300, rect_hitbox_flash.h = 68;
			break;
		case Facing::Right:
			anim_effect_flash_current = &anim_effect_flash_right;
			rect_hitbox_flash.x = (int)(position.x + size.x / 2);
			rect_hitbox_flash.y = (int)(position.y - 68 / 2);
			rect_hitbox_flash.w = 300, rect_hitbox_flash.h = 68;
			break;
		case Facing::Up:
			anim_effect_flash_current = &anim_effect_flash_up;
			rect_hitbox_flash.x = (int)(position.x - 68 / 2);
			rect_hitbox_flash.y = (int)(position.y - size.x / 2 - 300);
			rect_hitbox_flash.w = 68, rect_hitbox_flash.h = 300;
			break;
		case Facing::Down:
			anim_effect_flash_current = &anim_effect_flash_down;
			rect_hitbox_flash.x = (int)(position.x - 68 / 2);
			rect_hitbox_flash.y = (int)(position.y + size.x / 2);
			rect_hitbox_flash.w = 68, rect_hitbox_flash.h = 300;
			break;
		}

		is_releasing_flash = true;
		anim_effect_flash_current->reset();
		timer_release_flash_cd.restart();

		static const ResourcesManager::SoundPool& sound_pool
			= ResourcesManager::instance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_Flash)->second, 0);
	}

	void on_release_impact()
	{
		if (mp < 100 || is_releasing_impact)
			return;

		switch (facing)
		{
		case Facing::Left:
			anim_effect_impact_current = &anim_effect_impact_left;
			rect_hitbox_impact.x = (int)(position.x - size.x / 2 - 60);
			rect_hitbox_impact.y = (int)(position.y - 140 / 2);
			rect_hitbox_impact.w = 60, rect_hitbox_impact.h = 140;
			break;
		case Facing::Right:
			anim_effect_impact_current = &anim_effect_impact_right;
			rect_hitbox_impact.x = (int)(position.x + size.x / 2);
			rect_hitbox_impact.y = (int)(position.y - 140 / 2);
			rect_hitbox_impact.w = 60, rect_hitbox_impact.h = 140;
			break;
		case Facing::Up:
			anim_effect_impact_current = &anim_effect_impact_up;
			rect_hitbox_impact.x = (int)(position.x - 140 / 2);
			rect_hitbox_impact.y = (int)(position.y - size.x / 2 - 60);
			rect_hitbox_impact.w = 140, rect_hitbox_impact.h = 60;
			break;
		case Facing::Down:
			anim_effect_impact_current = &anim_effect_impact_down;
			rect_hitbox_impact.x = (int)(position.x - 140 / 2);
			rect_hitbox_impact.y = (int)(position.y + size.x / 2);
			rect_hitbox_impact.w = 140, rect_hitbox_impact.h = 60;
			break;
		}

		mp = 0;
		is_releasing_impact = true;
		anim_effect_impact_current->reset();

		static const ResourcesManager::SoundPool& sound_pool
			= ResourcesManager::instance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_Impact)->second, 0);
	}

};

#endif // !_PLAYER_MANAGER_H_

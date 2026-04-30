#ifndef _KINNG_SLIM_ENEMY_H_
#define _KINNG_SLIM_ENEMY_H_

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class KingSlimeEnemy : public Enemy
{
public:
	KingSlimeEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture* tex_king_slime = texture_pool.find(ResID::Tex_KingSlime)->second;
		static SDL_Texture* tex_king_slime_sketch = texture_pool.find(ResID::Tex_KingSlimeSketch)->second;
		static ConfigManager::EnemyTemplate& king_slim_template = ConfigManager::instance()->king_slim_template;

		static const std::vector<int> idx_list_up = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };
		static const std::vector<int> idx_list_left = { 6, 7, 8, 9, 10, 11 };
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };

		anim_up.set_loop(true); anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_king_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_king_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_king_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_king_slime, 6, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_right);

		max_hp = king_slim_template.hp;
		max_speed = king_slim_template.speed;
		damage = king_slim_template.damage;
		reward_ratio = king_slim_template.reward_ratio;
		recover_interval = king_slim_template.recover_interval;
		recover_range = king_slim_template.recover_range;
		recover_intensity = king_slim_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~KingSlimeEnemy() = default;

};


#endif // !_KINNG_SLIM_ENEMY_H_
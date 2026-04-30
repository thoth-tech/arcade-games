#ifndef _SKELETON_ENEMY_H_
#define _SKELETON_ENEMY_H_

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class SkeletonEnemy : public Enemy
{
public:
	SkeletonEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture* tex_skeleton = texture_pool.find(ResID::Tex_Skeleton)->second;
		static SDL_Texture* tex_skeleton_sketch = texture_pool.find(ResID::Tex_SkeletonSketch)->second;
		static ConfigManager::EnemyTemplate& skeleton_template = ConfigManager::instance()->skeleton_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };

		anim_up.set_loop(true); anim_up.set_interval(0.15);
		anim_up.set_frame_data(tex_skeleton, 5, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.15);
		anim_down.set_frame_data(tex_skeleton, 5, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.15);
		anim_left.set_frame_data(tex_skeleton, 5, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.15);
		anim_right.set_frame_data(tex_skeleton, 5, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.15);
		anim_up_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.15);
		anim_down_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.15);
		anim_left_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.15);
		anim_right_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_right);

		max_hp = skeleton_template.hp;
		max_speed = skeleton_template.speed;
		damage = skeleton_template.damage;
		reward_ratio = skeleton_template.reward_ratio;
		recover_interval = skeleton_template.recover_interval;
		recover_range = skeleton_template.recover_range;
		recover_intensity = skeleton_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~SkeletonEnemy() = default;

};

#endif // !_SKELETON_ENEMY_H_


#ifndef _GUNNER_TOWER_H_
#define _GUNNER_TOWER_H_

#include "tower.h"
#include "resources_manager.h"

class GunnerTower : public Tower
{
public:
	GunnerTower()
	{
		static SDL_Texture* tex_gunner = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_Gunner)->second;

		static const std::vector<int> idx_list_idle_up = { 4, 5 };
		static const std::vector<int> idx_list_idle_down = { 0, 1 };
		static const std::vector<int> idx_list_idle_left = { 12, 13 };
		static const std::vector<int> idx_list_idle_right = { 8, 9 };
		static const std::vector<int> idx_list_fire_up = { 20, 21, 22, 23 };
		static const std::vector<int> idx_list_fire_down = { 16, 17, 18, 19 };
		static const std::vector<int> idx_list_fire_left = { 28, 29, 30, 31 };
		static const std::vector<int> idx_list_fire_right = { 24, 25, 26, 27 };

		anim_idle_up.set_frame_data(tex_gunner, 4, 8, idx_list_idle_up);
		anim_idle_down.set_frame_data(tex_gunner, 4, 8, idx_list_idle_down);
		anim_idle_left.set_frame_data(tex_gunner, 4, 8, idx_list_idle_left);
		anim_idle_right.set_frame_data(tex_gunner, 4, 8, idx_list_idle_right);
		anim_fire_up.set_frame_data(tex_gunner, 4, 8, idx_list_fire_up);
		anim_fire_down.set_frame_data(tex_gunner, 4, 8, idx_list_fire_down);
		anim_fire_left.set_frame_data(tex_gunner, 4, 8, idx_list_fire_left);
		anim_fire_right.set_frame_data(tex_gunner, 4, 8, idx_list_fire_right);

		size.x = 48, size.y = 48;

		tower_type = TowerType::Gunner;

		fire_speed = 6;
		bullet_type = BulletType::Shell;
	}

	~GunnerTower() = default;

};


#endif // !_GUNNER_TOWER_H_


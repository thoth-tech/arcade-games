#ifndef _ARCHER_TOWER_H_
#define _ARCHER_TOWER_H_

#include "tower.h"
#include "resources_manager.h"


class ArcherTower : public Tower
{
public:
	ArcherTower()
	{
		static SDL_Texture* tex_archer = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_Archer)->second;

		static const std::vector<int> idx_list_idle_up = { 3, 4 };
		static const std::vector<int> idx_list_idle_down = { 0, 1 };
		static const std::vector<int> idx_list_idle_left = { 6, 7 };
		static const std::vector<int> idx_list_idle_right = { 9, 10 };
		static const std::vector<int> idx_list_fire_up = { 15, 16, 17 };
		static const std::vector<int> idx_list_fire_down = { 12, 13, 14 };
		static const std::vector<int> idx_list_fire_left = { 18, 19, 20 };
		static const std::vector<int> idx_list_fire_right = { 21, 22, 23 };

		anim_idle_up.set_frame_data(tex_archer, 3, 8, idx_list_idle_up);
		anim_idle_down.set_frame_data(tex_archer, 3, 8, idx_list_idle_down);
		anim_idle_left.set_frame_data(tex_archer, 3, 8, idx_list_idle_left);
		anim_idle_right.set_frame_data(tex_archer, 3, 8, idx_list_idle_right);
		anim_fire_up.set_frame_data(tex_archer, 3, 8, idx_list_fire_up);
		anim_fire_down.set_frame_data(tex_archer, 3, 8, idx_list_fire_down);
		anim_fire_left.set_frame_data(tex_archer, 3, 8, idx_list_fire_left);
		anim_fire_right.set_frame_data(tex_archer, 3, 8, idx_list_fire_right);

		size.x = 48, size.y = 48;

		tower_type = TowerType::Archer;

		fire_speed = 6;
		bullet_type = BulletType::Arrow;
	}

	~ArcherTower() = default;



};


#endif // !_ARCHER_TOWER_H_


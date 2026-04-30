#ifndef _ARROW_BULLET_H_
#define _ARROW_BULLET_H_

#include"bullet.h"
#include "resources_manager.h"


class ArrowBullet : public Bullet
{
public:
	ArrowBullet()
	{
		static SDL_Texture* tex_arrow = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_BulletArrow)->second;
		static const std::vector<int> idx_list = { 0,1 };
	
		animation.set_loop(true);
		animation.set_interval(0.1);
		animation.set_frame_data(tex_arrow, 2, 1, idx_list);
	
		can_rotated = true;
		size.x = 48, size.y = 48;
	}

	~ArrowBullet() = default;

	void on_collide(Enemy* enemy) override
	{
		static const ResourcesManager::SoundPool& sound_pool
			= ResourcesManager::instance()->get_sound_pool();
	
		switch(rand()%3)
		{
		case 0:
			Mix_PlayChannel(-1,sound_pool.find(ResID::Sound_ArrowHit_1)->second,0);
			break;
		case 1:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_2)->second, 0);
			break;
		case 2:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_3)->second, 0);
			break;
		default:
			break;
		}
		Bullet::on_collide(enemy);
	}

private:

};

#endif // !_ARROW_BULLET_H_


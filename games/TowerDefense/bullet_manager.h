#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

#include "bullet.h"
#include "manager.h"
#include "bullet_type.h"
#include "shell_bullet.h"
#include "arrow_bullet.h"
#include "axe_bullet.h"
#include <vector>

class BulletManager : public Manager<BulletManager>
{
	friend class Manager<BulletManager>;

public:
	typedef std::vector<Bullet*> BulletList;

public:
	void on_update(double delta)
	{
		for (Bullet* bullet : bullet_list)
			bullet->on_update(delta);

		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet)
			{
				bool deletable = bullet->can_remove();
				if (deletable) delete bullet;
				return deletable;
			}), bullet_list.end());
	}

	void on_render(SDL_Renderer* renderer)
	{
		for (Bullet* bullet : bullet_list)
			bullet->on_render(renderer);
	}

	BulletList& get_bullet_list()
	{
		return bullet_list;
	}

	void fire_bullet(BulletType type, const Vector2& position, const Vector2& velocity, double damage)
	{
		Bullet* bullet = nullptr;

		switch (type)
		{
		case Arrow:
			bullet = new ArrowBullet();
			break;
		case Axe:
			bullet = new AxeBullet();
			break;
		case Shell:
			bullet = new ShellBullet();
			break;
		default:
			bullet = new ArrowBullet();
			break;
		}

		bullet->set_position(position);
		bullet->set_velocity(velocity);
		bullet->set_damage(damage);

		bullet_list.push_back(bullet);
	}

protected:
	BulletManager() = default;

	~BulletManager()
	{
		for (Bullet* bullet : bullet_list)
			delete bullet;
	}

private:
	BulletList bullet_list;

};

#endif // !_BULLET_MANAGER_H_

#ifndef _PLACE_PANEL_H_
#define _PLACE_PANEL_H_

#include "panel.h"
#include "coin_manager.h"
#include "tower_manager.h"
#include "resources_manager.h"

#include <SDL2_gfxPrimitives.h>

class PlacePanel : public Panel
{
public:
	PlacePanel()
	{
		const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();

		tex_idle = texture_pool.find(ResID::Tex_UIPlaceIdle)->second;
		tex_hovered_top = texture_pool.find(ResID::Tex_UIPlaceHoveredTop)->second;
		tex_hovered_left = texture_pool.find(ResID::Tex_UIPlaceHoveredLeft)->second;
		tex_hovered_right = texture_pool.find(ResID::Tex_UIPlaceHoveredRight)->second;
	}

	~PlacePanel() = default;

	void on_update(SDL_Renderer* renderer) override
	{
		static TowerManager* instance = TowerManager::instance();

		val_top = (int)instance->get_place_cost(TowerType::Axeman);
		val_left = (int)instance->get_place_cost(TowerType::Archer);
		val_right = (int)instance->get_place_cost(TowerType::Gunner);

		reg_top = (int)instance->get_damage_range(TowerType::Axeman) * SIZE_TILE;
		reg_left = (int)instance->get_damage_range(TowerType::Archer) * SIZE_TILE;
		reg_right = (int)instance->get_damage_range(TowerType::Gunner) * SIZE_TILE;

		Panel::on_update(renderer);
	}

	void on_render(SDL_Renderer* renderer) override
	{
		if (!visible) return;

		int reg = 0;
		switch (hovered_target)
		{
		case Panel::HoveredTarget::Top:
			reg = reg_top;
			break;
		case Panel::HoveredTarget::Left:
			reg = reg_left;
			break;
		case Panel::HoveredTarget::Right:
			reg = reg_right;
			break;
		}

		if (reg > 0)
		{
			filledCircleRGBA(renderer, center_pos.x, center_pos.y, reg,
				color_region_content.r, color_region_content.g, color_region_content.b, color_region_content.a);
			aacircleRGBA(renderer, center_pos.x, center_pos.y, reg,
				color_region_frame.r, color_region_frame.g, color_region_frame.b, color_region_frame.a);
		}

		Panel::on_render(renderer);
	}

protected:
	void on_click_top_area() override
	{
		CoinManager* instance = CoinManager::instance();

		if (val_top <= instance->get_current_coin_num())
		{
			TowerManager::instance()->place_tower(TowerType::Axeman, idx_tile_selected);
			instance->decrease_coin(val_top);
		}
	}

	void on_click_left_area() override
	{
		CoinManager* instance = CoinManager::instance();

		if (val_left <= instance->get_current_coin_num())
		{
			TowerManager::instance()->place_tower(TowerType::Archer, idx_tile_selected);
			instance->decrease_coin(val_left);
		}
	}

	void on_click_right_area() override
	{
		CoinManager* instance = CoinManager::instance();

		if (val_right <= instance->get_current_coin_num())
		{
			TowerManager::instance()->place_tower(TowerType::Gunner, idx_tile_selected);
			instance->decrease_coin(val_right);
		}
	}

private:
	const SDL_Color color_region_frame = { 30, 80, 162, 175 };
	const SDL_Color color_region_content = { 0, 149, 217, 75 };

private:
	int reg_top = 0, reg_left = 0, reg_right = 0;

};

#endif // !_PLACE_PANEL_H_

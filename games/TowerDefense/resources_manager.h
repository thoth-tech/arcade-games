#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include "manager.h"

#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <unordered_map>

enum class ResID
{
	Tex_Tileset,

	Tex_Player,
	Tex_Archer,
	Tex_Axeman,
	Tex_Gunner,

	Tex_Slime,
	Tex_KingSlime,
	Tex_Skeleton,
	Tex_Goblin,
	Tex_GoblinPriest,
	Tex_SlimeSketch,
	Tex_KingSlimeSketch,
	Tex_SkeletonSketch,
	Tex_GoblinSketch,
	Tex_GoblinPriestSketch,

	Tex_BulletArrow,
	Tex_BulletAxe,
	Tex_BulletShell,

	Tex_Coin,
	Tex_Home,

	Tex_EffectFlash_Up,
	Tex_EffectFlash_Down,
	Tex_EffectFlash_Left,
	Tex_EffectFlash_Right,
	Tex_EffectImpact_Up,
	Tex_EffectImpact_Down,
	Tex_EffectImpact_Left,
	Tex_EffectImpact_Right,
	Tex_EffectExplode,

	Tex_UISelectCursor,
	Tex_UIPlaceIdle,
	Tex_UIPlaceHoveredTop,
	Tex_UIPlaceHoveredLeft,
	Tex_UIPlaceHoveredRight,
	Tex_UIUpgradeIdle,
	Tex_UIUpgradeHoveredTop,
	Tex_UIUpgradeHoveredLeft,
	Tex_UIUpgradeHoveredRight,
	Tex_UIHomeAvatar,
	Tex_UIPlayerAvatar,
	Tex_UIHeart,
	Tex_UICoin,
	Tex_UIGameOverBar,
	Tex_UIWinText,
	Tex_UILossText,

	Sound_ArrowFire_1,
	Sound_ArrowFire_2,
	Sound_AxeFire,
	Sound_ShellFire,
	Sound_ArrowHit_1,
	Sound_ArrowHit_2,
	Sound_ArrowHit_3,
	Sound_AxeHit_1,
	Sound_AxeHit_2,
	Sound_AxeHit_3,
	Sound_ShellHit,

	Sound_Flash,
	Sound_Impact,

	Sound_Coin,
	Sound_HomeHurt,
	Sound_PlaceTower,
	Sound_TowerLevelUp,

	Sound_Win,
	Sound_Loss,

	Music_BGM,

	Font_Main
};

class ResourcesManager : public Manager<ResourcesManager>
{
	friend class Manager<ResourcesManager>;

public:
	typedef std::unordered_map<ResID, TTF_Font*> FontPool;
	typedef std::unordered_map<ResID, Mix_Chunk*> SoundPool;
	typedef std::unordered_map<ResID, Mix_Music*> MusicPool;
	typedef std::unordered_map<ResID, SDL_Texture*> TexturePool;

public:
	bool load_from_file(SDL_Renderer* renderer)
	{
		texture_pool[ResID::Tex_Tileset] = IMG_LoadTexture(renderer, "resources/tileset.png");

		texture_pool[ResID::Tex_Player] = IMG_LoadTexture(renderer, "resources/player.png");
		texture_pool[ResID::Tex_Archer] = IMG_LoadTexture(renderer, "resources/tower_archer.png");
		texture_pool[ResID::Tex_Axeman] = IMG_LoadTexture(renderer, "resources/tower_axeman.png");
		texture_pool[ResID::Tex_Gunner] = IMG_LoadTexture(renderer, "resources/tower_gunner.png");

		texture_pool[ResID::Tex_Slime] = IMG_LoadTexture(renderer, "resources/enemy_slime.png");
		texture_pool[ResID::Tex_KingSlime] = IMG_LoadTexture(renderer, "resources/enemy_king_slime.png");
		texture_pool[ResID::Tex_Skeleton] = IMG_LoadTexture(renderer, "resources/enemy_skeleton.png");
		texture_pool[ResID::Tex_Goblin] = IMG_LoadTexture(renderer, "resources/enemy_goblin.png");
		texture_pool[ResID::Tex_GoblinPriest] = IMG_LoadTexture(renderer, "resources/enemy_goblin_priest.png");
		texture_pool[ResID::Tex_SlimeSketch] = IMG_LoadTexture(renderer, "resources/enemy_slime_sketch.png");
		texture_pool[ResID::Tex_KingSlimeSketch] = IMG_LoadTexture(renderer, "resources/enemy_king_slime_sketch.png");
		texture_pool[ResID::Tex_SkeletonSketch] = IMG_LoadTexture(renderer, "resources/enemy_skeleton_sketch.png");
		texture_pool[ResID::Tex_GoblinSketch] = IMG_LoadTexture(renderer, "resources/enemy_goblin_sketch.png");
		texture_pool[ResID::Tex_GoblinPriestSketch] = IMG_LoadTexture(renderer, "resources/enemy_goblin_priest_sketch.png");

		texture_pool[ResID::Tex_BulletArrow] = IMG_LoadTexture(renderer, "resources/bullet_arrow.png");
		texture_pool[ResID::Tex_BulletAxe] = IMG_LoadTexture(renderer, "resources/bullet_axe.png");
		texture_pool[ResID::Tex_BulletShell] = IMG_LoadTexture(renderer, "resources/bullet_shell.png");

		texture_pool[ResID::Tex_Coin] = IMG_LoadTexture(renderer, "resources/coin.png");
		texture_pool[ResID::Tex_Home] = IMG_LoadTexture(renderer, "resources/home.png");

		texture_pool[ResID::Tex_EffectFlash_Up] = IMG_LoadTexture(renderer, "resources/effect_flash_up.png");
		texture_pool[ResID::Tex_EffectFlash_Down] = IMG_LoadTexture(renderer, "resources/effect_flash_down.png");
		texture_pool[ResID::Tex_EffectFlash_Left] = IMG_LoadTexture(renderer, "resources/effect_flash_left.png");
		texture_pool[ResID::Tex_EffectFlash_Right] = IMG_LoadTexture(renderer, "resources/effect_flash_right.png");
		texture_pool[ResID::Tex_EffectImpact_Up] = IMG_LoadTexture(renderer, "resources/effect_impact_up.png");
		texture_pool[ResID::Tex_EffectImpact_Down] = IMG_LoadTexture(renderer, "resources/effect_impact_down.png");
		texture_pool[ResID::Tex_EffectImpact_Left] = IMG_LoadTexture(renderer, "resources/effect_impact_left.png");
		texture_pool[ResID::Tex_EffectImpact_Right] = IMG_LoadTexture(renderer, "resources/effect_impact_right.png");
		texture_pool[ResID::Tex_EffectExplode] = IMG_LoadTexture(renderer, "resources/effect_explode.png");

		texture_pool[ResID::Tex_UISelectCursor] = IMG_LoadTexture(renderer, "resources/ui_select_cursor.png");
		texture_pool[ResID::Tex_UIPlaceIdle] = IMG_LoadTexture(renderer, "resources/ui_place_idle.png");
		texture_pool[ResID::Tex_UIPlaceHoveredTop] = IMG_LoadTexture(renderer, "resources/ui_place_hovered_top.png");
		texture_pool[ResID::Tex_UIPlaceHoveredLeft] = IMG_LoadTexture(renderer, "resources/ui_place_hovered_left.png");
		texture_pool[ResID::Tex_UIPlaceHoveredRight] = IMG_LoadTexture(renderer, "resources/ui_place_hovered_right.png");
		texture_pool[ResID::Tex_UIUpgradeIdle] = IMG_LoadTexture(renderer, "resources/ui_upgrade_idle.png");
		texture_pool[ResID::Tex_UIUpgradeHoveredTop] = IMG_LoadTexture(renderer, "resources/ui_upgrade_hovered_top.png");
		texture_pool[ResID::Tex_UIUpgradeHoveredLeft] = IMG_LoadTexture(renderer, "resources/ui_upgrade_hovered_left.png");
		texture_pool[ResID::Tex_UIUpgradeHoveredRight] = IMG_LoadTexture(renderer, "resources/ui_upgrade_hovered_right.png");
		texture_pool[ResID::Tex_UIHomeAvatar] = IMG_LoadTexture(renderer, "resources/ui_home_avatar.png");
		texture_pool[ResID::Tex_UIPlayerAvatar] = IMG_LoadTexture(renderer, "resources/ui_player_avatar.png");
		texture_pool[ResID::Tex_UIHeart] = IMG_LoadTexture(renderer, "resources/ui_heart.png");
		texture_pool[ResID::Tex_UICoin] = IMG_LoadTexture(renderer, "resources/ui_coin.png");
		texture_pool[ResID::Tex_UIGameOverBar] = IMG_LoadTexture(renderer, "resources/ui_game_over_bar.png");
		texture_pool[ResID::Tex_UIWinText] = IMG_LoadTexture(renderer, "resources/ui_win_text.png");
		texture_pool[ResID::Tex_UILossText] = IMG_LoadTexture(renderer, "resources/ui_loss_text.png");

		for (const auto& pair : texture_pool)
			if (!pair.second) return false;

		sound_pool[ResID::Sound_ArrowFire_1] = Mix_LoadWAV("resources/sound_arrow_fire_1.mp3");
		sound_pool[ResID::Sound_ArrowFire_2] = Mix_LoadWAV("resources/sound_arrow_fire_2.mp3");
		sound_pool[ResID::Sound_AxeFire] = Mix_LoadWAV("resources/sound_axe_fire.wav");
		sound_pool[ResID::Sound_ShellFire] = Mix_LoadWAV("resources/sound_shell_fire.wav");
		sound_pool[ResID::Sound_ArrowHit_1] = Mix_LoadWAV("resources/sound_arrow_hit_1.mp3");
		sound_pool[ResID::Sound_ArrowHit_2] = Mix_LoadWAV("resources/sound_arrow_hit_2.mp3");
		sound_pool[ResID::Sound_ArrowHit_3] = Mix_LoadWAV("resources/sound_arrow_hit_3.mp3");
		sound_pool[ResID::Sound_AxeHit_1] = Mix_LoadWAV("resources/sound_axe_hit_1.mp3");
		sound_pool[ResID::Sound_AxeHit_2] = Mix_LoadWAV("resources/sound_axe_hit_2.mp3");
		sound_pool[ResID::Sound_AxeHit_3] = Mix_LoadWAV("resources/sound_axe_hit_3.mp3");
		sound_pool[ResID::Sound_ShellHit] = Mix_LoadWAV("resources/sound_shell_hit.mp3");

		sound_pool[ResID::Sound_Flash] = Mix_LoadWAV("resources/sound_flash.wav");
		sound_pool[ResID::Sound_Impact] = Mix_LoadWAV("resources/sound_impact.wav");

		sound_pool[ResID::Sound_Coin] = Mix_LoadWAV("resources/sound_coin.mp3");
		sound_pool[ResID::Sound_HomeHurt] = Mix_LoadWAV("resources/sound_home_hurt.wav");
		sound_pool[ResID::Sound_PlaceTower] = Mix_LoadWAV("resources/sound_place_tower.mp3");
		sound_pool[ResID::Sound_TowerLevelUp] = Mix_LoadWAV("resources/sound_tower_level_up.mp3");

		sound_pool[ResID::Sound_Win] = Mix_LoadWAV("resources/sound_win.wav");
		sound_pool[ResID::Sound_Loss] = Mix_LoadWAV("resources/sound_loss.mp3");

		for (const auto& pair : sound_pool)
			if (!pair.second) return false;

		music_pool[ResID::Music_BGM] = Mix_LoadMUS("resources/music_bgm.mp3");

		for (const auto& pair : music_pool)
			if (!pair.second) return false;

		font_pool[ResID::Font_Main] = TTF_OpenFont("resources/ipix.ttf", 25);

		for (const auto& pair : font_pool)
			if (!pair.second) return false;

		return true;
	}

	const FontPool& get_font_pool()
	{
		return font_pool;
	}

	const SoundPool& get_sound_pool()
	{
		return sound_pool;
	}

	const MusicPool& get_music_pool()
	{
		return music_pool;
	}

	const TexturePool& get_texture_pool()
	{
		return texture_pool;
	}

protected:
	ResourcesManager() = default;
	~ResourcesManager() = default;

private:
	FontPool font_pool;
	SoundPool sound_pool;
	MusicPool music_pool;
	TexturePool texture_pool;

};


#endif // !_RESOURCES_MANAGER_H_

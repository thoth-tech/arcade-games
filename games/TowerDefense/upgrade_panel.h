#ifndef _UPGRADE_PANEL_H_
#define _UPGRADE_PANEL_H_

#include "panel.h"
#include "tower_manager.h"
#include "resources_manager.h"
#include "coin_manager.h"           // 需要拿到当前金币
#include "config_manager.h"         // 只为打印当前等级时读取 level_*

class UpgradePanel : public Panel
{
public:
    UpgradePanel()
    {
        const ResourcesManager::TexturePool& texture_pool
            = ResourcesManager::instance()->get_texture_pool();

        tex_idle = texture_pool.find(ResID::Tex_UIUpgradeIdle)->second;
        tex_hovered_top = texture_pool.find(ResID::Tex_UIUpgradeHoveredTop)->second;
        tex_hovered_left = texture_pool.find(ResID::Tex_UIUpgradeHoveredLeft)->second;
        tex_hovered_right = texture_pool.find(ResID::Tex_UIUpgradeHoveredRight)->second;
    }

    ~UpgradePanel() = default;

    void on_update(SDL_Renderer* renderer)
    {
        // 读取当前花费
        static TowerManager* tm = TowerManager::instance();
        const int new_top = (int)tm->get_upgrade_cost(TowerType::Axeman);
        const int new_left = (int)tm->get_upgrade_cost(TowerType::Archer);
        const int new_right = (int)tm->get_upgrade_cost(TowerType::Gunner);

        // 读取当前金币
        const int new_coin = CoinManager::instance()->get_current_coin_num();

        // 读取当前等级（仅用于打印，方便观察是否随升级变化）
        ConfigManager* cfg = ConfigManager::instance();
        const int lvlA = cfg->level_archer;
        const int lvlX = cfg->level_axeman;
        const int lvlG = cfg->level_gunner;

        // —— 节流打印：仅当数值变化 且 距上次打印超过最小间隔 才打印 —— //
        const Uint32 now = SDL_GetTicks();
        const Uint32 MIN_LOG_INTERVAL_MS = 250; // 最小打印间隔 250ms（可按需调整）

        const bool changed =
            (new_top != prev_top) ||
            (new_left != prev_left) ||
            (new_right != prev_right) ||
            (new_coin != prev_coin) ||
            (lvlA != prev_lvlA) || (lvlX != prev_lvlX) || (lvlG != prev_lvlG);

        if (changed && (now - last_log_ms >= MIN_LOG_INTERVAL_MS))
        {
            SDL_Log("[UPGRADE_PANEL] cost Axeman(top)=%d  Archer(left)=%d  Gunner(right)=%d  coin=%d  levels(A:%d X:%d G:%d)",
                new_top, new_left, new_right, new_coin, lvlA, lvlX, lvlG);

            prev_top = new_top;
            prev_left = new_left;
            prev_right = new_right;
            prev_coin = new_coin;
            prev_lvlA = lvlA;
            prev_lvlX = lvlX;
            prev_lvlG = lvlG;
            last_log_ms = now;
        }

        // 继续走你原来的 UI 更新逻辑
        val_top = new_top;
        val_left = new_left;
        val_right = new_right;

        Panel::on_update(renderer);
    }

protected:
    void on_click_top_area() override
    {
        CoinManager* cm = CoinManager::instance();
        const int coin = cm->get_current_coin_num();

        SDL_Log("[UPGRADE_PANEL] click TOP (Axeman): need=%d  coin=%d", val_top, coin);

        if (val_top > 0 && val_top <= coin)
        {
            TowerManager::instance()->upgrade_tower(TowerType::Axeman);
            cm->decrease_coin(val_top);
        }
        else
        {
            SDL_Log("[UPGRADE_PANEL] click TOP ignored: not enough coins or invalid cost.");
        }
    }

    void on_click_left_area() override
    {
        CoinManager* cm = CoinManager::instance();
        const int coin = cm->get_current_coin_num();

        SDL_Log("[UPGRADE_PANEL] click LEFT (Archer): need=%d  coin=%d", val_left, coin);

        if (val_left > 0 && val_left <= coin)
        {
            TowerManager::instance()->upgrade_tower(TowerType::Archer);
            cm->decrease_coin(val_left);
        }
        else
        {
            SDL_Log("[UPGRADE_PANEL] click LEFT ignored: not enough coins or invalid cost.");
        }
    }

    void on_click_right_area() override
    {
        CoinManager* cm = CoinManager::instance();
        const int coin = cm->get_current_coin_num();

        SDL_Log("[UPGRADE_PANEL] click RIGHT (Gunner): need=%d  coin=%d", val_right, coin);

        if (val_right > 0 && val_right <= coin)
        {
            TowerManager::instance()->upgrade_tower(TowerType::Gunner);
            cm->decrease_coin(val_right);
        }
        else
        {
            SDL_Log("[UPGRADE_PANEL] click RIGHT ignored: not enough coins or invalid cost.");
        }
    }

private:
    // —— 仅用于“是否变化”的轻量缓存，避免刷屏 —— //
    int prev_top = INT_MIN;
    int prev_left = INT_MIN;
    int prev_right = INT_MIN;
    int prev_coin = INT_MIN;
    int prev_lvlA = INT_MIN;
    int prev_lvlX = INT_MIN;
    int prev_lvlG = INT_MIN;
    Uint32 last_log_ms = 0;
};

#endif // !_UPGRADE_PANEL_H_

#ifndef HALLWAYLEVEL_H
#define HALLWAYLEVEL_H

#include "splashkit.h"
#include "player.h"
#include "interactable.h"
#include <vector>

class HallwayLevel {
private:
    bitmap background;
    float original_width, original_height;
    float scaled_width, scaled_height;
    float scale_factor;
    float y_offset;                    // vertical centering offset

    float world_left_boundary;         // left wall X
    float world_right_boundary;        // right wall X
    float ground_level;                // floor Y
    float ceiling_level;               // ceiling Y

    float image_offset_x;              // where the image's left edge sits relative to world_left

    std::vector<Interactable> interactables;
    Interactable* nearby_interactable;
    bool show_boundaries;

public:
    HallwayLevel();

    void load();
    void unload();
    void update(Player& player);
    void draw(const point_2d& camera_pos) const;

    void check_boundaries(Player& player);
    void check_interactions(const Player& player);

    float get_ground_level() const { return ground_level; }
    float get_y_offset() const { return y_offset; }
    float get_world_left_boundary() const { return world_left_boundary; }
    float get_world_right_boundary() const { return world_right_boundary; }

    Interactable* get_nearby_interactable() const { return nearby_interactable; }
    const std::vector<Interactable>& get_interactables() const { return interactables; }
    const bitmap& get_background() const { return background; }

    void set_show_boundaries(bool show) { show_boundaries = show; }
    bool get_show_boundaries() const { return show_boundaries; }

    void setup_interactables();
    void trigger_interaction(const std::string& id);

private:
    void draw_boundaries(const point_2d& camera_pos) const;
};

#endif
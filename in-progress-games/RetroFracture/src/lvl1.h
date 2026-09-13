#ifndef LVL1_H
#define LVL1_H

#include "splashkit.h"
#include "player.h"
#include "interactable.h"
#include <vector>

/**
 * A thin vertical wall obstacle.
 * - x: world x coordinate of the wall centre
 * - y_top, y_bottom: vertical range (y_top is the top, y_bottom is the bottom; note y increases downward)
 * - half_width: half thickness (default 5 pixels)
 */
struct WallSegment
{
    float x;
    float y_top;
    float y_bottom;
    float half_width;
    WallSegment(float x, float y_top, float y_bottom, float hw = 5.0f)
        : x(x), y_top(y_top), y_bottom(y_bottom), half_width(hw) {}
};

class Level1 {
private:
    bitmap background;
    float original_width, original_height;
    float scaled_width, scaled_height;
    float scale_factor;
    float y_offset;

    float world_left_boundary;
    float world_right_boundary;
    float ground_level;              // representative floor height (for camera)
    float ceiling_level;

    float image_offset_x;

    // --- NEW: polyline ground data ---
    std::vector<point_2d> ground_path;   // sorted by x, no duplicate x
    float global_ground_level;           // fallback when x is outside the path
    float get_ground_y(float x) const;   // returns floor y at any x

    // --- NEW: vertical interior walls ---
    std::vector<WallSegment> wall_segments;

    Interactable exit_door;
    bool door_triggered;
    bool show_boundaries;

public:
    Level1();

    void load();
    void unload();

    void update(Player& player);
    void draw(const Player& player, const point_2d& camera_pos) const;

    void check_boundaries(Player& player);
    void check_interactions(const Player& player);

    float get_ground_level() const { return ground_level; }
    float get_y_offset() const { return y_offset; }
    float get_world_left_boundary() const { return world_left_boundary; }
    float get_world_right_boundary() const { return world_right_boundary; }

    bool is_door_triggered() const { return door_triggered; }
    void reset_door() { door_triggered = false; }

    void set_show_boundaries(bool show) { show_boundaries = show; }
    bool get_show_boundaries() const { return show_boundaries; }

private:
    void draw_boundaries(const point_2d& camera_pos) const;
};

#endif
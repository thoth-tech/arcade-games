#ifndef HUD_H
#define HUD_H

#include "splashkit.h"
#include "player.h"

/**
 * Heads-Up Display class
 * Manages rendering of UI elements with level-appropriate styling
 */
class HUD 
{
private:
    color hallway_text_color;    // Text color for hallway level
    color apartment_text_color;  // Text color for apartment level

public:
    HUD();  // Constructor
    
    // HUD rendering methods
    void draw_controls(bool in_hallway) const;                              // Draw control instructions
    void draw_debug_info(const Player& player, const point_2d& camera_pos, bool in_hallway) const; // Draw debug info
    void draw_level_info(const std::string& level_name, bool in_hallway) const; // Draw level name
    void draw_interaction_prompt(const std::string& prompt, bool in_hallway) const; // Draw interaction prompt
    
    // Color setters
    void set_hallway_text_color(color c) { hallway_text_color = c; }
    void set_apartment_text_color(color c) { apartment_text_color = c; }
    
    // Color getter based on current level
    color get_text_color(bool in_hallway) const;
};

#endif
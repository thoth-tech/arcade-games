#include "hud.h"
#include <iostream>

/**
 * HUD constructor
 * Initializes color scheme for different levels
 */
HUD::HUD() 
{
    // Set default colors for text readability per level
    hallway_text_color = COLOR_YELLOW;                       // Bright for dark hallway
    apartment_text_color = COLOR_GREEN;     // Dark for light apartment
}

/**
 * Get appropriate text color for current level
 * @param in_hallway Whether player is in hallway level
 * @return Color for text rendering
 */
color HUD::get_text_color(bool in_hallway) const 
{
    return in_hallway ? hallway_text_color : apartment_text_color;
}

/**
 * Draw control instructions
 * @param in_hallway Whether player is in hallway level
 */
void HUD::draw_controls(bool in_hallway) const 
{
    color text_color = get_text_color(in_hallway);
    
    if (in_hallway) 
    {
        draw_text("CONTROLS: A (Left), D (Right), W (Jump), E (Interact)", text_color, 10, 10);
        draw_text("ESC to quit | R to reset", text_color, 10, 35);
    } 
    else 
    {
        draw_text("CONTROLS: A (Left), D (Right), W (Jump), E (Interact)", text_color, 10, 10);
        draw_text("ESC to quit | R to reset", text_color, 10, 35);
    }
}

/**
 * Draw debug information
 * @param player Player for position data
 * @param camera_pos Camera position
 * @param in_hallway Whether player is in hallway level
 */
void HUD::draw_debug_info(const Player& player, const point_2d& camera_pos, bool in_hallway) const 
{
    color text_color = get_text_color(in_hallway);
    point_2d player_pos = player.get_position();
    
    // Player world position
    std::string world_text = "WORLD: (" + std::to_string((int)player_pos.x) + 
                            ", " + std::to_string((int)player_pos.y) + ")";
    draw_text(world_text, text_color, 10, 65);
    
    // Camera position
    std::string camera_text = "CAMERA: (" + std::to_string((int)camera_pos.x) + 
                             ", " + std::to_string((int)camera_pos.y) + ")";
    draw_text(camera_text, text_color, 10, 90);
}

/**
 * Draw current level name
 * @param level_name Name of current level
 * @param in_hallway Whether player is in hallway level
 */
void HUD::draw_level_info(const std::string& level_name, bool in_hallway) const 
{
    color text_color = get_text_color(in_hallway);
    std::string level_text = "LEVEL: " + level_name;
    draw_text(level_text, text_color, 10, 115);
}

/**
 * Draw interaction prompt if available
 * @param prompt Text to display as prompt
 * @param in_hallway Whether player is in hallway level
 */
void HUD::draw_interaction_prompt(const std::string& prompt, bool in_hallway) const 
{
    if (!prompt.empty()) 
    {
        // Use different colors based on level for better visibility
        color prompt_color = in_hallway ? COLOR_CYAN : rgba_color(0, 150, 200, 255);
        
        // Position near bottom center with a background for readability
        // Estimate text width - using fixed approximation since text_width might not be available
        int text_width_estimate = prompt.length() * 8;  // Approximate: 8 pixels per character
        int x_pos = (800 - text_width_estimate) / 2;    // Center horizontally
        int y_pos = 570;                                // Near bottom
        
        // Draw with a slight shadow for better readability
        draw_text(prompt, rgba_color(0, 0, 0, 200), x_pos + 1, y_pos + 1);
        draw_text(prompt, prompt_color, x_pos, y_pos);
    }
}
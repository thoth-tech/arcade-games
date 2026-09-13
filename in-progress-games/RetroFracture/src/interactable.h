#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "splashkit.h"
#include <string>

/**
 * Interactable object class
 * Represents objects that player can interact with (doors, switches, etc.)
 */
class Interactable 
{
private:
    std::string id;         // Unique identifier
    float x, y;            // World position
    float width, height;   // Collision dimensions
    bool is_active;        // Whether object is active
    bool is_triggered;     // Whether interaction has occurred
    std::string label;     // Display label

public:
    // Constructor
    Interactable(std::string id, float x, float y, float width, float height, std::string label = "");
    
    // Rendering and update
    void draw(point_2d camera_pos, float background_y_offset) const; // Draw object
    void update();                                                   // Update object state
    
    // Collision detection
    bool check_collision(point_2d player_pos) const;                 // Check player proximity
    
    // Interaction handling
    void trigger();      // Trigger interaction
    void reset();        // Reset to initial state
    
    // Getters
    std::string get_id() const { return id; }
    bool get_is_active() const { return is_active; }
    bool get_is_triggered() const { return is_triggered; }
    std::string get_label() const { return label; }
};

#endif
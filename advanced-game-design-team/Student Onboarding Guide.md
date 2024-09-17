# Splashkit Gamedev Cheat Sheet

---

Heeeeyyyyy. This guide will help you get started with game development using SplashKit's API. 

It's important to follow the patterns here to the best of your ability throughout your completion of the capstone units:

- It means your implementations will be consistent with other projects this team has worked / is working on, which will help you and your peers achieve more across more projects, and in less time.

- It will help you reason about and debug your code far more easily, so you can spend more time having fun and less time confused and angry.

- They're simple rules which are broadly good practice across the majority of software projects.

---

## Introduction

**Let's define some concepts that this document emphasises:**

### State

**State** refers to all the variable values that represent your game's current condition. This includes positions of players and enemies, health levels, scores, game level, and any other data that changes as the game progresses.

### Rendering

**Rendering** is the process of drawing the current state on the screen. Rendering functions take the state and use it to produce visual representations. For example, if your state includes player coordinates in a normalized space (e.g., a range from 0 to 1), the rendering function can map these coordinates to actual screen positions. This makes handling resolution changes and different screen sizes easier.

### Control Flow

**Control Flow** dictates how your program executes, managing the sequence and timing of state updates and rendering. Control flow interacts with state by updating it based on player input or game events, and it interacts with rendering by calling rendering functions to update the visuals.

---

## 1. Separate State and Render Concerns

**Principle**: Keep state logic and rendering logic separate.

**Why?** This makes the code easier to manage and debug. When state and rendering logic are mixed, it becomes hard to track bugs and make changes. 

Here are some examples:

**Debugging Complexity:**

If your rendering code modifies the game state, you might encounter visual bugs that are difficult to diagnose. For example, an animation function that inadvertently changes player coordinates can cause unpredictable visual outcomes. 

*Example:*

```cpp
// Bad: Rendering logic mixed with game state updates in a larger context
struct Game {
    Player player;
    Enemy enemy;
};

struct Enemy {
    int x, y;
    bool alive;
};

void render_and_update(Game &game) {
    // Render player and modify state
    draw_player(game.player.x, game.player.y);
    game.player.x += 5; // This can cause game logic issues

    // Render enemy and modify state
    draw_enemy(game.enemy.x, game.enemy.y);
    game.enemy.alive = false; // This can cause game logic issues
}
```

**Issue**: In the above example, modifying the player and enemy state during rendering can lead to unexpected behaviour, such as player and enemy positions changing erratically or enemies not behaving as expected in subsequent frames.

**Code Maintainability:**

Mixing state and rendering logic makes the codebase harder to maintain. Changes in game state management or rendering can have unintended side effects on each other.

*Bad example:*

```cpp
struct Enemy {
    float x, y;
    int health;
    bool active;
};

void update_and_render_enemy(Enemy& e, float delta_time) {
    // Update enemy state
    e.x += 50.0f * delta_time;
    if (e.health <= 0) {
        e.active = false;
    }

    // Render enemy
    if (e.active) {
        draw_sprite("enemy", e.x, e.y);
        draw_health_bar(e.x, e.y - 10, e.health);
    }
}
```

**Issues:**

- Besides anything else, the `update_and_render_enemy()` function is doing too much and violates the [Single-responsibility principle](https://en.wikipedia.org/wiki/Single-responsibility_principle).

- To change state or render logic we have to modify the same function which can lead to side effects across both areas.

- It's harder to test either pattern in isolation.

*Good example:*

```cpp
struct Enemy {
    float x, y;
    int health;
    bool active;
};

void update_enemy(Enemy& e, float delta_time) {
    if (!e.active) return;

    e.x += 50.0f * delta_time;
    if (e.health <= 0) {
        e.active = false;
    }
}

void render_enemy(const Enemy& e) {
    if (!e.active) return;

    draw_sprite("enemy", e.x, e.y);
    draw_health_bar(e.x, e.y - 10, e.health);
}
```

**Benefits:**

- Clear separation of concerns: `update_enemy()` handles state changes, while `render_enemy()` handles drawing.

- Easier to maintain: You can modify the movement logic without touching the rendering code, and vice versa.

- Improved testability: You can unit test the update logic (you won't) without worrying about rendering.

- More flexible: You can easily change the order of updates and renders, or add additional processing steps between them.

- Better performance potential: You can optimize updates and renders separately, and potentially parallelise them.

Let's look at a scenario where we want to extend the `Enemy` type's functionality by giving them a boolean `stunned` property:

```cpp
struct Enemy {
    float x, y;
    int health;
    bool active;
    bool stunned;
    float stun_duration;
};

void update_enemy(Enemy& e, float delta_time) {
    if (!e.active) return;

    if (e.stunned) {
        e.stun_duration -= delta_time;
        if (e.stun_duration <= 0) {
            e.stunned = false;
        }
        return;  // Don't move while stunned
    }

    e.x += 50.0f * delta_time;
    if (e.health <= 0) {
        e.active = false;
    }
}

void render_enemy(const Enemy& e) {
    if (!e.active) return;

    draw_sprite("enemy", e.x, e.y);
    draw_health_bar(e.x, e.y - 10, e.health);

    if (e.stunned) {
        draw_sprite("stun_effect", e.x, e.y);
    }
}
```

We've added the stunned state to the enemy struct and updated both the `update_enemy()` and `render_enemy()` functions independently. This demonstrates how separating state and rendering makes it easier to add new features and modify existing behaviour without risking unintended side effects or having to modify our higher order control flow.

**Update and Render Order:**

Separating update and render logic allows for flexible control over execution order. This separation is crucial for proper draw ordering and performance optimisation.

*Good example:*

```cpp
struct Game {
    Player player;
    Enemy enemy;
    Background background;
};

void update_game(Game &game) {
    update_player(game.player);
    update_enemy(game.enemy); // Enemy update depends on player being updated first
}

void render_game(const Game &game) {
    draw_background(game.background); // Correct order, background first
    draw_enemy(game.enemy);
    draw_player(game.player); // Player drawn after enemies as enemies should not occlude player 
}

void game_loop(Game &game) {
    update_game(game);
    render_game(game);
}
```

**Benefits**:

- Flexible control over update and render order.
- Correct layering of game elements.
- Potential for performance optimizations (e.g., parallel updates).
- Facilitates pattern where we want to update the player before updating enemies, but draw the player *after* we draw enemies.

---

## 2. Avoid Mutating Global State

**Why?** Modifying global variables can lead to:

- Unpredictable behaviour
- Difficult-to-trace bugs
- Reduced code modularity
- Complications in multi-threaded environments

**How to manage state effectively**

- **Use Local Variables**: Keep variables scoped to where they're needed.
- **Pass Parameters**: Explicitly pass state to functions that need it.
- **Use Structs for Game State**: Encapsulate related state in structs and pass them around.

*Example - Avoiding Global State:*

```cpp
// Avoid this
int global_score = 0;

void update_score() {
    global_score++;  // Modifies global state
}

// Do this instead
struct GameState {
    int score;
};

void update_score(GameState& state) {
    state.score++;  // Modifies encapsulated state
}

// Usage
GameState game_state = {0};  // Initialize score to 0
update_score(game_state);
```

**Global Constants:** - while mutable global state should be avoided, global constants can be beneficial:

- **Immutability**: Constants cannot be changed, avoiding the pitfalls of mutable global state.

- **Centralized Configuration**: Useful for game-wide settings.

- **Performance**: No runtime cost for passing values.

- **Readability**: Can make code more self-documenting.

*Example: Effective Use of Global Constants:*

```cpp
// Global constants (use all caps by convention)
const float PLAYER_SPEED = 5.0f;
const int MAX_ENEMIES = 10;
const std::string GAME_TITLE = "Space Invaders";

// Usage in functions
void update_player(Player& player, float delta_time) {
    player.x += PLAYER_SPEED * delta_time;
}

void initialize_game(GameState& state) {
    state.enemies.resize(MAX_ENEMIES);
    set_window_title(GAME_TITLE);
}
```

- Use `const` keyword to ensure immutability.
- Name constants in ALL_CAPS for easy identification.
- Group related constants in headers or structs for better organization.
- Use constants for values that are truly constant throughout the game.

---

## 3. Maintain Coherent Control Flow

**Principle**: Establish and follow consistent patterns for control flow throughout your game code.

**Why?**

- Improves code readability and maintainability.
- Reduces cognitive load when switching between different parts of the codebase.
- Facilitates easier onboarding for new team members.
- Helps in identifying and fixing bugs more quickly.

**How to Achieve Coherent Control Flow**

- **Consistent Naming Conventions**
  - Use snake_case for function and variable names (or whatever is conventional in your chosen language).
  - Use PascalCase for struct and class names.
  - Prefix boolean variables with `is_` or `has_`.
- **Uniform Function Structures**
  - Maintain a consistent order of operations within similar functions.
  - Use similar parameter ordering across related functions.
- **Centralized State Management**
  - Group related state variables in structs.
  - Update state in dedicated functions.
- **Consistent Error Handling**
  - Use a uniform approach to error checking and reporting.
  - Consider using exceptions for exceptional cases.
- **Modular Design**
  - Break down complex logic into smaller, focused functions.
  - Maintain a clear separation of concerns.

*Example - Coherent Control Flow in Game Entity Updates:*

```cpp
struct Entity {
    float x, y;
    bool is_active;
};

struct Player : Entity {
    int health;
    float speed;
};

struct Enemy : Entity {
    int damage;
    float patrol_radius;
};

// Consistent function naming and structure
void update_entity(Entity& entity, float delta_time) {
    if (!entity.is_active) return;

    // Common entity update logic
}

void update_player(Player& player, float delta_time) {
    update_entity(player, delta_time);  // Call base update first

    // Player-specific update logic
    handle_player_input(player);
    update_player_health(player);
}

void update_enemy(Enemy& enemy, float delta_time) {
    update_entity(enemy, delta_time);  // Call base update first

    // Enemy-specific update logic
    patrol_area(enemy);
    check_player_collision(enemy);
}

// Main game loop with consistent flow
void game_loop(GameState& state) {
    float delta_time = get_delta_time();

    update_player(state.player, delta_time);

    for (auto& enemy : state.enemies) {
        update_enemy(enemy, delta_time);
    }

    handle_collisions(state);
    render_game(state);
}
```

- **Hierarchy**: `update_player()` and `update_enemy()` both call `update_entity()` first, establishing a clear hierarchy and avoiding code duplication.
- **Consistency**: All update functions follow the same pattern: check if active, perform base updates, then specific updates.
- **Naming**: Function names clearly indicate their purpose (e.g., `update_player`, `handle_collisions`).
- **Structure**: The `game_loop` function provides a clear, consistent structure for each frame of the game.
- **Modularity**: Each function has a single responsibility, making the code easier to understand and maintain.

---

By following these principles, you create a codebase that is more intuitive, maintainable, and scalable. This coherent structure allows developers to quickly understand the flow of the game and make modifications with confidence.

To get started developing graphical applications which use Splashkit's event and rendering functionality, check out:

[Splashkit Documentation](https://splashkit.io/api)

[BreakIn Tutorial - Introduction](https://github.com/p4stoboy/BreakIn#introduction)

[BreakIn Tutorial - Event Loop](https://github.com/p4stoboy/BreakIn#the-event-loop)

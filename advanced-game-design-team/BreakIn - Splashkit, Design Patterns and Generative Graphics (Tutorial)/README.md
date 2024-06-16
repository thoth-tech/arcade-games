![BreakIn](tutorial_assets/title.gif)

# Splashkit :: How to build BreakIn

---

As of end of T1 2024 the current tutorial sections are implemented and the provided codebase is in line with them:

- **[Introduction]**
    - Overview of BreakIn
    - Setting up the development environment
- **[The Event Loop]**
    - Understanding the main game loop
    - Event handling and game flow
- **[Detour - Utility Functions]**
    - Implementing utility functions for game development
    - Rolling our own XOR PRNG
- **[Setting Up Global Constants]**
    - Defining constants for screen dimensions and gameplay settings
    - Organizing global constants for easy access and modification
- **[Global State Management]**
    - Designing the global state structure
    - Managing game-wide states and settings
- **[Basic Game Entities]**
    - Defining instance structures for game objects
    - Abstracting common functionalities
- **[Initialising Game Entities]**
    - Creating instances of game objects
    - Initialising states for gameplay
- **[Terrain Grid and Block Positioning]**
    - Implementing the grid system for blocks
    - Storing block positions within the grid
- **[Populating the Grid]**
    - Creating a function to generate a fully populated grid
- **[Updating State and Looping Back]**
    - Handling state changes during gameplay
    - Integrating state updates with the event loop
    - Implementing update functions for game entities
- **[Drawing the Game]**
    - Implementing draw functions for game entities
    - Rendering the game state on the screen
- **[Making the Terrain Endless]**
    - Implementing DFS culling to remove disconnected blocks
    - Adding velocity to blocks for smooth transition
- **[Particles]**
    - Creating a particle type
    - Implementing particle updates and drawing
    - Spawning particles on block destruction
---

# Plans for T2:

- BallEffect type and higher order functions

- Asset loading and audio

- Optimisation via threading

- Implementing menu

- Granular exploration of coordinate system and vectors

- How to apply 90% of this codebase to make a completely different game

- Making palettes

- Other procedural data generation and animation techniques

- Saving game state

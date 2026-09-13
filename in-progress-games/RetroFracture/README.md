# RetroFracture 🎮

*A Multi-Genre Arcade Adventure*

RetroFracture is a multi-genre arcade game built in C++ using SplashKit, featuring 8-bit neon aesthetics and genre-switching gameplay. Each level represents a different genre, with the player adapting to new mechanics, controls, and challenges as they progress.

![RetroFracture Banner](assets/envs/Apartment%20Hallway.png)

## 🌟 Features

### 🎯 Genre-Switching Gameplay

- **Platformer**: Precision jumping and exploration
- **Run & Slash**: Melee combat with acquired weapons 
- **Beat 'Em Up**: Combo-based boss battles [In Future]
- **Top-Down Shmup**: Vehicle combat [In Future]

### 🎨 Visual Style

- **8-bit pixel art** with vibrant neon palette
- **640×480 resolution** (arcade-perfect)
- **Smooth animations** and dynamic camera

### 🕹️ Current Implementation

- Explorable hallway level with multiple apartments
- **Apartment 101 interior** – fully playable platforming level
- Player physics and collision
- Interactable system (doors, prompts)
- Adaptive HUD

## 🚀 Quick Start

### Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- [SplashKit SDK](https://splashkit.io/) installed

### Installation & Building

```bash
# Fork and clone the repository
git clone https://github.com/yourusername/RetroFracture.git
cd RetroFracture

# Build with SplashKit
skm clang++ program.cpp src/*.cpp -o RetroFracture

# Launch the game!
./RetroFracture
```

### Controls

| Key     | Action                |
| ------- | --------------------- |
| **A/D** | Move left/right       |
| **W**   | Jump                  |
| **E**   | Interact with objects |
| **ESC** | Quit game             |
| **R**   | Reset position        |

## 📁 Project Structure

```
RetroFracture/
├── assets/                   # All game assets
│   ├── envs/                 # Backgrounds & environments
│   │   ├── Apartment Hallway.png
│   │   └── lvl1_platform.png
│   └── sprites/              # Player animations (Striker)
│       ├── striker/          # Striker character spritesheets
│       └── ui/               # UI elements
├── src/                      # Source code
│   ├── game.cpp/.h           # Main game controller
│   ├── player.cpp/.h         # Player character & physics
│   ├── hallwaylevel.cpp/.h   # Hallway level logic
│   ├── lvl1.cpp/.h           # Apartment 101 level
│   ├── hud.cpp/.h            # UI & display
│   └── interactable.cpp/.h   # Interactive objects
├── include/                  # External dependencies
│   └── splashkit -> (symlink)
├── program.cpp               # Entry point
├── config.txt                # Arcade machine config
└── README.md
```

### Architecture Overview

- **Game Class**: Orchestrates levels, camera, and game state
- **Player Class**: Handles movement, animation, physics, and input
- **Level Classes**: Modular level implementations (`HallwayLevel`, `Lvl1`)
- **HUD Class**: Context-aware UI rendering
- **Interactable System**: Event-driven interactions (doors, prompts)

### Adding New Levels

1. Create `lvlX.cpp/.h` following the pattern of `lvl1`
2. Add the new level to the `Game` enum and instance variables
3. Implement transition methods in `game.cpp`
4. Add the level to update/draw switches

## 🗺️ Planned Roadmap

### Campaign (8 Levels, 2 Levels per Act)

```
Act 1:
├── ✅ Apartment Hallway
├── 🔄 Apartment 101 (Level 1)
├── 📋 Apartment 102 (Level 2)
├── 📋 Apartment 103 (Level 3)
├── 📋 Apartment 104 (Level 4)
├── 📋 Apartment 105 (Level 5)
├── 📋 Apartment 106 (Level 6)
├── 📋 Apartment 107 & Boss
└── 📋 Rest of apartment complex

Act 2:
└── To be decided
Act 3:
└── To be decided
Act 4:
└── To be decided
```

*The "Apartments" are the actually the long form game stage levels* 

### Coming Soon

- Enemy AI and combat systems
- Character selection mechanics
- Music and sound implementation
- More apartment interiors (102–107)
- Lighting/shading effects

## 🤝 Contributing

Contributions are welcome! Please follow the existing code style and document complex logic with comments. For major changes, open an issue first to discuss what you would like to change.

---

*Built with [SplashKit](https://splashkit.io/) – making game development accessible.*

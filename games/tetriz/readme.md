# Tetriz

A modern terminal-based Tetris game written in C++20 with cross-platform support.

## 🎮 Features

- **Classic Tetris Gameplay**: Complete with all standard Tetris mechanics
- **Cross-Platform**: Runs on both Linux and Windows
- **Terminal UI**: Beautiful terminal-based interface with colors and animations
- **Modern C++**: Built with C++20 features and best practices
- **Real-time Controls**: Responsive keyboard controls
- **Game Statistics**: Score, level, and line tracking
- **Hold System**: Store pieces for later use
- **Next Piece Preview**: See upcoming pieces
- **Pause/Resume**: Full game state management
- **Help System**: Built-in controls reference

## 🎯 Game Controls

| Action | Key(s) |
|--------|--------|
| Pause/Resume | `W` |
| Rotate Left | `T` |
| Rotate 180° | `G` |
| Move Left | `A` |
| Move Right | `D` |
| Soft Drop | `S` |
| Hard Drop | `F` |
| Hold Piece | `Y` |
| Reset Game | `R` |
| Help | `H` |
| Quit | `ESC` |

## 🚀 Quick Start

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 12+, or MSVC 2019+)
- CMake 3.22 or later

### Linux

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install cmake g++

# Clone and build
git clone <your-repo-url>
cd tetriz
mkdir build && cd build
cmake ..
make

# Run the game
./tetriz
```

### Windows

```bash
# Using Visual Studio Developer Command Prompt
git clone <your-repo-url>
cd tetriz
mkdir build && cd build
cmake ..
cmake --build . --config Release

# Run the game
.\Release\tetriz.exe
```

## 🏗️ Building from Source

1. **Clone the repository**:
   ```bash
   git clone <your-repo-url>
   cd tetriz
   ```

2. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   # Linux/macOS
   make
   
   # Windows (Visual Studio)
   cmake --build . --config Release
   ```

5. **Run the executable**:
   ```bash
   # Linux/macOS
   ./tetriz
   
   # Windows
   .\Release\tetriz.exe
   ```

## 📁 Project Structure

```
tetriz/
├── CMakeLists.txt      # Build configuration
├── main.cpp            # Entry point
├── game.h/cpp          # Core game logic
├── piece.h/cpp         # Piece management
├── tetromino.h/cpp     # Tetromino definitions
├── control.h/cpp       # Input handling
├── terminal.h/cpp      # Terminal control
├── window.h/cpp        # UI rendering
├── draw.h/cpp          # Drawing utilities
├── utils.h/cpp         # Utility functions
├── color.h             # Color definitions
├── define.h            # Global definitions
└── tetriz.map          # Game map data
```

## 🎨 Game Features

- **7 Standard Tetrominoes**: I, O, T, S, Z, J, L pieces
- **Line Clearing**: Complete lines disappear and award points
- **Level Progression**: Game speed increases with level
- **Scoring System**: Points for line clears and drops
- **Hold Mechanism**: Store one piece for later use
- **Next Queue**: Preview upcoming pieces
- **Game States**: Pause, help, and game over screens
- **Responsive UI**: Adapts to terminal size changes

## 🔧 Technical Details

- **Language**: C++20
- **Build System**: CMake
- **Dependencies**: Standard library only
- **Platform Support**: Linux, Windows
- **Terminal Support**: ANSI escape sequences
- **Threading**: C++20 std::jthread for input handling

## 📋 Requirements

### Minimum System Requirements
- **OS**: Linux (any modern distribution) or Windows 10+
- **RAM**: 64 MB
- **Storage**: 1 MB
- **Terminal**: ANSI-compatible terminal emulator

### Development Requirements
- **Compiler**: C++20 compatible (GCC 10+, Clang 12+, MSVC 2019+)
- **CMake**: 3.22 or later
- **Build Tools**: Make or Visual Studio Build Tools

## 🎯 Game Rules

- **Objective**: Clear horizontal lines by filling them with tetromino pieces
- **Scoring**: 
  - Single line: 100 × level
  - Double lines: 300 × level  
  - Triple lines: 500 × level
  - Tetris (4 lines): 800 × level
- **Level Up**: Every 10 lines cleared increases the level
- **Game Over**: When pieces reach the top of the playfield

## 🐛 Troubleshooting

### Common Issues

1. **Terminal not responding**: Ensure your terminal supports ANSI escape sequences
2. **Build errors**: Check that you have C++20 compatible compiler
3. **Input not working**: Make sure terminal is in focus and supports raw input

### Getting Help

If you encounter issues:
1. Check the terminal compatibility
2. Verify build requirements
3. Open an issue with system details

---

**Enjoy playing Tetriz!** 🎮
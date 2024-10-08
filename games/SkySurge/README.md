# Repo Name

1. Requires a [SplashKit installation](https://splashkit.io/installation/).
2. OPTIONAL: Build using `skm dotnet build` while in the project folder. Note that you may need to manually specify the build target using e.g., `skm dotnet build Sky_Surge.csproj`.
3. Build and run using `skm dotnet run` while in the project folder.

_See also: usage documentation for [skm](https://github.com/splashkit/skm/blob/master/README.MD) and [SplashKit](https://splashkit.io/guides/using-splashkit/0-overview/)._

## Technical overview

- **Language:** C#
- **Current scope:** Small
- **Code quality:** Fair
- **Gameplay implementation:** Incomplete

## Gameplay overview

A vertically scrolling shooter for one player. Control an aircraft and dodge or destroy enemy aircraft.

### Player 1 Controls

| Action       | Key          |
|--------------|--------------|
| Move Forward | Up arrow     |
| Move Left    | Left arrow   |
| Move Right   | Right arrow  |
| Move Down    | Down arrow   |
| Shoot        | SPACE        |

## Other notes

- Documentation from the original author can be found in `/docs/`, though it describes the game as though it were complete so could be thought of as more of a to-do list than a description of the current game.
- BUG: the game can run significantly faster while the player isn't shooting because the game's primary rendering loop is not correctly configured.

## Screenshots

![](docs/screenshot-01.png)

## Credits

Originally created by Ayan Masood in 2023-T3.

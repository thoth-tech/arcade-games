# ARCADE PING PONG

## How to Install

- Install [Splashkit](https://www.splashkit.io) to your computer 
- Enter to your terminal and open the project folder
- Enter command `skm clang++ program.cpp player.cpp game.cpp obstacle.cpp ball.cpp -o kkc` to build the file
- Enter command `./kkc` to play the game
- Press ENTER button to start the game 

> Arcade Machine Mechanisms
### **Player 1**   

| Input | Description |     
| --- | ----------- |   
| W Key | Player 1 dicer goes UP |          
| S Key | Player 1 dicer goes DOWN |
| A Key | Player 1 dicer goes LEFT |          
| D Key | Player 1 dicer goes RIGHT |
| R Key | RESTART |

### **Player 2**   

| Input | Description |     
| --- | ----------- |   
| Up Key | Player 2 dicer goes UP |          
| Left Key | Player 2 dicer goes LEFT |
| Right Key | Player 2 dicer goes RIGHT |          
| Down Key | Player 2 dicer goes DOWN |
| ESC Key | Exit The Game |



# Documentation 

## _Introduction_

I will provide an overview of the design process for a C++ based Ping Pong game
using the Splashkit SDK. The game follows the rules of Air Hockey and allows two players to
compete against each other. The cursor is allowed to move horizontally, and player one controls
their movement using the WASD keys, while player two uses the arrow keys. The game is
designed for an HD screen resolution of 1280x720.

## _Asset Structure_

To ensure consistency and reusability, a basic default structure was used for every asset in the
game. The Asset.h file contains the asset's structure, bitmap enumeration, and functions that
are shared across libraries. The Asset.cpp file includes the asset's constructor, update_game
function, draw_game function, and other in-game functions.

## _Game Design_

The design process began with drawing the background bitmap using Canva. A football-themed
background was chosen for this game. The dimensions of each asset were calculated, and
horizontal and vertical obstacles were drawn as boundaries. A free bubble bitmap was selected
for the ball asset.

![Alt text](image.png)

## _Asset Creation_

The game assets were created following the design specifications. The background bitmap was
drawn on the screen, and obstacle assets were created for the upper, left, right, and bottom
borders. These assets were then called in the game to create the complete background.
The ball asset was designed next, with the specifications that it would spawn at the center with
a random angle and speed. Player assets, represented as cursors or blocks, were also
designed and called in the game.

![Alt text](image-1.png)

## _In-Game Rule and Physics_

After creating all the assets, the in-game rules and physics were implemented. The
out_of_map() function was designed to check if the ball is out of the left or right screen,
indicating that the opposing player has scored. The collision() function was created to handle all
collisions, both horizontally and vertically, with the ball.

## _Testing Debugging_

Throughout the development process, extensive testing and debugging were conducted.
Several issues were encountered, particularly with the collision part of the code. These issues
were addressed and resolved to ensure smooth gameplay.

## _Final Design_

After debugging, the game screen was redesigned using Canva backgrounds. This step
enhanced the visual appeal of the game and provided a more immersive experience for the
players.

![Alt text](image-2.png)

## _Conclusion_

In conclusion, a C++ based Ping Pong game was successfully designed using the Splashkit
SDK. The game follows the rules of Air Hockey and allows two players to compete against each
other. The cursor movement is enabled horizontally, with player one using the WASD keys and
player two using the arrow keys. The game assets, including the background, obstacles, ball,
and player cursors, were created and implemented following the design specifications. In-game
rules and physics, such as scoring and collision detection, were also incorporated. Extensive
testing and debugging were conducted to ensure a smooth gameplay experience. The final
design included visually appealing Canva backgrounds, enhancing the overall aesthetics of the
game.




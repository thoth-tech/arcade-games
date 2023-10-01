# CAR-RACING

## How to Install

- Install [Splashkit](https://www.splashkit.io) to your computer 
- Enter to your terminal and open the project folder
- Enter command `skm clang++ program.cpp player.cpp game.cpp -o output` to build the file
- Enter command `./output` to play the game
- Press ENTER button to start the game 

### *Rules*   

| Input | Description |     
| --- | ----------- |   
| A Key | Car goes LEFT |          
| D Key | Car goes RIGHT |
| R Key | Change The Car Colour Between The Game |


# Documentation 

## Introduction
The Car Race game developed by me using the Splash Kit SDK. The game is a basic arcade-style car game where the player's objective is to navigate a car on a road while avoiding collisions with other cars.

## Game Design
I started by designing the basic rules of the game. The game features a car that is spawned on a road and needs to avoid other cars approaching it. The game is designed to have a vertical orientation with a resolution of 800x600 pixels.
![image](https://github.com/KhushiLaddi08/arcade-games/assets/113286899/9f3cec55-f7bb-4d40-a855-a7fa24dec69d)

## Drawing and Animation
To create the lane road animation, I took inspiration from the thoth-tech/arcade-games 2D racing demo. I spent time understanding animations in C++ and utilized C# functions to adapt to the required changes.

![image](https://github.com/KhushiLaddi08/arcade-games/assets/113286899/e26c7900-4e40-42f2-b3ca-4b24de0b570e)

![image](https://github.com/KhushiLaddi08/arcade-games/assets/113286899/2937ad15-4fbb-4852-a4db-042e61ab7b13)

After successfully implementing the animation and car movement, I created a template on Canva for the score and overall theme of the game.

## Car Structure and Functionality
I designed the structure and functions for the cars in the game. The player's car was initialized within the game, and a handle_input() function was implemented to enable car movement. Additionally, a vector was defined to store the obstacle cars, which are spawned every 2 seconds with random speeds. A function was created to initialize these cars and store them in the vector. To optimize memory allocation, a check was implemented to remove cars that went out of the screen (bottom) from the vector.

## Scoring and Collision
The game includes a scoring mechanism that increments the score within a loop. I also implemented a collision function to detect collisions between the player's car and the obstacle cars. If a collision occurs, the game ends.
![image](https://github.com/KhushiLaddi08/arcade-games/assets/113286899/ef01ef6e-aa50-4935-8a3e-a85b77995c2e)

## Additional Features
I added multiple screen phases to enhance the gameplay experience. Sound effects such as car crash sounds, car motor sounds, and lobby music were incorporated to make the game more immersive. Furthermore, a restart button was implemented to allow players to restart the game after it ends.

## Conclusion
In conclusion, I successfully designed a Car Race game using the Splash Kit SDK. The game features a player car that needs to avoid collisions with obstacle cars while navigating a road. I implemented various functionalities such as car movement, obstacle spawning, scoring, collision detection, and additional features like sound effects and screen phases. The game provides an engaging and enjoyable experience for players.
![image](https://github.com/KhushiLaddi08/arcade-games/assets/113286899/fcb84b21-9a3d-4e3e-9ac3-d292c616ca34)

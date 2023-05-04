# Documentation on Splashkit's Brick Breaker/Breakout game - 'DX game'

| Members | Roles |
|---------|---------|
|Swanny Aurellia | Code|
|Norman Spencer Harty | Code|
|Tulsi Balamurali| Documentation: Purpose and steps involved, Raspberry Pi testing|
|Harney Halls | Documentation: Introduction and Details of game, Game sprites|

## _Introduction_
The team has decided to create a clone of the games 'DX' and 'Breakout', by using C++ and Splashkit to demonstrate the capapbilities available to splashkit. Swanny and Norman have been the coder's behind the game, while Tulsi has worked on the documentation and testing the game on rapsberry Pi, and Harney have worked on both documentation and the game sprites. The game is made to assist future students in developing games and understanding the fundamentals of C++ and the coding language with Splashkit. The game is also made to test splashkit's capabilities with Raspberry Pi and arcade style games thanks to the work of Tulsi. DXball/Breakout is one of two new products being developed, alongside the Asteroid's game done by Phil Williams, Riley Michael and Ro Wern Ong.

## _Purpose of the game_
This application/game is designed to demonstrate and explore the capabilities of SplashKit, an open-source Software Development Kit (SDK) created with the purpose of reducing the overhead required for truly technical coding.

SplashKit is primarily used for coding 2D games with unrestricted creativity and have a grounded knowledge of C++. To show the efficiency and working of this software we have decided to create a DX Ball Game, also known as Breakout, which is an arcade video game designed to be played on arcade machines and raspberry pi-based units. The functionality of the game can also be later extended to other gaming platforms like mobiles, PCs and console environments and is open for interpretation of future developers. 

## _Steps involved in making the game_
#### Programming Language:

The programming language we have chosen is C++ and an open source like SplashKit. Main reason for the team to choose C++ because some members had more experience in the language. This language was used in SIT102 Introduction to Programming, which also made use of SplashKit. 

C++ was chosen because it has imperative, object-oriented, and generic programming features and runs on various platforms like Windows, Linux, Unix, Mac, etc. C++ is an object-oriented programming language, which is a collection of commands instructing the computer with C++ ASCII text files. The language can support unions and structures that are a mix of stand-alone and put-together files. C++ programs are useful for low-level programming language and efficient for general purposes. They offer performance, memory efficiency, and high-level abstraction, within the language of the matter domain.

Two major problems in C++ is complex during a very large high-level program, C++ is employed for platform-specific application and commonly, for the actual OS or platform, the library set is typically chosen.   C++ program is complex during a very large high-level program, C++ is employed for platform-specific application commonly, For the actual OS or platform, the library set is typically chosen.

Despite the disadvantages, the main reason for the team to choose C++ because some members had more experience in the language. This language was used in SIT102 Introduction to Programming, which also made use of SplashKit. Since the team had a clear language preference, C++ was chosen as the way to go.

#### Software Used:

We also use GitHub to store and share our work within the team. GitHub is a platform used to manage open-source projects. It is free to use for open-source projects and includes a wiki and issue tracker. 

Microsoft Teams was used to communicate within the team members and used to share various documentation and data regarding the project. It was also used to communicate with fellow company members and the company staff. 

## _Description of the game_
The brick breakout game is a simple game where the player must destroy bricks of a wall in order to proceed to the next level. The game is based off of the games Break out, and DX-ball, as shown below.
![DX-Ball](https://images.sftcdn.net/images/t_app-cover-l,f_auto/p/0f5173aa-96da-11e6-9405-00163ec9f5fa/4242823519/dx-ball-2-screenshot.jpg).
The game would have an opening screen that would allow the player to begin the game, including the name of the game, Thoth Tech company, and Splashkit. 
The game would run by allowing the player to move the platform left or right, or the x axis. The ball bounces between the walls and the bricks, and the platform. The goal is to break all the bricks on screen without the ball falling to the floor. The player would need to ensure the ball is able to be deflected off of the platform each time it returns. Once the ball hits the floor (past the platform), the player would be met with a game over screen.

#### Environment:

The software involved are the MSYS terminal to compile and run SplashKit games and a code editor such as Visual Studio to write the code. MSYS2 is software distribution and a building platform for Windows. It provides a Unix-like environment, a command-line interface and a software repository making it easier to install, use, build and port software on Windows. It is an independent rewrite of MSYS, based on modern Cygwin (POSIX compatibility layer) and MinGW-w64 with the aim of better interoperability with native Windows software.

We have used  basic shapes like circle for the ball and rectangle for the blocks and paddle for the initial development of the game. We replaced these basic structures with sprites for bettering the cosmetics and the aesthetics of the game. Sprites are created using basic software like MS Paint. 

The draw_blocks function in the code was made to help drawing all the blocks to the environment. This function is mainly used to take an array of blocks and iterate it to draw each block on the scene. If a particular block has been broken, it will not be drawn to the environment.   

#### Types of Bricks:

Currently, three different types of bricks are being used in the game.

1. **Single Collision Bricks**: These bricks can be destroyed when collides with the ball once. These bricks have the easiest difficulty to score with.
2.	**Dual Collision Bricks**: These bricks can be destroyed after colliding with the ball twice. They have been designed to offer the player medium difficulty to clear the level with.
3.	**Hidden Blocks**: These blocks are invisible to the player on the first playthrough. These blocks become visible once the ball comes in contact with the brick. Although, these blocks are quite easier to clear with as they can be broken on the second playthrough basically becoming a single collision brick. 

#### Controls of the game:

1.	**Controlling the player movement**: The function that is being used to control players is **process_events()**. The key part of the function is to allow SplashKit to react to user input e.g., keyboard presses. The most basic controls are:   
a.	**Key A** – Move the paddle to the left.
b.	**Key D** – Move the paddle to the right.
c.	**Key R** - Restart the game after winning/losing the game.
d.	**Key F** - Shortcut to change level (for developmental and display purposes only)

2.	**Controlling the ball movement**: The structure that is used to control ball movement is  **ball_data** that contains the direction of the ball. The variable **bool_up** decides if the ball moves upwards or downwards on the y axis, while **bool_right** decides if the ball moves towards the left or right on the x axis. For example, if **bool_up** and **bool_right** is true, the ball is moving diagonally upwards towards the right side.

3.	**Continuous Ball Movement until stoppage**: The ball follows the laws of physics and to the other direction when colliding with the walls and bricks. The collision is detected by evaluating the ball location. When the edges of the ball is at the location of the edges of the walls or the blocks, the ball will bounce. This functionality is included under the function **check_ball_collision**.  

4.	**End the game if out of scope**: When the ball hits the ground, the ball is removed. If there is no more presence of the ball in the game, the game state variables are set to: **game_over = true, game_won = false**. This will show a text saying **You lose, press R to try again**. The player can then press **R** to restart the game. 

#### Scoring in the game:

1.	**Sensing the breakage of the brick**: The **brick_data** structure contains the brick's hit point. Each time the brick collides with a ball, the value of the hit point is reduced. When the value hits **0**, the brick is broken and score is added. 

2.	**Calculating scores in the game**: The scoring system is **Ax1** point added to the score when a brick is broken, where **A** is the amount of the points accumulated by the player until that point in the game. Each time a block is broken, the score is increased by one unit. When the ball finally goes out of scope, the final value on **A** is displayed.

3.	**Proceeding to the next level**: The **remaining_blocks** variable determines how many bricks are left in the game. It has a predefined value pertaining to that level depending on the number of blocks in the level. Once it reaches zero, the Boolean variable **next_level** becomes true and the function to spawn the next level is called. 

#### Powerups: 

1.	**Multiple Balls** – Based on the score limiter, multiple balls can be generated to break the balls much more effectively and easily. 
2.	**Score multipliers** -  

#### Levels: 

We currently have four levels that each introduce a new gameplay mechanic, which are the different types of bricks in this case. 

1.	**Level 1** introduce basic gameplay with single collision bricks and making thee player familiar with the existing gameplay. 
2.	**Level 2** is updated with dual collision blocks to entertain the player with moderate difficulty leading to the next level. 
3.	**Level 3** introduces hidden blocks to show and test the player’s capabilities that has been earned so far in the game.
4.	**Level 4** combines everything we had tested the player for. We will have a combination of all the different types of bricks and players will be encouraged to use the powerups in order to clear the level. 

The goal of the four levels is to showcase the game we have built so far. It combines all the gameplay mechanics and items to demonstrate the work that we have been involved so far. Future teams can add more levels with increasing difficulties whilst also giving players more powerups and scoring opportunities.

Originally, the idea was decided between 10 set levels, or the ability to have infinite levels with chances of repetative levels. The idea was chosen for, at the time, 10 levels to be included. It was then changed to 4 levels, as there was nothing new to be introduced after level 4. 

Future ideas for the game can include lives of the player (standard of 3 lives), with the chance to gain more lives through powerups. Different powerups, such as a larger ball, or smaller platform, or 'bombs' to explode large chunks of bricks. There can also be disadvantages to the player too, such as the smaller platform, lose a life or others. 

## _Sprites of the game_ (Harney)
Sprites include:
 - Different coloured blocks/bricks.
 - multiplier score (x2, x3, x4, x5) gauge.
 - power ups (transparent images drop down).
 - Platform.
 - ball/s.
 - Score text
 - Multiplier text
 




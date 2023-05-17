# Documentation on Runner Dash

| Members | Roles |
|---------|---------|
|Swanny Aurellia | Documentation|
|Norman Spencer Harty | Documentation|
|Tulsi Balamurali| Code|
|Harney Halls | Code|

## _Introduction_
Runner Dash is a game built by the SplashKit team in T1 2023, with the members Swanny, Norman, Tulsi, and Harney. Tulsi and Harney have worked on the code for this game, while Swanny and Norman wrote the documentation. Himanshu has built the base code for Runner Dash as a terminal game using the C# language. The team then decided to use C++ to build the game using SplashKit. The game will be tested on Raspberry Pi to see how it performs on the arcade machine. This is the second game the team has worked on, after working on DX Ball game in the first half of the trimester.

## _Purpose of the game_
The purpose of this game is to demonstrate the use of SplashKit to convert a terminal game into a game with graphics that can be run on the arcade machines. This game will show that SplashKit can be integrated into the backend of a pre-existing game to turn a simple game made to learn about the processes behind a game into one that looks like a real arcade game.

## _Steps involved in making the game_
### Base Code
The development of this game began with the base code created by Himanshu. This code is written in C# and runs on the terminal.

<img width="469" alt="runnerdash_screenshot" src="https://github.com/swanos/arcade-games/assets/72289464/6bc3a6fc-5135-4b63-81ca-78c753e5605a">

The team then started developing the code for this game based on the source code provided, with some modifications to include the SplashKit library, and in order for it to be compatible with the arcade machine.

### Programming Language
The team has chosen to use C++ as the programming language to build the game on SplashKit. Although the base code for the terminal game was written in C#, the team chose C++ because of their familiarity with the language.

### Tech Stack
**Microsoft Teams** was used as the main communication method between team members. The team communicates task to complete each week and update each other after finishing a task. The team also participates in weekly meetings with the other SplashKit teams using Microsoft Teams.

**GitHub** is used to store files the team has worked on and sharing files between members. A member makes a pull request to Thoth Tech's `arcade-games` repository under the `runnerdash` branch after major updates. 

**SplashKit** is an open-source Software Development Kit with many functions useful for creating games and animations. It is also used as a tool to learn programming, introduced in units such as SIT102 - Introduction to Programming.

**MSYS2** is a terminal used to install and run SplashKit. The team uses this terminal to install the library, create SplashKit projects, and running the program.

## _Details of the game_
The terminal version of the game involves the player moving through a grid using the arrow keys to gain score by collecting gems while avoiding an enemy that moves towards the player when the player moves.
There is one gem on the screen at a time that gets replaced by a new gem at a random position when collected.
The game is endless, with no win condition, only the enemy colliding with the player as a loss condition.
The SplashKit version of the game will use an arcade machine's joystick for player movement and SplashKit graphics instead of a text grid.

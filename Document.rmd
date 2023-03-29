---
title: "Game Documentation (Arcade 2.0)"
author: "Thoth Tech"
date: "2023-03-26"
output: 
  word_document:
  toc: yes
  toc_title: Table of Content
---



Table of Content(up to date)

1. [Name of the Game](#Name-of-the-Game)
2. [Genre](#Genre)
3. [Art style](#Art-style)
4. [What is the Game](#What-is-the-Game)
5. [How the Game works](#How-the-Game-works)

   5.1 [How the Game is Played Solo](#How-the-Game-is-Played-Solo)
   
   5.2 [How the Game is Played Cooperatively](#How-the-Game-is-Played-Cooperatively)

   5.3 [How the Game is Won](#How-the-Game-is-Won)

   5.4 [How the Game is Lost](#How-the-Game-is-Lost)
   
6. [Levels](#Levels)
7. [Enemies](#Enemies)
8. [Tools](#Tools)
9. [Reference Game](#Reference-Game) 



```{r}
```

## 1. Name of the Game
Asteroid 2.0


## 2. Genre
Shooter


## 3. Art style
Flat


## 4. What is the Game
Asteroid 2.0 is a 2D Shooter game in which a ship controlled through A controller capable of moving in all directions. The ship will move in space and will be obstructed by Meteors and a final alien ship. The player's ship will be loaded with shooter and will help to shoot the hurdles.  


## 5. How the Game works
Asteroid 2.0 game start with a cool attractive intro with animation and music, leading to a main menu which will contain different options such as selecting game mode, then selecting the sprites. The player will use controller to control the ship sprite and move it in all directions. Besides, that the t ship will be attacked by Meteors, other objects(to be decided) and a boss ship which it will tackle through shooters. Each time the ship is hit its life will decrease and overall a player will get 3 lives. 

Further, there would be 5 levels, with each level to be around 1 min excluding the boss level. Also, there will be power ups for the player which will help ship to add lives, add more shooter power. Similar to other arcade game it will contain score for hitting the targets and Top score will be recorded. 


### 5.1. How the Game is Played Solo
When selecting the game mode, you will be able to choose Single Player, when the game is then loaded in only a single ship will appear, and the game will continue as normal with the solo player destroying asteroids to survive until the final alien ship.


### 5.2. How the Game is PLayed Cooperatively
When selecting the game mode, will be be able to select Two player mode, when the game loads in two different ships will appear that can each be controlled by a separate player. the game will then proceed with both players being able to destroy asteroids and the final alien ship.


### 5.3. How the Game is Won
The game is won when the player has tackled all the obstruction and hurdles, and then finishes the boss spaceship. The player will have shooters for shooting and different power ups to assit. 


### 5.4. How the Game is Lost
The game is lost after the player has lost all of their lives. Players will start the game with 3 Lives and can lose them by getting hit by asteroids and the alien ship. players will be able to gain back lost lives by picking up power ups throughout the game. 

## 6. Levels 
Each level will last for 1 minute. 

Level 1 : Player will get attacked by Meteors and will tackle them using shooters. At-last Solider ship will arrive.

Level 2 : Player will get attacked by Meteors and will tackle them using shooter but will get power ups like life or more stronger shooters. At-last Army general ship will arrive.

Level 3 : Player will get attacked by Meteors and will tackle them using shooter but will get power ups like life or more stronger shooters. More obstructions and At-last Boss ship will arrive

## 7. Enemies
There are three enemies for each level.

Solider ship : This ship will arrive at end of level 1 and it will be the weakest among three ships.

Army general ship : This ship will arrive at end of level 2 and it will be the stronger than Solider ship.

Boss ship : This ship will arrive at end of level 3 and it will be the strongest among three ships.

## 8. Tools 
Tools used fr developing game is SplashKit with C#. 

## 9 Reference Game

![Menu Part](img1.png){width="45%"}


![Player Selection](img2.png){width="45%"}


![Multi-PLayer PLay](img3.png){width="45%"}


![Visual of Game 1](img4.png){width="45%"}


![Visual of Game 2](img5.png){width="45%"}


![Visual of Game 3](img6.png){width="45%"}


![Visual of Game 4](img7.png){width="45%"}




# Below the Surface Documentation

This is a collaborative effort to create an **exceptional game** to be implemented within the **Arcade Machine**.


## Header Files

Here are the following header files that you should be aware of while developing this game. 

**player.h** 
Header file containing the player class, its attributes, functions, and states.

**map.h**
Header file containing the necessary contents for the game to display when playing.

**level.h**
Header file responsible for scene management. Such as switching levels or checking collisions.

**enemy.h**
Header file containing enemy classes, its attributes and functions.

**behaviour.h**
Header file responsible for determining the AI behaviour of different enemy class.

**block.h**
Header file responsible for each of the block's behaviour.

**camera.h**
Header file responible for tracking the player around the level.

**cellsheet.h**
Header file responsible for the definition of cells.

**collision.h**
Header file responsible for handling collisions throughout the level.

**get_level.h**
Header file responsible for grabbing a level from header files level to screen.

**hud.h**
Header file responsible for the display of the player HUD.

**screen.h**
Header file responsible for displaying the game onto the screen.

**testing.h**
Header file responsible for testing functions.
 
## Level Text Files
Within folder **levels** there are text files filled with block id. These ids are the what represents the levels that you see within the game. And you can create one by using the level editor. Just make sure to add them in the folder once you've created it. **DO NOT CHANGE THE VALUES INSIDE THE TEXT FILES**. Otherwise, the levels will be modified. If you want to test out the level you have created, write the following code.

### Format
```
./test -l {amount of layers} levels/{layer0.txt} levels/{layer1.txt} {name of the level}
```

### Example
```
./test -l 2 file0.txt file1.txt test
```

## Loading Levels in Level Editor
If you want to edit an existing level. Drag the files of that level onto the level editor, compile, and write the following.

### Format
```
./test -load {amount of layers} {layer0.txt} {layer1.txt} 
```

### Example
```
./test -load 1 file0.txt
```

Keep in mind, to have both the latest versions of the game and the level editor.

## Create files and folders
The file explorer is accessible using the button in left corner of the navigation bar. You can create a new file by clicking the **New file** button in the file explorer. You can also create folders by clicking the **New folder** button.

## Compiling the game and opening the .exe file
To compile the game, write the following prompt onto the terminal. Keep in mind to also have and set the directory of where you have the repo located in your system onto the terminal 
```
skm clang++ -o test program.cpp
```
Once compiled, run the .exe file by writing the following prompt onto the same terminal
```
./test
```



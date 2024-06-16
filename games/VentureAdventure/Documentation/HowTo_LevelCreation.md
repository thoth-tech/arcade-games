# How to make a new level

Hello! Welcome to my short guide on how to add a new level to Venture Adventure!

It's not too tricky, but don't forget to reach out to your team if you need help.

## Step 1: Creating the initial file

- Create a copy of the file named **blanklevel.txt** that is in this documentation folder. This level can be used as a template.

- Rename the file in the format of **level1.txt**, but with whatever level number you are adding.

- Ideally, add your new level number in before the final level, as the last level spells out 'thanks for playing'. Someone may decide to change this in future.

- Move the file into the **/VentureAdventure/Resources/levels** folder.

- **Please ensure that level numbers are sequential (starting at 1) and that no numbers are skipped in this folder**

## Step 2: Design the Level

- Each type of tile has a particular number that it corresponds to. Please refer to the bottom of this document for more information.

- Using the blank level template, create your level by adjusting the numbers to your chosen tile numbers. Levels are made up of 16 x 16 tiles, there should always be 256 numbers in the file, separated by at least 1 space. These are generally arranged in 16 rows and 16 columns to make it easier to edit.

- Each level should always have a border of 'solid' tiles. Generally you should leave the border numbers that are already in the template (14 and 30), but you can change this if you are confident.

- Always make sure the campfire tile is in the same place, 9th row, 9th column, number 84. This is how the level is completed.

## Step 3: Changing the total levels in the game

- Navigate to the game.cpp file. In there is a variable named **totallevels**. You will need to update this number to reflect the new number of levels and save.

## Step 4: Testing the level

- Your level should now now be in the game to test

- You can adjust the **startinglevel** variable in program.cpp to jump straight to your level when the game starts, however I always recommend testing the whole game through to ensure nothing has gone wrong. Remember to change it back to '1' after testing

- This is also useful for testing your level while you are designing it, to check that it looks as you intended

## Tile Numbers

### Game Mechanic Tiles

**84**: Campfire tile - always at the 9th row, 9th column

**200**: Spider/enemy

**300**: Box

**400**: Gem

**0**: Grass

**1**: Grass with flowers

### Solid Tiles

Solid tiles are numbers greater than 1, and less than 300. These are tiles that the player cannot walk through (nor can enemies or boxes). This excludes numbers 84 (campfire), and 200 (spider) which are non-solid to allow for sprite collisions. The code for solid tiles can currently be found in the **game.cpp** file, in the **get_objects** method.

Please refer to the **solidtilenumbers.png** file in this documentation folder to view the main game tiles numbers, or refer to **/Resources/images/forest_tiles.png** to see them without the numbers.

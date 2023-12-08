# Class Descriptions and Decisions

Use this file to keep information about the different classes, such as what they are responsible for, and any important decisions made about their design that others working on the project should know. This should not be a substitute for leaving descriptive comments in your code.

## Program.cpp (Main)

## Block Class

- Block class will hold information about individual blocks such as colour, whether it's a 'breaker', whether it is 'moving' etc. It will be able to compare it's colour to surrounding blocks (this information will be used by the Board class to then manage the board when breaks occur).

- Has custom code for moving the sprite downwards to a destination, instead of using the SplashKit sprite move_to function which did not allow it to properly stop at the destination and still move smoothly.

- **Not using collisions**, just returning boolean data to tell if the sprite is moving. More straightforward than using listeners and callbacks. Probably also more accurate for block positioning than relying on collision data. Does mean that the bitmap files are quite specific to the pixel, so avoid changing these if possible. Height and widths are kept track of by the board rather than this block class.

- Will be able to compare it's colour to other blocks when asked by the board, can return a boolean true or false.

## Board Class

- Board class will be responsible for managing a lot of the gameplay logic. It holds the 2D array/vector of all block pointers and manages logic for how they are are placed, break and move. It will also have details about the board size and blocks size which the blocks themselves don't actually need to know.

- Board class will do all the calculations for where a block needs to go and pass this information onto the block. All the block needs to do is make sure the sprite gets to that location and the ability for the board to check if it's at it's destination yet or not.

- Board class will be able to ask blocks to compare colours to decide if breaks need to occur. Should receive a boolean true or false back. Will have some sort of loop to check for all adjacent blocks, and then all adjacent blocks for any that come back true in order to track the entire string.

- There are multiple phases to the way that the gameplay happens (e.g. blocks fall, blocks break which may cause other blocks to readjust etc.). Please refer to the game design documentation for more information about how these phases play out. The board class may be responsible for some of it, but there will likely be other classes involved in the processing. It's not a completely linear/procedural process, something will need to keep track of what phase the game is in, and when it's okay to change phases (such as when it's okay to spawn a new block).

## UserInput Class

- Will be responsible for handling user input. Will be expanded to include 2-player controls in future. Will need an actual .cpp file when this happens.

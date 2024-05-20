# Working Document for Game Design

Use this file to keep track of the current design of the game.
The main inspiration for the game is the [Swordfighting Puzzle](https://yppedia.puzzlepirates.com/Swordfight) from Puzzle Pirates. A demonstration of gameplay can be found [here](https://www.youtube.com/watch?v=tjZq-b0F2GQ).

This document isn't for assigning tasks, just keeping general track of game process and design. Use Teams/Trello/otherwise communicate with the team as to what you are working on.

## Phases of the game

Game phases will likely need to managed somewhere in an update function, where it checks what 'phase' the game is in and thus what to do next to update the game.

> **The phases are not necessarily linear and may repeat due to 'combos' occurring.**

We want to avoid a player having to deal with a new block to move when there's other stuff happening on the board, and also ensure that they can't move anything they aren't supposed to. In this sense there will not always be an 'active block'.

- **Player Phase:**

    The initial and probably standard phase of the game, where there is one active block (or later a couplet) that spawns and falls that the player can move. It is then placed into the grid array at the end of this phase. This phase should not start again until other phases have finished (sometimes the other phases may occur multiple times before this phase starts again).

- **Break Phase:**

    When a block/couplet has placed, there will checks to see whether any breaks will occur. If touching a 'breaker block', any connected blocks of the same colour will break simultaneously before the next phase begins. Player should not be able to move any blocks during this time, nor should a new block spawn. We will need to decide what to do in the situation that two breakers are touching relevant blocks - whether they will both break together, or one after the other (original game uses the simultanous option).

- **Readjustment Phase:**

    After blocks are broken, above blocks will need to fall down to fill the gaps. This should happen after the blocks have all broken and not at the same time. Player should not be able to move any blocks during this time, nor should a new block spawn. At the end of the Readjustment Phase, it is possible that another Break Phase could be initialized if there is now a different breaker touching relevant blocks as a result of the readjustment.

## Functionality/Features

### Still To Add

In a loose order:

- Game should end if a block can no longer spawn on the board

- Breakers need to randomly be generated instead of a block, with a lower chance of generating as opposed to a regular block.

- Breakers need to break same coloured blocks (and any connected same coloured blocks)

- After blocks are broken, other blocks in the board will fall to fill the gaps left behind if necessary. No new block should spawn until the board has readjusted itself (there may be multiple phases of this before a new block spawn, e.g. combos - see original game reference to combos in play)

- Blocks should fall in couplets, only separating once one of them reaches a block (see original game reference to see this mechanic in play)

- Couplets should be able to be rotated left or right to fall vertically or horizontally (like in Tetris). This means that their left/right or top/bottom order can be swapped. Need to decision if blocks should start vertically or horizontally

- Preview of next block to spawn to be added

- Rectangular patterns of blocks will create larger units of blocks that will always fall together (see original game reference to see this mechanic in play)

- Scoring mechanism to be added

- Start to develop 2-player functionality. Initially this can just be based on highest score or whoever loses first. This will involve signficant changes to the window size and board coordinates which needs to be carefully considered before implementing

- 2-player functionality to 'attack' other player through combos or large breaks. See game proposal document for more information about the grey/random blocks that get added to the board, as well as original game reference. Receiving attack blocks will need to be part of the game phases.

- Add music, update art

- Ensure full compatability with Arcade Machine

### Already Added

- Blocks will fall from the top of the game board and can be moved left, right or sped up by the player.

- Multiple blocks can be placed and can stack

- Blocks will be a randomly generated colour

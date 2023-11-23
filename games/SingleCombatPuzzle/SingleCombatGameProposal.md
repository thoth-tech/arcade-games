# Game Proposal

## Game Title

Single Combat

## Genre

Puzzle

## Game Overview

Single Combat is a multi-player puzzle game based on the [swordfighting puzzle](https://yppedia.puzzlepirates.com/Swordfight) in the massively multiplayer online game (MMO), [Puzzle Pirates](https://en.wikipedia.org/wiki/Puzzle_Pirates). It's got a similar feel to [Tetris](https://en.wikipedia.org/wiki/Tetris), with the main goal being for the player to position falling coloured blocks in such a way that they can create groups of a single colour that can be then be 'broken' to score points or to impede the opponent. The game is being adapted to have a medieval theme, hence the idea of 'single combat'. It's designed to be played with two people, but will have a 'training' mode for single-player that is scored with points.

## Game Mechanics

A quick way to get an idea of the game mechanics to have a look at this Puzzle Pirates swordfighting [tutorial video](https://www.youtube.com/watch?v=tjZq-b0F2GQ).

- **Falling Blocks**

      The primary mechanic of the game is that there are descending coloured blocks that fall in couplets. Each couplet is made of two separate block entities that can be positioned by the player, but are initially joined as they descend. These blocks can be rotated and moved left or right as they fall to allow the player to strategically place them on the board. Blocks are considered as 'placed' when they either reach the bottom of the board, or one (or both) of the couplet reach another block. Individual blocks will then fall according to gravity to the lowest point they can.

- **Breakers**

      In addition to the falling blocks, there are occasionally 'breakers' that are part of the couplet. These breakers are not block shaped, but are represented by an icon associated with the same coloured block. Breakers need to be placed like any other block, but if a breaker is placed adjacent to a block or group of blocks of the same colour/pattern, those blocks will 'break' and be removed from the board.

- **Creating Groups**

       The general aim of players is to position blocks so that they create groups of the same colour before 'breaking' them. The bigger the group, the larger the impact when the group is finally broken.

- **Gravity of blocks**

      When blocks are removed from the board due to breakers, surrounding blocks will fall down to fill the created gaps. Large groups of the same colour (four or more) that are created in a rectangular pattern will create bigger block units that will always fall together, making it possible to have gaps in the board.

- **Combos**

      The board can be set-up in such a way that the player can cause combos. This is in the sense that when a group is broken, it could cause other pieces on the board to fall in such a way that another break will occur in the same 'move'. A move is definied by one couplet being placed by the player.

- **Grey stone blocks (aka 'attacks')**

      As part of the player versus player (PvP) aspect of this game, grey stone blocks can fall randomly onto the board in varying amounts. This is caused by the opposing player creating combos or breaking large groups. These grey blocks will turn into random other coloured blocks after two moves, and thus hinder the player by reducing space and obstructing their combos. This means speed in placing blocks can be key.

- **Lose/Win conditions**

      The game is lost when the player's blocks reach the top of the centre of the board, such that there is no room for new block couplets to spawn, much like Tetris. In extreme scenarios this could even be caused by attacks from the other player. This means offense strategy while still maintaining space on the board can be vital. The game is won by outlasting the other player.

- **Formulas**

      At this time there haven't been decided formulas for single-player point scoring, how varying attacks are sent/spawned, or how couplets are randomized, but this will need to be nutted out during the development process, potentially through trial and error.

## Controls

The player will use the joystick to move couplets left or right, and use buttons to rotate blocks clockwise or anti-clockwise, as well as use a separate button to get blocks to drop instantly (like Tetris). The joystick currently won't be used to drop blocks to avoid accidents which could be detrimental to a player's game.

## Game Art

### Background

The background will be of a castle and medieval tournament ground. A couple of references below:

- [Castle and field](https://cdn1.vectorstock.com/i/1000x1000/80/40/medieval-battle-scene-in-cartoon-style-vector-42118040.jpg)

- [Medieval tournament ground](https://cdn3.vectorstock.com/i/1000x1000/69/02/empty-medieval-battle-scene-vector-41576902.jpg)

- [Pixel castle](https://images.squarespace-cdn.com/content/v1/551a19f8e4b0e8322a93850a/1555963806321-NC7RENATW8QTB9B75WFQ/Title_Image_16.png)

A pixel art style would be ideal to match other art, but there's room for debate.

### Blocks and Breakers

There will be four main kinds of blocks in a pixel style, compromised of four colours, each with a different icon that follows the medieval theme:

- Blue shield: [Icon reference](https://www.shutterstock.com/image-vector/shield-icon-pixel-design-gray-260nw-1296175603.jpg)

- Red sword: [Icon reference](https://cdn.vectorstock.com/i/preview-1x/64/78/sword-pixel-art-vector-43906478.jpg)

- Yellow 'cup of life': [Icon reference](https://thumbs.dreamstime.com/b/winner-pixel-trophy-cup-pixel-trophy-cup-240579038.jpg)

- Green horse: [Icon reference](https://www.shutterstock.com/image-vector/8-bit-pixel-horse-vector-260nw-1449653930.jpg)

- Grey blocks at different stages and block units: [Puzzle Pirates reference](https://yppedia.puzzlepirates.com/images/8/88/Crewswordfight.jpg)

### 'Mini-map'

Due to the arcade nature of this game meaning that both players will be able to see each other's boards, the ['mini-map' feature](https://yppedia.puzzlepirates.com/images/thumb/9/93/Swordfightchallenge.png/300px-Swordfightchallenge.png) will be excluded, but might be useful for future expansion in single-player.

## Game Sound

### Background Music

To suit the arcade nature of this game, Single Combat with have an 8-bit soundtrack. As the game is a puzzle game, intended to have high replayability, the soundtrack should be easy listening but still a bit catchy. There shuld be a few different pieces to allow for familiarity, but also ensure that the game doesn't become overly monotonous. The music should fit with the 'medieval battle' theme, but shouldn't be too intense (such as one might expect in an action game). Some possible references include the below:

- [Nine Valiants 8-Bit Version by Dave Eddy](https://www.youtube.com/watch?v=3x02rm1Dgoo)

- [Skyrim 8-Bit Cover by Floating Point](https://www.youtube.com/watch?v=fcFl6BgK_vw)

- [Mountain Trial by Joshua McLean](https://www.youtube.com/watch?v=L_OYo2RS8iU)

- [Undertale OST: 035 - Bird That Carries You Over A Disproportionately Small Gap](https://www.youtube.com/watch?v=Z51lfE2k7jU)

The background music should be in keeping with the tempo of the game itself, e.g. a similar pace to the falling blocks.

### Sound Effects

There should be sound effects for the following:

- When a couplet is placed
- When a breaker breaks a block/group
- When a combo occurs
- When a group unit (four or more blocks in a rectungular pattern) is formed
- When an attack is incoming/landed
- When the game is won
- When the game is lost

## Expansion Ideas

### Single-player with non-player character (NPC) opponents

One idea for expansion is to incorporate a single-player mode that allows players to play against an NPC. This could also incorporate varying levels of difficulty (e.g. based on how severely their 'attacks' impede the player concerning amount of grey blocks that fall, or frequency). This could even include even boss fights, such as against a 'knight'. Soundtracks like in Undertale or Scott Pilgrim vs The World: The Game, could be useful references for boss fight music.

### Power-ups or Effects for large breaks/combos

Another idea for an expansion of this game is to incorporate power-ups, occuring as a reward for breaking large groups of blocks, or creating combos. An example of a power-up might be that the next few couplets will all be the same colour, having block descent slowed (or sped up for the opponent), an increased chance of getting a 'breaker', so some multiplier added to an attack. This could also be incorporated into boss fights, with bosses having different abilities.

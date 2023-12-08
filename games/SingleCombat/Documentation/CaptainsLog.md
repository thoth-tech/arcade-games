# Captain's Log

9/12/2023 -  Castiel thoughts on next steps and TODOs:

- Revise current code to encapsulate everything a bit more. I have made some TODO comments for myself with ideas on how to separate some code out into functions further. There is particular work to do in the Board class.

- Board class in particular I need to simplify/break up the code that figures out all the active row, column and destination stuff into nicer methods. Most recent UML diagram will help with this a bit.

- Change it so board class can pass exact coordinate calculation to the block class, instead of block class needing to figure it out. Means block doesn't need to know it's height and width. This should be a quickish one

- Bigger Trello Card which is to add code so that block can't move left or right if there is another block there. Best to do this after fixing the above which should hopefully make adding this functionality easier. Use modulo for this to figure out if it's in exactly in the right position or not, as I think this will be useful for later when you can slot blocks into gaps.

- Make a boolean function in block class to compare colour to another given block.

- Add breakers into the block class (I believe this just needs to be an attribute) and have a percentage of blocks spawn as a breaker. Add a function for the board class to be able to get the breaker value from the block. Need to think about this -  a check needs to be performed everytime a new block is placed, firstly to see if it's a breaker (if not, can ignore), if it is, checks for surrounding colours and then potentially performs a break and starts that phase. For now just start getting those checks working and don't worry about the breakers and falling phases yet. Need to start separating this into more functions and maybe even classes. Don't want that update function in board to be too long. Block definitely needs to know if it's a breaker as this affects the sprite bitmap. Also I think needs to know because combos could happen. Board couldn't keep track of multiple breakers and their locations without being able to 'get' that information whenever it needs to check.

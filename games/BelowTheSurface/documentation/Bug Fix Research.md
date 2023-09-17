# Below The Surface Bug Fixing Documentation

This is a ongoing list to update with any information regarding bugs found.

## Ladder Behaviour

As of the finalized build version of Below the Surface, Players can't stand on the top of ladders and if you try to you will fall straight to the bottom. this makes some sections of the game much more frustrating and harder than they should be.

So far a temporary fix implemented for this is to raise the collision detection by 15 units (found in Block.h, line 275), this gives the player a bit more height in dismounting the ladders, but also causes a slight visual bug with the player being able to climb a little higher than the ladder block.

### Main Issues

Through research and attempts on correcting the issue, The main problem was the collision detection in the game is a boolean value, the player is either colliding or they're not, and the ladders work originally by checking if the player is colliding with a ladder block, and then if they input up or down they will enter a climbing state until they're no longer colliding with a ladder block.
this is where I got stuck as I could never figure out a way to detect when the player was standing on the top of the ladder without causing that same detection method to activate on every block of the ladder.

### Fixes Tried

I was able to get the player to stand on top of the ladder by implementing the directional collision detection found in the SolidBlock class in Block.h into the ladder class in block.h. using this I could detect the players collision. within collision.h, using this when the player would collide with the top of the ladder I used set_on_floor and the player would now stand on top of the ladder.
the issue I then found was that once this happened, the original ladder collision would activate meaning that the player is now stuck to the top of the ladder as pressing up would try to jump and put the player into a climbing state, meaning the player is now stuck to the top of the ladder and cant move.

I tried to implement a check so that the directional collision would check first and if the player was on top of a ladder block it would enter the floor state and wouldn't check for entering the climbing state from the regular collision, this just caused the player to get stuck the same way as before but to the top of every ladder block.

### Fixes To Look Into

- Trying to rebuild the collision so that it can check if the player is touching many blocks.

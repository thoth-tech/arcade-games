# Features and Refinements
## Features left to add

 - Dynamic music which plays a particular track on situation (ex: large amount of enemies play a higher tempo track)
 - Increase damage and size mechanic. Easy to implement with current framework, just need to increase player size and projectile damage when killing an enemy.
 - Colliders for screen so that player is unable to leave screen bounds. Not needed for enemy due to spawn system.
 - PowerUps and DownGrades. Implement a system where certain enemy drops collectable powerups which can allow player to use an alternative fire (mapped on another button) which upon hit by an enemy the player loses.
 - Points and Leaderboard system. Gain points for killing enemies and add to total at end of level.
 - Additional selectable planes for the player on start screen which have different firing patterns.
## Refinements needed
- Changing of collision detection of player and enemies to avoid too many checks. Current systems updates every refresh so is quite intensive. this can be changed to use another type of collision method.
- Separation of `CreateLevel()` to a separate new class so that there is more clarity of code.
- Changing of coordinates of enemies/player to be consistent across screen using `SplashKit.ScreenHeight()` and `SplashKit.ScreenWidth()`.


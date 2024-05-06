# Explanation of Classes and Functionality
## GameStateManager
The GameStateManager class is the central controller of the game. It is responsible for managing the overall game state which is used to initialise, exit and play. It sets up the entire environment including player and enemies locations with level design as well.

**Methods within class**
`public void Update()`

 - Used to switch between the `currentState` in order to move within the
   different prescribed states.

`private void DrawMenu()`

 - This method draws all environments for different states of the game,
   also setting up the playing environment.

`public void StartGame()`

 - Handles player start position and enemy level setup.

`private void DrawEnemies()`

 - Sets up enemy movement, shooting and drawing capabilities.

`private void CreateLevel()`

 - Contains all information of enemy predefined start positions to
   create a level inside the game.
## Player
The player class handles all movement, firing, collision and damage capabilities of the game character. Certain aspects such as health, start position, and damage can be adjusted for further capabilities

**Methods within class**
`public void Draw()`

 - Draws the player's sprite and updates the bullet position, removing
   them as they go off-screen.

`public void Update(List<Enemy> enemies)`

 - Checks the collision and position of bullets. If a collision takes
   place it damages the enemy and removes the bullet.

`public void Move()`

 - Moves the player based on arrow keys at a constant speed.

`public void Shoot()`

 - Allows shooting capabilities by pressing space to shoot, creating a
   new bullet and adding to a list.

`public void TakeDamage(int damage)`

 - Reduces the player health by a specified amount.
## Enemy
This class handles enemy related functionalities such as movement, shooting, following and collision handling with similar editable functionality such as the player. It has added timers to change the bullet shooting interval as well as the movement interval.

**Methods within class**
`public void Draw()`

 - Draws enemy sprite on the screen and updates position of bullets same
   functionality of removing off-screen as player.

`public void Move()`

 - Controls movement of enemy based on a timer allowing vertical
   movement at specific intervals.

`public void Shoot()`

 - Similar shooting method related to the player but fires on bases of
   another timer with an editable interval.

`public void Update(Player player)`

 - Similar functionality of collision detection but takes in the Player
   class.

`public void Follow(Player player)`

 - Makes the enemy follow the player by taking in player position.

`public void TakeDamage(int damage)`

 - Same functionality as player but takes damage number from player
   bullet amount.

`private bool IsOnScreen()`

 - Checks if the enemy is within the screen boundaries.
## Bullet
This class handle the projectiles fired by the player and enemy classes. It contains the speed, damage, coordinates and the bitmap for the bullets. It also handles the collision detection for both player and enemy.

**Methods within class**
`public void Move()`

 - Updates the position of the bullet moving the bullet at a constant
   speed and also drawing the bitmap.

`public bool CheckCollisionP(Enemy enemy, Bullet bullet)`

 - Collision detection between the bullet and an enemy for the player
   class.

`public bool CheckCollisionE(Player player, Bullet bullet)`

 - Collision detection between the bullet and a player for the enemy
   class.
## Program
The program class serves as an entry point to the game and only contains the `Main()` for functionality.

**Methods within class**
`Main()`

 - Opens the window for the application also toggling fullscreen mode.
   Creates an instance of the GameStateManager class and enters a while
   loop which ends if the user exits the application.

using System;
using SplashKitSDK;



namespace TD
{

    class Player
    {
        public int x;
        public int y;
        public int score;
        public bool win;

        public Player(Field field)
        {//srandomise player spawn location
            Random rnd = new Random();
            this.x = rnd.Next(field.size);
            this.y = rnd.Next(field.size);
            score = 0;
            win = true;
        }
    }
    class Enemy
    {
        public int x, y;
        public Enemy(Field field)
        {//randomise enemy spawn location
            Random rnd = new Random();
            this.x = rnd.Next(field.size);
            this.y = rnd.Next(field.size);
        }
    }

    class Gem
    {
        public int x;
        public int y;
        public int value;
        public Gem(Field field)
        {//randomise the gem spawn location
            Random rnd = new Random();
            this.x = rnd.Next(field.size);
            this.y = rnd.Next(field.size);
            value = 20;
        }
        //public void rnd_cord(Field field)
        //{
        //    Random rnd = new Random();
        //    this.x = rnd.Next(field.size);
        //    this.y = rnd.Next(field.size);
        //}
    }

    class Field
    {
        public int size = 10;
        int[,]? array;

        public void draw(Game game, int EnemyCount, bool EnemyMoveDiagonal, int EnemySkipTurnPercent)
        {
            //this method draws the game board and all the active entities on it
            Console.Clear();
            array = new int[size, size];
            Console.WriteLine("-----------------------------------------------------------------");
            Console.WriteLine("");
            Console.WriteLine("You, the player, are the 'X'.");
            Console.WriteLine("Your objective, the gems, are the '$'.");
            Console.WriteLine("The enemies are the '#'.");
            Console.WriteLine("");
            Console.WriteLine("The current enemy count is: " + EnemyCount.ToString());
            Console.WriteLine("Enemy Diagonal Movement is: " + EnemyMoveDiagonal.ToString());
            Console.WriteLine("The chance for the enemy/enemies to skip their turn is: " + EnemySkipTurnPercent.ToString() + "%");
            Console.WriteLine("");
            Console.WriteLine("Your objective is to collect the gems and avoid the enemies.");
            Console.WriteLine("");
            for (int i = 0; i < array.GetLength(0); i++)
            {
                for (int j = 0; j < array.GetLength(1); j++)
                {

                    if (i == game.player.x && j == game.player.y)
                    {
                        Console.Write("|X|");
                    }
                    else if (i == game.gem.x && j == game.gem.y)
                    {
                        Console.Write("|$|");
                    }
                    else if (i == game.enemy.x && j == game.enemy.y)
                    {
                        Console.Write("|#|");
                        // the additional enemies are only 'activated' when the user adds more
                        // enemies through the difficulty settings
                    }else if(EnemyCount > 1 && i == game.enemy2.x && j == game.enemy2.y)
                    {
                        Console.Write("|#|");
                    }else if(EnemyCount > 2 && i == game.enemy3.x && j == game.enemy3.y)
                    {
                        Console.Write("|#|");
                    }else if(EnemyCount > 3 && i == game.enemy4.x && j == game.enemy4.y)
                    {
                        Console.Write("|#|");
                    }
                    else
                    {
                        Console.Write("| |");
                    }
                }
                Console.WriteLine("");
            }
        }
    }

    class Game
    {
        public Player player;

        //While there are 4 enemy objects here, the various methods that interact with the enemies
        //will use the enemycount setting to determine how many enemy objects are actually used
        //during gameplay.

        public Enemy enemy;
        public Enemy enemy2;
        public Enemy enemy3;
        public Enemy enemy4;
        public Field field;
        public Gem gem;

        public Game()
        {
            field = new Field();
            player = new Player(field);
            gem = new Gem(field);
            enemy = new Enemy(field);
            enemy2 = new Enemy(field);
            enemy3 = new Enemy(field);
            enemy4 = new Enemy(field);
        }
        public void gem_grab()
        {
            if (player.x == gem.x && player.y == gem.y)
            {
                //this method despawns the gem when the player picks it up, 
                //gives the player 20 score for picking it up, then creates a new gem
                gem = new Gem(field);
                player.score += 20;
            }
        }

        public void enemy_move_individual(bool EnemyMoveDiagonal, Enemy SelectedEmeny)
        {
            if(EnemyMoveDiagonal)
            {//if enemy diagonal movement is enabled, the selected enemy moves towards the player
                if (player.x > SelectedEmeny.x)
                {
                    SelectedEmeny.x++;
                }
                else if (player.x < SelectedEmeny.x)
                {
                    SelectedEmeny.x--;
                }//the separation of enemy movement into two if else statements visible here
                //is what allows enemy diagonal movement 
                if (player.y > SelectedEmeny.y)
                {
                    SelectedEmeny.y++;
                }
                else if (player.y < SelectedEmeny.y)
                {
                    SelectedEmeny.y--;
                }
            }else
            {//if enemy diagonal movement is disabled, the selected enemy moves towards the player,
            //but without diagonal movement options
                if (player.x > SelectedEmeny.x)
                {
                    SelectedEmeny.x++;
                }
                else if (player.x < SelectedEmeny.x)
                {
                    SelectedEmeny.x--;
                }
                else if (player.y > SelectedEmeny.y)
                {
                    SelectedEmeny.y++;
                }
                else if (player.y < SelectedEmeny.y)
                {
                    SelectedEmeny.y--;
                }
            }
        }
        public void enemy_move(bool EnemyMoveDiagonal, int EnemyCount)
        {
            //here, the individual enemies that are currently 'active' based on the 'enemy count' 
            //difficulty setting are moved towards the player via the enemy_move_individual method
            enemy_move_individual(EnemyMoveDiagonal, enemy);
            if(EnemyCount > 1)
            {
                enemy_move_individual(EnemyMoveDiagonal, enemy2);
            }
            if(EnemyCount > 2)
            {
                enemy_move_individual(EnemyMoveDiagonal, enemy3);
            }
            if(EnemyCount > 3)
            {
                enemy_move_individual(EnemyMoveDiagonal, enemy4);
            }
        }
        public void win_check(int EnemyCount)
        {
            //checks the lose condition of the 'active' enemy touching the player
            if (player.x == enemy.x && player.y == enemy.y)
            {
                Console.WriteLine("game over");
                player.win = false;
            }
            if(EnemyCount > 1 && player.x == enemy2.x && player.y == enemy2.y)
            {//the additional enemies are only 'active' for the lose condition 
            //checking if the player has changed the 'enemy count' difficulty 
            //setting add/activate the enemies
                Console.WriteLine("game over");
                player.win = false;
            }
            if(EnemyCount > 2 && player.x == enemy3.x && player.y == enemy3.y)
            {
                Console.WriteLine("game over");
                player.win = false;
            }
            if(EnemyCount > 3 && player.x == enemy4.x && player.y == enemy4.y)
            {
                Console.WriteLine("game over");
                player.win = false;
            }
        }
        public void gameplay(bool EnemyMoveDiagonal, int EnemySkipTurnPercent, int EnemyCount)
        {
            Random rnd = new Random();
            gem_grab();//check for gem grabbing
            switch(EnemySkipTurnPercent)
            {
                //here, the chance for the enemy to skip its turn is calculated, based on the
                //'EnemySkipTurnPercent' setting in thge difficulty settings
                case 0:
                    enemy_move(EnemyMoveDiagonal, EnemyCount);
                    //once it has beeen determined that the enemy does not skip its turn, the
                    //'EnemyMoveDiagonal' and 'EnemyCount' difficulty settings are sent as parameters
                    //into the enemy_move method to help determine how the enemy will move based on 
                    //the current difficulty settings 
                break;
                case 25:
                    if (rnd.Next(4) != 0)
                        enemy_move(EnemyMoveDiagonal, EnemyCount);
                break;
                case 50:
                    if (rnd.Next(2) == 0)
                        enemy_move(EnemyMoveDiagonal, EnemyCount);
                break;
                case 75:
                    if (rnd.Next(4) == 0)
                        enemy_move(EnemyMoveDiagonal, EnemyCount);
                break;
                default:
                    if (rnd.Next(2) == 0)
                        enemy_move(EnemyMoveDiagonal, EnemyCount);
                break;
            }   
            win_check(EnemyCount);//lose condition is checked after enemy movement
        }
        public void move(bool EnemyMoveDiagonal, int EnemySkipTurnPercent, int EnemyCount)
        {
            field.draw(this, EnemyCount, EnemyMoveDiagonal, EnemySkipTurnPercent);
            do
            {//here the game responds to user inputs to either move the player character
            //or respond with why the input was not possible/valid
                try
                {
                    Console.WriteLine("\nUse the arrow keys to move or press X to return to the main menu.\n");

                    var choice = Console.ReadKey(true).Key;
                    
                    switch (choice)
                    {
                        case ConsoleKey.UpArrow:
                            if (player.x > 0)
                            {
                                player.x -= 1;
                                Console.WriteLine("");
                                gameplay(EnemyMoveDiagonal, EnemySkipTurnPercent, EnemyCount);
                                field.draw(this, EnemyCount, EnemyMoveDiagonal, EnemySkipTurnPercent);
                            }
                            else
                                throw new InvalidOperationException("\nOh! You hit the top wall! Try moving in another direction.\n");

                            break;
                        case ConsoleKey.DownArrow:
                            if (player.x < field.size - 1)
                            {
                                player.x += 1;
                                Console.WriteLine("");
                                gameplay(EnemyMoveDiagonal, EnemySkipTurnPercent, EnemyCount);
                                field.draw(this, EnemyCount, EnemyMoveDiagonal, EnemySkipTurnPercent);
                            }
                            else
                                throw new InvalidOperationException("\nOh! You hit the bottom wall! Try moving in another direction.\n");

                            break;
                        case ConsoleKey.LeftArrow:
                            if (player.y > 0)
                            {
                                player.y -= 1;
                                Console.WriteLine("");
                                gameplay(EnemyMoveDiagonal, EnemySkipTurnPercent, EnemyCount);
                                field.draw(this, EnemyCount, EnemyMoveDiagonal, EnemySkipTurnPercent);
                            }
                            else
                                throw new InvalidOperationException("\nOh! You hit the left wall! Try moving in another direction.\n");

                            break;
                        case ConsoleKey.RightArrow:
                            if (player.y < field.size - 1)
                            {
                                player.y += 1;
                                Console.WriteLine("");
                                gameplay(EnemyMoveDiagonal, EnemySkipTurnPercent, EnemyCount);
                                field.draw(this, EnemyCount, EnemyMoveDiagonal, EnemySkipTurnPercent);
                            }
                            else
                                throw new InvalidOperationException("\nOh! You hit the right wall! Try moving in another direction.\n");

                            break;
                        case ConsoleKey.X:
                            Console.WriteLine("Returning to main menu.");
                            return;
                        default:
                            throw new InvalidOperationException("Invalid option, Please try again.");
                    }
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
            } while (player.win);
        }

        public int[] Menu(bool EnemyMoveDiagonal, int EnemySkipTurnPercent, int EnemyCount)
        {
            //this is the method of the menu and changing difficulty settings
            int i = 0;
            int DifficultyOption = 0;

            int[] output = new int[4]{0,1,1,50};
            //this array is the output being returned, as there are multiple difficulty settings
            //here that can be changed. 
            //Index 0 of the array is for whether the user is quitting, with 0 being false 
            //and 1 being true.
            //Index 1 is for enemy count and ranges from 1 to 4 enemies.
            //Index 2 is for enabling/disabling enemy diagonal movement, with 0 being false 
            //and 1 being true.
            //Index 3 represents the chance for the enemies to skip their turn, with valid integers being
            //0 for 0%, 25 for 25%, 50 for 50%, and 75 for 75%.

            Console.WriteLine("-----------------------------------------------------------------");
            Console.WriteLine("");

            Console.WriteLine("The current enemy count is: " + EnemyCount.ToString());
            Console.WriteLine("Enemy Diagonal Movement is: " + EnemyMoveDiagonal.ToString());
            Console.WriteLine("The chance for the enemy/enemies to skip their turn is: " + EnemySkipTurnPercent.ToString() + "%");
            Console.WriteLine("");

            Console.WriteLine("Start a new game, change difficulty settings, or quit?");
            Console.WriteLine("");
            Console.WriteLine("Press UP to start a new game");
            Console.WriteLine("Press RIGHT to change difficulty settings");
            Console.WriteLine("Press DOWN to quit");

            do
            {
                var choice = Console.ReadKey(true).Key;
                
                if (choice == ConsoleKey.UpArrow)
                {
                    output[0] = 0;
                    break;
                }
                else if (choice == ConsoleKey.DownArrow)
                {
                    output[0] = 1;
                    break;
                }
                else if (choice == ConsoleKey.RightArrow)
                {
                    output[0] = 0;
                    DifficultyOption +=1;
                    break;
                }
                Console.WriteLine("Invalid option, Please try again");
            }while(i == 0);

            //these below are for changing the difficulty settings and only display if the user 
            //choosese to change said difficulty settings

            if(DifficultyOption > 0)
            {

                Console.WriteLine("-----------------------------------------------------------------");
                Console.WriteLine("");
                Console.WriteLine("How many enemies should there be?");
                Console.WriteLine("Press LEFT for 1 enemy");
                Console.WriteLine("Press UP for 2 enemies");
                Console.WriteLine("Press RIGHT for 3 enemies");
                Console.WriteLine("Press DOWN for 4 enemies");

                do
                {
                    var choice = Console.ReadKey(true).Key;
                    
                    if (choice == ConsoleKey.LeftArrow)
                    {
                        output[1] = 1;
                        break;
                    }
                    else if (choice == ConsoleKey.UpArrow)
                    {
                        output[1] = 2;
                        break;
                    }
                    else if (choice == ConsoleKey.RightArrow)
                    {
                        output[1] = 3;
                        break;
                    }
                    else if (choice == ConsoleKey.DownArrow)
                    {
                        output[1] = 4;
                        break;
                    }
                    Console.WriteLine("Invalid option, Please try again");
                }while(i == 0);
            }

            if(DifficultyOption > 0)
            {
                Console.WriteLine("-----------------------------------------------------------------");
                Console.WriteLine("");
                Console.WriteLine("Should the enemies be able to move diagonally?");
                Console.WriteLine("Press UP to enable enemy diagonal movement");
                Console.WriteLine("Press DOWN to disable enemy diagonal movement");

                do
                {
                    var choice = Console.ReadKey(true).Key;
                    
                    if (choice == ConsoleKey.UpArrow)
                    {
                        output[2] = 1;
                        break;
                    }
                    else if (choice == ConsoleKey.DownArrow)
                    {
                        output[2] = 0;
                        break;
                    }
                    Console.WriteLine("Invalid option, Please try again");
                }while(i == 0);
            }

            if(DifficultyOption > 0)
            {
                Console.WriteLine("-----------------------------------------------------------------");
                Console.WriteLine("");
                Console.WriteLine("How high should the chance for enemies to skip their turn be?");
                Console.WriteLine("Press LEFT for 0%");
                Console.WriteLine("Press UP for 25%");
                Console.WriteLine("Press RIGHT for 50%");
                Console.WriteLine("Press DOWN for 75%");

                do
                {
                    var choice = Console.ReadKey(true).Key;
                    
                    if (choice == ConsoleKey.LeftArrow)
                    {
                        output[3] = 0;
                        break;
                    }
                    else if (choice == ConsoleKey.UpArrow)
                    {
                        output[3] = 25;
                        break;
                    }
                    else if (choice == ConsoleKey.RightArrow)
                    {
                        output[3] = 50;
                        break;
                    }
                    else if (choice == ConsoleKey.DownArrow)
                    {
                        output[3] = 75;
                        break;
                    }
                    Console.WriteLine("Invalid option, Please try again");
                }while(i == 0);
            }

            return output;

        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            bool EnemyMoveDiagonal = true;
            int EnemySkipTurnPercent = 50; 
            int EnemyCount = 1;
            int[] menuOutput;
            Game game = new Game();
            {

                bool quitChoice = false;

                while(quitChoice == false)
                {//putting the output difficulty settings into their own variables
                    game = new Game();//a new game is created here to respawn the various
                    //entities and prevent their positions from carrying over to future games
                    menuOutput = game.Menu(EnemyMoveDiagonal, EnemySkipTurnPercent, EnemyCount);
                    if(menuOutput[0]==0)
                    {
                        quitChoice = false;
                    }else
                    {
                        quitChoice = true;
                    }

                   EnemyCount = menuOutput[1];

                    if(menuOutput[2]==0)
                    {
                        EnemyMoveDiagonal = false;
                    }else
                    {
                        EnemyMoveDiagonal = true;
                    }

                    EnemySkipTurnPercent = menuOutput[3];

                    if (quitChoice == false)
                    {
                        Console.Clear();
                        game.player.win = true;
                        Console.WriteLine("-----------------------------------------------------------------");
                        Console.WriteLine("");
                        game.move(EnemyMoveDiagonal, EnemySkipTurnPercent, EnemyCount);

                        Console.WriteLine("");//difficulty settings are shown alongside the score
                        Console.WriteLine("The enemy count was: " + EnemyCount.ToString());
                        Console.WriteLine("Enemy Diagonal Movement was: " + EnemyMoveDiagonal.ToString());
                        Console.WriteLine("The chance for the enemy/enemies to skip their turn was: " + EnemySkipTurnPercent.ToString() + "%");

                        Console.WriteLine("");
                        Console.WriteLine("your score was: " + game.player.score);
                    }
                }
            }
        }
    }
}

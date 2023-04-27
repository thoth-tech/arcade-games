using System;

namespace TD
{
    class Player
    {
        public int x;
        public int y;
        public int score;
        public bool win;

        public Player(Field field)
        {
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
        {
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
        {
            Random rnd = new Random();
            this.x = rnd.Next(field.size);
            this.y = rnd.Next(field.size);
            value = 20;
        }
        public void rnd_cord(Field field)
        {
            Random rnd = new Random();
            this.x = rnd.Next(field.size);
            this.y = rnd.Next(field.size);
        }
    }
    class Field
    {
        public int size = 10;
        int[,] array;

        public void draw(Game game)
        {
            array = new int[size, size];
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
        public Enemy enemy;
        public Field field;
        public Gem gem;

        public Game()
        {
            field = new Field();
            player = new Player(field);
            gem = new Gem(field);
            enemy = new Enemy(field);
        }
        public void gem_grab()
        {
            if (player.x == gem.x && player.y == gem.y)
            {
                gem = new Gem(field);
                player.score += 20;
            }
        }
        public void enemy_move()
        {
            if (player.x > enemy.x)
            {
                enemy.x++;
            }
            else if (player.x < enemy.x)
            {
                enemy.x--;
            }
            if (player.y > enemy.y)
            {
                enemy.y++;
            }
            else if (player.y < enemy.y)
            {
                enemy.y--;
            }
        }
        public void win_check()
        {
            if (player.x == enemy.x && player.y == enemy.y)
            {
                Console.WriteLine("game over");
                player.win = false;
            }
        }
        public void gameplay()
        {
            Random rnd = new Random();
            gem_grab();
            if (rnd.Next(2) == 0)
                enemy_move();
            win_check();
        }
        public void move()
        {
            field.draw(this);
            do
            {
                try
                {
                    Console.WriteLine("\nuse arrow keys to move\n");

                    var choice = Console.ReadKey(true).Key;
                    switch (choice)
                    {
                        case ConsoleKey.UpArrow:
                            if (player.x > 0)
                                player.x -= 1;
                            else
                                throw new InvalidOperationException("\nOh !!! you hit the upper wall\n");

                            break;
                        case ConsoleKey.DownArrow:
                            if (player.x < field.size - 1)
                                player.x += 1;
                            else
                                throw new InvalidOperationException("\nOh !!! you hit the bottom wall\n");

                            break;
                        case ConsoleKey.LeftArrow:
                            if (player.y > 0)
                                player.y -= 1;
                            else
                                throw new InvalidOperationException("\nOh !!! you hit the left wall\n");

                            break;
                        case ConsoleKey.RightArrow:
                            if (player.y < field.size - 1)
                                player.y += 1;
                            else
                                throw new InvalidOperationException("\nOh !!! you hit the right wall\n");

                            break;
                        case ConsoleKey.X:
                            Console.WriteLine("back to main menu");
                            return;
                        default:
                            throw new InvalidOperationException("please enter suitable key.");
                    }
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
                finally
                {
                    Console.WriteLine("");
                    gameplay();
                    field.draw(this);
                }
            } while (player.win);
        }
    }


    class Program
    {
        static void Main(string[] args)
        {
            Game game = new Game();
            {
                game.move();
                Console.WriteLine("your score -- " + game.player.score);
            }
        }
    }
}

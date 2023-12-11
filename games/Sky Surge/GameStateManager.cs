using System;
using System.Security.Cryptography.X509Certificates;
using SplashKitSDK;

namespace Sky_Surge
{
    public enum GameStates
    {
        Menu,
        Playing,
        Exit
    }

    class GameState
    {
        private Player player=new Player(0,0);
        private List<Enemy> enemies;
        private Color backgroundColor = Color.White;
        public GameStates currentState;

        public GameState()
        {
            currentState = GameStates.Menu;
            enemies = new List<Enemy>();
        }

        public void Update() 
        {
            switch(currentState)
            {
                case GameStates.Menu:
                    DrawMenu();
            if (SplashKit.KeyTyped(KeyCode.XKey))
            {
                currentState = GameStates.Exit;
            }
            else if (SplashKit.KeyTyped(KeyCode.SKey))
            {
                currentState = GameStates.Playing;
                StartGame();
                
            }
                    break;

                case GameStates.Playing:
                    DrawMenu();
                    
                    
                    break;

                case GameStates.Exit:
                    Environment.Exit(0);
                    
                    break;

                default:
                    break;
            }
        }

        private void DrawMenu()
        {
             
            if (currentState == GameStates.Menu)
            {
            
           
            SplashKit.ClearScreen(backgroundColor);
            
            SplashKit.DrawText("Welcome to Sky Surge !!", Color.Black, "Arial", 90, 700, 600);
            SplashKit.DrawText("Press X to exit", Color.Black, "Arial", 90, 730, 700);
            }
            else if (currentState == GameStates.Playing)
            {
                SplashKit.ClearScreen(Color.Blue);
                player.Draw();
                player.Move();
                player.Shoot(enemies);
                player.Update(enemies);
                DrawEnemies();
                
                
                
            }

            SplashKit.RefreshScreen();
        }

         public void StartGame()
        {
            player = new Player(770, 700);
            enemies.Add(new Enemy(400, 300, 30, 500, 5));
            enemies.Add(new Enemy(100, 300, 30, 100, 5));
            
            
        }
       
        private void DrawEnemies()
        {
            for (int i = enemies.Count - 1; i >= 0; i--)
            {
                var enemy = enemies[i];
                enemy.Draw();
                
                enemy.SetPlayer(player);

                if (enemy.health <= 0)
                {
                    Console.WriteLine("Enemy destroyed");
                    enemies.RemoveAt(i);
                }
            }
        }

        






    }



 }
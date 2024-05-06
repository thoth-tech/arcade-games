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

    class GameStateManager
    {
        private Player _player;
        private List<Enemy> _enemies;
        private Color backgroundColor = Color.White;
        public GameStates currentState;
        private Bitmap _startmenu;

        public GameStateManager()
        {
            currentState = GameStates.Menu;
            _enemies = new List<Enemy>();
            _player = new Player(0, 0, 0);
            _startmenu = SplashKit.LoadBitmap("menuBackground", "menuimg.png");
        }

        public void Update()
        {
            switch (currentState)
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
                    if (SplashKit.KeyTyped(KeyCode.XKey))
                    {
                        currentState = GameStates.Exit;
                    }
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
                SplashKit.DrawBitmap(_startmenu, 0, 50);
                SplashKit.DrawText("Welcome to Sky Surge !!", Color.White, "Arial", 90, 700, 600);
                SplashKit.DrawText("Press S to Start", Color.White, "Arial", 90, 725, 550);
                SplashKit.DrawText("Press X to exit", Color.White, "Arial", 90, 730, 700);
            }
            else if (currentState == GameStates.Playing)
            {
                SplashKit.ClearScreen(Color.Blue);
                _player.Draw();
                _player.Move();
                _player.Shoot();
                _player.Update(_enemies);
                DrawEnemies();
                if (_player._health <= 0)
                {
                    currentState = GameStates.Menu;
                }
            }
            SplashKit.RefreshScreen();
        }

        public void StartGame()
        {
            _player = new Player(770, 700, 1);
            CreateLevel();
        }

        private void DrawEnemies()
        {
            for (int i = _enemies.Count - 1; i >= 0; i--)
            {
                _enemies[i].Draw();
                _enemies[i].Move();
                _enemies[i].Shoot();
                _enemies[i].Update(_player);
                if (_enemies[i].health <= 0)
                {
                    _enemies.RemoveAt(i);
                }
                if (_enemies.Count == 0)
                {
                    currentState = GameStates.Menu;
                }
            }
        }
        private void CreateLevel()
        {
            _enemies.Add(new Enemy(500, -40, 5, 15, 8));
            _enemies.Add(new Enemy(600, -40, 5, 15, 8));
            _enemies.Add(new Enemy(700, -40, 5, 15, 8));
            _enemies.Add(new Enemy(600, -40, 5, 26, 8));
            _enemies.Add(new Enemy(800, -40, 5, 26, 8));
            _enemies.Add(new Enemy(900, -40, 5, 26, 8));
            _enemies.Add(new Enemy(1200, -40, 5, 26, 7));
            _enemies.Add(new Enemy(100, -40, 5, 32, 7));
            _enemies.Add(new Enemy(200, -40, 5, 30, 7));
            _enemies.Add(new Enemy(800, -40, 5, 43, 5));
            _enemies.Add(new Enemy(600, -40, 5, 43.5, 5));
            _enemies.Add(new Enemy(350, -40, 5, 44, 5));
            _enemies.Add(new Enemy(200, -40, 5, 45, 5));
            _enemies.Add(new Enemy(1000, -40, 5, 60, 4));
            _enemies.Add(new Enemy(1200, -40, 5, 60, 4));
            _enemies.Add(new Enemy(1600, -40, 5, 60, 4));
            _enemies.Add(new Enemy(0, -40, 5, 72, 3));
            _enemies.Add(new Enemy(100, -40, 5, 72, 3));
            _enemies.Add(new Enemy(200, -40, 5, 72, 3));
            _enemies.Add(new Enemy(800, -40, 5, 80, 2));
            _enemies.Add(new Enemy(700, -40, 5, 80, 2));
            _enemies.Add(new Enemy(650, -40, 5, 80, 1));
            _enemies.Add(new Enemy(1300, -40, 5, 90, 1));
            _enemies.Add(new Enemy(1200, -40, 5, 90, 1));
            _enemies.Add(new Enemy(500, -40, 5, 90, 1));
        }
    }
}
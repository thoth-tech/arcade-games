using System;
using SplashKitSDK;

namespace AsteroidsGame
{
    public class Program
    {
        private static bool _GameStarted, _GameExit;
        public static double gameScale;

        public static void Main()
        {
            game_setup setup = new game_setup();
            Window gameWindow = setup.gameWindow;
            //Window gameWindow = new Window("Asteroids", 1600, 900);
            //gameWindow.ToggleBorder();
            Game Game = null;
            Menu Menu = new Menu(gameWindow);
            Controls controls = new Controls();

            FrameTickLog ftl = new FrameTickLog(gameWindow, "pricedown_bl", 50);
            ftl.Start();

            _GameStarted = false;
            while (!_GameExit)
            {
                ftl.Reset();
                SplashKit.ProcessEvents();
                gameWindow.Clear(Color.Black);      // reset screen 

                if (SplashKit.KeyTyped(Controls.Keylookup("Esc"))) _GameExit = true;
                if (gameWindow.CloseRequested) _GameExit = true;

                if (!Menu.GameStarted)
                {

                    Menu.DrawMenu();
                    Menu.Selection();

                    if (Menu.quit == true) _GameExit = true;  //if player selects quite close window

                    if (Menu.GameStarted)
                        Game = new Game(gameWindow, Menu.players, Menu.p1Ship, Menu.p2Ship);   // create new game instance

                }
                else
                {
                    Game.Draw();
                    Game.HandleInput();
                    Game.Updates();
                    if (!Game.GameStarted) Menu.ReSetup();      // re initialize menu
                }

                //ftl.draw();
                gameWindow.Refresh(90);         // draw frame to window
                ftl.update();

            }
            gameWindow.Close();
        }
        public class game_setup
        {
            public Window gameWindow;


            public game_setup()
            {
                //default config options
                const int screen_width = 1600;
                const int screen_height = 900;
                const bool fullscreen = false;
                const bool no_boarder = false;
                const double gameScaleDefault = 1;
                const string game_bundles = "gamebundle_full_size.txt";


                Json config_json = SplashKit.JsonFromFile("Game_Config.json");




                if (!config_json.HasKey("Asteroids_game_config"))
                {
                    Json temp_config_json = SplashKit.CreateJson();
                    temp_config_json.AddString("Asteroids_game_config", "");
                    temp_config_json.AddNumber("screen_width", screen_width);
                    temp_config_json.AddNumber("screen_height", screen_height);
                    temp_config_json.AddBool("fullscreen", fullscreen);
                    temp_config_json.AddBool("no_boarder", no_boarder);
                    temp_config_json.AddNumber("gameScale", gameScaleDefault);
                    temp_config_json.AddString("game_bundles", game_bundles);
                    config_json = temp_config_json;

                    SplashKit.JsonToFile(config_json, "Game_Config.json");
                }

                if (config_json.HasKey("screen_width") & config_json.HasKey("screen_height"))
                {
                    gameWindow = new Window("Asteroids", config_json.ReadInteger("screen_width"), config_json.ReadInteger("screen_height"));
                }
                else
                {
                    gameWindow = new Window("Asteroids", screen_width, screen_height);
                }


                if (config_json.HasKey("fullscreen"))
                {
                    if (config_json.ReadBool("fullscreen"))
                    {
                        gameWindow.ToggleFullscreen();
                    }
                }

                if (config_json.HasKey("no_boarder"))
                {
                    if (config_json.ReadBool("no_boarder"))
                    {
                        gameWindow.ToggleBorder();
                    }
                }

                if (config_json.HasKey("game_bundles"))
                {
                    SplashKit.LoadResourceBundle("MainGameBundle", config_json.ReadString("game_bundles"));
                }
                else
                {
                    SplashKit.LoadResourceBundle("MainGameBundle", game_bundles);
                }

                if (config_json.HasKey("gameScale"))
                {
                    gameScale = config_json.ReadNumber("gameScale");
                }
                else
                {
                    gameScale = gameScaleDefault;
                }

            }
        }

        public class Controls
        {
            private static Json _controls_json;
            public Controls()
            {
                _controls_json = SplashKit.JsonFromFile("Controls.json");
            }
            public static KeyCode Keylookup(string key)
            {
                if (!_controls_json.HasKey(key)) return KeyCode.UnknownKey;

                if (Enum.IsDefined(typeof(KeyCode), _controls_json.ReadString(key)))
                {
                    return Enum.Parse<KeyCode>(_controls_json.ReadString(key));
                }
                return KeyCode.UnknownKey;

            }
        }
    }

}

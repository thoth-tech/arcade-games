using System;
using SplashKitSDK;

public class Program
{
    private static bool _GameStarted;
    private static int _players;
    private static string? _p1Ship;
    private static string? _p2Ship;


    public static void Main()
    {
        Window gameWindow = new Window("Asteroids", 1600, 900);
        gameWindow.ToggleBorder();
        Game Game = null;
        Menu Menu = null;

        _GameStarted = false;
        while (!SplashKit.KeyDown(KeyCode.EscapeKey))
        {
            SplashKit.ProcessEvents();

            if (!_GameStarted)
            {
                if (Menu == null) Menu = new Menu(gameWindow);
                Menu.DrawMenu();
                Menu.Selection();
                gameWindow.Refresh(60);
                if (Menu.quit == true) { gameWindow.Close(); } //if player selects quite close window
                _GameStarted = Menu.GameStarted;
                _players = Menu.players;
                _p1Ship = Menu.p1Ship;
                _p2Ship = Menu.p2Ship;
                if (_GameStarted == true) Menu = null;

            }
            else
            {
                if (Game == null) Game = new Game(gameWindow, _players, _p1Ship, _p2Ship);
                Game.Draw();
                Game.HandleInput();
                Game.Updates();
                _GameStarted = Game.GameStarted;
                if (_GameStarted == false) Game = null;
            }
        }
        gameWindow.Close();
    }
}

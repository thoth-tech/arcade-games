using System;
using SplashKitSDK;

public class Program
{
    private static bool _GameStarted;

    public static void Main()
    {
        Window gameWindow = new Window("Asteroids", 1600, 900);
        gameWindow.ToggleBorder();
        Game Game = null;
        Menu Menu = new Menu(gameWindow);

        FrameTickLog ftl = new FrameTickLog(gameWindow,"pricedown_bl",50);
        ftl.Start();

        while (!SplashKit.KeyDown(KeyCode.EscapeKey))
        {
            ftl.Reset();
            SplashKit.ProcessEvents();
            gameWindow.Clear(Color.Black);      // reset screen 
            
            if (!Menu.GameStarted)
            {
                Menu.DrawMenu();
                Menu.Selection();

                if (Menu.quit == true)  
                    System.Environment.Exit(1); //if player selects quite close window

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

            ftl.draw();
            gameWindow.Refresh(60);         // draw frame to window
            ftl.update();
            
        }
        gameWindow.Close();
    }
}

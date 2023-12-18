using System;
using SplashKitSDK;

namespace Sky_Surge
{
    public class Program
    {
        public static void Main()
        {
            SplashKit.OpenWindow("Sky Surge", 1600, 900);
            SplashKit.WindowToggleFullscreen("Sky Surge");
            GameStateManager gameState = new GameStateManager();
            while (!SplashKit.QuitRequested() && gameState.currentState != GameStates.Exit)
            {
                gameState.Update();
                SplashKit.ProcessEvents();
            }
            SplashKit.CloseWindow("Sky Surge");
        }
    }
}

using System;
using SplashKitSDK;

namespace _2dRacerDemo
{
    public class Program
    {
        public static void Main()
        {
            Window gameWindow = new Window("2D Racing", 1600, 900);
            DrawDemo DrawDemo = new DrawDemo(gameWindow);
            bool GameExit = false;

            while (!GameExit)
            {
                gameWindow.Clear(Color.Black);
                SplashKit.ProcessEvents();
                SplashKit.DrawAllSprites();
                SplashKit.UpdateAllSprites();
                if (SplashKit.KeyTyped(KeyCode.EscapeKey)) GameExit = true;
                if (gameWindow.CloseRequested) GameExit = true;
                // reset screen





                gameWindow.Refresh(60);         // draw frame to window
            }
            gameWindow.Close(); // close window when game loop ends
        }

    }

    public class DrawDemo
    {
        private Window _GameWindow;
        private Sprite _leftEdge, _rightEdge;
        public DrawDemo(Window gameWindow)
        {
            _GameWindow = gameWindow;
            Bitmap roadEdge = SplashKit.CreateBitmap("roadEdge", 5, _GameWindow.Height);
            roadEdge.DrawLine(Color.White, 0, 0, 0, _GameWindow.Height, SplashKit.OptionLineWidth(5));
            roadEdge.SetupCollisionMask();
            _leftEdge = new Sprite("leftEdge", roadEdge);
            _leftEdge.MoveTo(200, 0);   // move to left side of screen

            _rightEdge = new Sprite("rightEdge", roadEdge);
            _rightEdge.MoveTo(_GameWindow.Width - 205, 0);

            Bitmap road = SplashKit.CreateBitmap("roadmarkings", 5, _GameWindow.Height);





        }

    }
}



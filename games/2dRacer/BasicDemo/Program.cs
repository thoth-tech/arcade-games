using System;
using SplashKitSDK;

namespace _2dRacerDemo
{
    public class Program
    {
        public static void Main()
        {
            Window gameWindow = new Window("2D Racing", 800, 600);
            DrawDemo DrawDemo = new DrawDemo(gameWindow);
            bool GameExit = false;

            while (!GameExit)
            {
                gameWindow.Clear(Color.Black);
                SplashKit.ProcessEvents(); //process splashkit events this is needed for splashkit to read things like key presses
                SplashKit.UpdateAllSprites(); // This has Splashkit update all the sprites in the current sprite pack, this will trigger animations, move position etc. 
                SplashKit.DrawAllSprites(); //This has Splashkit draw all the sprites in the current sprite pack to the screen

                if (SplashKit.KeyTyped(KeyCode.EscapeKey)) GameExit = true; //exits the main game loop if esc is pressed.
                if (gameWindow.CloseRequested) GameExit = true;
                // reset screen

                DrawDemo.HandleInput();



                gameWindow.Refresh(60);         // draw frame to window
            }
            gameWindow.Close(); // close window when game loop ends
        }

    }

    public class DrawDemo
    {
        private Window _GameWindow;
        private Sprite _leftEdge, _rightEdge, _greenCar;
        private Player _players;
        public DrawDemo(Window gameWindow)
        {
            _GameWindow = gameWindow;
            int laneSpacing = 100;
            int leftEdgePosition = _GameWindow.Width / 2 - laneSpacing * 5 / 2;


            Bitmap roadEdge = SplashKit.CreateBitmap("roadEdge", 10, _GameWindow.Height);
            roadEdge.DrawLine(Color.White, 0, 0, 0, _GameWindow.Height, SplashKit.OptionLineWidth(10));
            roadEdge.SetupCollisionMask();
            _leftEdge = new Sprite("leftEdge", roadEdge);
            _leftEdge.MoveTo(leftEdgePosition, 0);   // move to left side of screen

            _rightEdge = new Sprite("rightEdge", roadEdge);
            _rightEdge.MoveTo(leftEdgePosition + (laneSpacing * 5) - 5, 0);

            Bitmap road = SplashKit.CreateBitmap("roadmarkings", 10, _GameWindow.Height);
            for (int i = 0; i < _GameWindow.Height; i += 20)
            {
                road.DrawLine(Color.White, 1, i, 1, i + 10, SplashKit.OptionLineWidth(8));
            }

            Bitmap roadCells = SplashKit.CreateBitmap("roadCells", 50, _GameWindow.Height);
            for (int i = 0; i < 5; i++)
            {
                SplashKit.DrawBitmapOnBitmap(roadCells, road, i * 10, i * 4);
            }

            roadCells.SetCellDetails(10, _GameWindow.Height, 5, 1, 5);
            roadCells.SetupCollisionMask();
            AnimationScript roadLineAnimation = SplashKit.LoadAnimationScript("roadLineAnimation", "roadLineAnimation.txt");

            Sprite[] roadMarkings = new Sprite[4];
            for (int i = 0; i < 4; i++)
            {
                roadMarkings[i] = SplashKit.CreateSprite("roadMarkings" + i, roadCells, roadLineAnimation);
                roadMarkings[i].StartAnimation("moving");
                roadMarkings[i].MoveTo(leftEdgePosition + (laneSpacing * (i + 1)), 0);
            }
_players = new Player(_GameWindow, 2);

            // Bitmap carBitmap = SplashKit.LoadBitmap("greenCar", "greenCar.png");
            // carBitmap.SetCellDetails(75, 120, 3, 1, 3);
            // AnimationScript carAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");

            // _greenCar = SplashKit.CreateSprite("greenCar", carBitmap, carAnimation);
            // _greenCar.MoveTo(_GameWindow.Width / 2, _GameWindow.Height - 200);
            // _greenCar.StartAnimation("straight");


        }

        public void HandleInput()
        {

            _players.HandleInputs(false);
            // if (SplashKit.KeyDown(KeyCode.LeftKey) & _greenCar.AnimationHasEnded) _greenCar.StartAnimation("left");
            // if (SplashKit.KeyDown(KeyCode.RightKey) & _greenCar.AnimationHasEnded) _greenCar.StartAnimation("right");
            // if (SplashKit.KeyReleased(KeyCode.RightKey) || SplashKit.KeyReleased(KeyCode.LeftKey)) _greenCar.StartAnimation("straight");
        }
    }

}

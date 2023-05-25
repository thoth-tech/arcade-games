using System;
using SplashKitSDK;

namespace _2dRacerDemo
{
    public class Program
    {
        // global variables for testing
        public static RoadMarking test;
        public static RoadMarkingSpawner test2;
        public static Sprite s1, s2;
        public static PlayerKCB p;

        public static void Main()
        {
            Window gameWindow = new Window("2D Racing", 1600, 900);
            DrawDemo DrawDemo = new DrawDemo(gameWindow);
            bool GameExit = false;

            // testing setup function call
            Testing();

            while (!GameExit)
            {
                gameWindow.Clear(Color.Black);
<<<<<<<< HEAD:games/2dRacer/AdvancedDemo/Program.cs
                SplashKit.ProcessEvents();
                SplashKit.DrawAllSprites();
                
                SplashKit.UpdateAllSprites();
                
                if (SplashKit.KeyTyped(KeyCode.EscapeKey)) GameExit = true;
========
                SplashKit.ProcessEvents(); //process splashkit events this is needed for splashkit to read things like key presses
                SplashKit.UpdateAllSprites(); // This has Splashkit update all the sprites in the current sprite pack, this will trigger animations, move position etc. 
                SplashKit.DrawAllSprites(); //This has Splashkit draw all the sprites in the current sprite pack to the screen

                if (SplashKit.KeyTyped(KeyCode.EscapeKey)) GameExit = true; //exits the main game loop if esc is pressed.
>>>>>>>> Orginal2DRacerDemo:games/2dRacer/BasicDemo/Program.cs
                if (gameWindow.CloseRequested) GameExit = true;
                // reset screen

                DrawDemo.HandleInput();


                // testing lines for collision, remove later
                if (s1.SpriteCollision(s2))
                {
                    //Console.Out.WriteLine("T");
                    SpritePhysics.CollisionRect(s1,s2,0.2f);
                }
                
                SplashKit.DrawRectangle(Color.White,s1.CollisionRectangle);
                SplashKit.DrawRectangle(Color.White,s2.CollisionRectangle);
                
                
                test.update();  // update call for the objects
                test2.update();

                gameWindow.Refresh(60);         // draw frame to window

                //SplashKit.Delay(100);     // refresh rate parameter does not work, only 60hz will be shown
            }
            gameWindow.Close(); // close window when game loop ends
        }

        public static void TestLoop()
        {

        }

        // contained function for setting up testing
        public static void Testing()
        {
            // static marking
            test = new RoadMarking(Json.FromFile("Road_Mk_Dotted.json"));
            test.setX(40);
            test.setSpeed(7.3f);

            // marking spawner
            test2 = new RoadMarkingSpawner(Json.FromFile("Road_Mk_Spawner.json"));
            test2.setX(80);
            test2.setSpeed(5.23f);
            test2.changeX(700,500);

            // sprite collision test objects
            s1 = SplashKit.CreateSprite("greenCar", "carAnimation");
            s2 = SplashKit.CreateSprite("greenCar", "carAnimation");
            s1.CollisionKind = CollisionTestKind.AabbCollisions;
            s2.CollisionKind = CollisionTestKind.AabbCollisions;
            s1.StartAnimation("straight");
            s2.StartAnimation("straight");

            s1.Mass = 10;    // change mass to affect the collision calculation, do not use 0
            s2.Mass = 1;

            s1.X = 100;
            s1.Y = 100;

            s2.X = 120;
            s2.Y = 300;
            
            s1.Dx = 0.5f;
            s1.Dy = 0.3f;

            s2.Dx = 0.2f;
            s2.Dy = -1f;

            Json controls = Json.FromFile("Controls.json");     // load json controls, holds two objects, an array containing the string values of keys, and control schemes as json array
            List<Json> control_Schemes = new List<Json>();
            Json controlsP1;

            controls.ReadArray("Controls_Schemes", ref control_Schemes);    // get control schemes
            controlsP1 = control_Schemes[0];                        // get player 1 controls, first index
            p = new PlayerKCB(Json.FromFile("Player_Car_Green.json"),controlsP1);  // pass object json and control scheme json
            p.sprite.X = 300;
            p.sprite.Y = 700;
            p.startCallback();
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


            // Bitmap carBitmap = SplashKit.LoadBitmap("greenCar", "greenCar.png");
            // carBitmap.SetCellDetails(75, 120, 3, 1, 3);
            // AnimationScript carAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");

            // _greenCar = SplashKit.CreateSprite("greenCar", carBitmap, carAnimation);
            // _greenCar.MoveTo(_GameWindow.Width / 2, _GameWindow.Height - 200);
            // _greenCar.StartAnimation("straight");


        }

        public void HandleInput()
        {
<<<<<<<< HEAD:games/2dRacer/AdvancedDemo/Program.cs
            if (SplashKit.KeyDown(KeyCode.LeftKey) & _greenCar.AnimationHasEnded) _greenCar.StartAnimation("left");
            if (SplashKit.KeyDown(KeyCode.RightKey) & _greenCar.AnimationHasEnded) _greenCar.StartAnimation("right");
            if (SplashKit.KeyReleased(KeyCode.RightKey) || SplashKit.KeyReleased(KeyCode.LeftKey)) _greenCar.StartAnimation("straight");

========

            _players.HandleInputs(false);
            // if (SplashKit.KeyDown(KeyCode.LeftKey) & _greenCar.AnimationHasEnded) _greenCar.StartAnimation("left");
            // if (SplashKit.KeyDown(KeyCode.RightKey) & _greenCar.AnimationHasEnded) _greenCar.StartAnimation("right");
            // if (SplashKit.KeyReleased(KeyCode.RightKey) || SplashKit.KeyReleased(KeyCode.LeftKey)) _greenCar.StartAnimation("straight");
>>>>>>>> Orginal2DRacerDemo:games/2dRacer/BasicDemo/Program.cs
        }



        


    }

}

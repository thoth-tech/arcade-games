using System;
using SplashKitSDK;


// This class will be created with functions to be used in the game, as of creating this class none of these functions have been implemented into program.cs 


public class Player : Program
{

    public void SpawnSolo() //spawns a single car in the middle of the screen
    {

        private sprite _greenCarSolo;

        Bitmap carBitmap = SplashKit.LoadBitmap("greenCar", "greenCar.png");
        carBitmap.SetCellDetails(75, 120, 3, 1, 3);
        AnimationScript carAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");

        _greenCarSolo = SplashKit.CreateSprite("greenCar", carBitmap, carAnimation);
        _greenCarSolo.MoveTo(_GameWindow.Width / 2, _GameWindow.Height - 200);
        _greenCarSolo.StartAnimation("straight");


    }

    public void SpawnDuo() // spawns 2 cars, each a lane away from the middle of the screen
    {

        private sprite _greenCar1, _greenCar2

        Bitmap carBitmap = SplashKit.LoadBitmap("greenCar", "greenCar.png");
        carBitmap.SetCellDetails(75, 120, 3, 1, 3);
        AnimationScript carAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");

        _greenCar1 = SplashKit.CreateSprite("greenCar", carBitmap, carAnimation);
        _greenCar1.MoveTo((_GameWindow.Width / 2) - 100, _GameWindow.Height - 200);
        _greenCar1.StartAnimation("straight");

        _greenCar2 = SplashKit.CreateSprite("greenCar", carBitmap, carAnimation);
        _greenCar2.MoveTo((_GameWindow.Width / 2) + 100, _GameWindow.Height - 200);
        _greenCar2.StartAnimation("straight");

    }

    public void HandleInputs(bool IsSolo)    //bool IsSolo would have to be a True/False value created from when the game mode selection is done.
    {

        if (IsSolo == true)
        {

            int Speed = 2; //speed can be changed to configure the games difficulty during testing
            
            if (SplashKit.KeyDown(KeyCode.LeftKey) & _greenCarSolo.AnimationHasEnded) 
            {
                _greenCarSolo.StartAnimation("left");
                _greenCarSolo.Dx = -Speed;
            }
            if (SplashKit.KeyDown(KeyCode.RightKey) & _greenCarSolo.AnimationHasEnded) 
            {
                _greenCarSolo.StartAnimation("right");
                _greenCarSolo.Dx = Speed;
            }
            if (SplashKit.KeyReleased(KeyCode.RightKey) || SplashKit.KeyReleased(KeyCode.LeftKey)) 
            {
                _greenCarSolo.StartAnimation("straight");
            }
            if (SplashKit.KeyDown(KeyCode.UpKey) & _greenCarSolo.AnimationHasEnded) 
            {
                _greenCarSolo.Dy = -Speed;
            }
            if (SplashKit.KeyReleased(KeyCode.UpKey))
            {
                _greenCarSolo.Dy = Speed;
            }

        }

        if (IsSolo == false)
        {
            
            int Speed = 2; //speed can be changed to configure the games difficulty during testing
            
            if (SplashKit.KeyDown(KeyCode.AKey) & _greenCar1.AnimationHasEnded) 
            {
                _greenCar1.StartAnimation("left");
                _greenCar1.Dx = -Speed;
            }
            if (SplashKit.KeyDown(KeyCode.DKey) & _greenCar1.AnimationHasEnded) 
            {
                _greenCar1.StartAnimation("right");
                _greenCar1.Dx = Speed;
            }
            if (SplashKit.KeyReleased(KeyCode.DKey) || SplashKit.KeyReleased(KeyCode.AKey)) 
            {
                _greenCar1.StartAnimation("straight");
            }
            if (SplashKit.KeyDown(KeyCode.WKey) & _greenCar1.AnimationHasEnded) 
            {
                _greenCar1.Dy = -Speed;
            }
            if (SplashKit.KeyReleased(KeyCode.WKey) & _greenCar1.AnimationHasEnded)
            {
                _greenCar1.Dy = Speed;
            }
            
            
            
            if (SplashKit.KeyDown(KeyCode.LeftKey) & _greenCar2.AnimationHasEnded) 
            {
                _greenCar2.StartAnimation("left");
                _greenCar2.Dx = -Speed;
            }
            if (SplashKit.KeyDown(KeyCode.RightKey) & _greenCar2.AnimationHasEnded) 
            {
                _greenCar2.StartAnimation("right");
                _greenCar2.Dx = Speed;
            }
            if (SplashKit.KeyReleased(KeyCode.RightKey) || SplashKit.KeyReleased(KeyCode.LeftKey)) 
            {
                _greenCar2.StartAnimation("straight");
            }
            if (SplashKit.KeyDown(KeyCode.UpKey) & _greenCar2.AnimationHasEnded) 
            {
                _greenCar2.Dy = -Speed;
            }
            if (SplashKit.KeyReleased(KeyCode.UpKey) & _greenCar2.AnimationHasEnded)
            {
                _greenCar2.Dy = Speed;
            }


        }
        
        

    }


}

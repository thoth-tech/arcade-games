using System;
using SplashKitSDK;


public class Menu 
{
    private Window GameWindow;
    private String [] options = { "1 player", "2 Players","Quit" };
    private Font _GameFont = new Font("pricedown_bl", "Resources/fonts/pricedown_bl.otf");
    private const int FontSize = 120;
    private Sprite GreenCar, RedCar, PurpleCar;
    private Bitmap CarBitmapGreen, CarBitmapRed, CarBitmapPurple;
    private AnimationScript CarAnimation;
    private int index = 0;



    public Menu(Window gameWindow)
    {
        GameWindow = gameWindow;
        InIt();
    }

    private void InIt(){
        CarBitmapGreen = SplashKit.LoadBitmap("greenCar", "greenCar.png");
        CarBitmapRed = SplashKit.LoadBitmap("redCar", "redCar.png");
        CarBitmapPurple = SplashKit.LoadBitmap("purpleCar", "purpleCar.png");
        CarBitmapGreen.SetCellDetails(75, 125, 3, 1, 3);
        CarBitmapRed.SetCellDetails(75, 125, 3, 1, 3);
        CarBitmapPurple.SetCellDetails(75, 125, 3, 1, 3);
        CarAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");
        GreenCar = SplashKit.CreateSprite("greenCar", CarBitmapGreen, CarAnimation);
        RedCar = SplashKit.CreateSprite("redCar", CarBitmapRed, CarAnimation);
        PurpleCar = SplashKit.CreateSprite("purpleCar", CarBitmapPurple, CarAnimation);
    }

    public int getselection(int count){
        bool GameExit = false;
        bool started = false;
        SplashKit.LoadSoundEffect("bells", "Resources/sound/PointOfClash.ogg");
        SoundEffect sndEffect = SplashKit.SoundEffectNamed("bells");
        sndEffect.Play();

        while(! GameExit){
            GameWindow.Clear(Color.Black);
            SplashKit.ProcessEvents();
            menu(count);
            animateMenu();
            SplashKit.DrawSprite(GreenCar);
            SplashKit.DrawSprite(RedCar);
            SplashKit.DrawSprite(PurpleCar);
                // reset screen
            if(SplashKit.KeyTyped(KeyCode.UpKey) && ! started){if(count > 0){count--;}else{count =2;}menu(count);}                
            if(SplashKit.KeyTyped(KeyCode.DownKey) && ! started){if(count == 2){count=0;}else{count++;}menu(count);}
            if(SplashKit.KeyTyped(KeyCode.ReturnKey) && ! started){if(! started){GameExit =true; started = true;}}
            GameWindow.Refresh(60); 
        }
        sndEffect.Stop();
        SplashKit.FreeAllSprites();

        return count;
    }

    private void menu(int count){
            drawTitle(GameWindow,_GameFont);
            if(count == 0){
                SplashKit.DrawTextOnWindow(GameWindow, options[0], Color.Green, _GameFont, FontSize, 50,200);
                SplashKit.DrawTextOnWindow(GameWindow, options[1], Color.White, _GameFont, FontSize, 50,350);
                SplashKit.DrawTextOnWindow(GameWindow, options[2], Color.White, _GameFont, FontSize, 50,500); 
            }if(count == 1){
                SplashKit.DrawTextOnWindow(GameWindow, options[0], Color.White, _GameFont, FontSize, 50,200);
                SplashKit.DrawTextOnWindow(GameWindow, options[1], Color.Green, _GameFont, FontSize, 50,350);
                SplashKit.DrawTextOnWindow(GameWindow, options[2], Color.White, _GameFont, FontSize, 50,500);                
            }if(count == 2){
                SplashKit.DrawTextOnWindow(GameWindow, options[0], Color.White, _GameFont, FontSize, 50,200);
                SplashKit.DrawTextOnWindow(GameWindow, options[1], Color.White, _GameFont, FontSize, 50,350);
                SplashKit.DrawTextOnWindow(GameWindow, options[2], Color.Green, _GameFont, FontSize, 50,500);  
            }
    }

    private void drawTitle(Window GameWindow,Font _GameFont){
        const int FontSize = 200;
        SplashKit.DrawTextOnWindow(GameWindow, "Racing Game", Color.Red, _GameFont, FontSize, 250,0);
    }

    private void animateMenu(){
        index +=10;
        if(index == 1400){index = 0;}
        GreenCar.MoveTo(GameWindow.Width / 2, GameWindow.Height - index);
        GreenCar.StartAnimation("straight");
        RedCar.MoveTo(GameWindow.Width / 2, GameWindow.Height - index-150);
        RedCar.StartAnimation("straight");
        PurpleCar.MoveTo(GameWindow.Width / 2, GameWindow.Height - index+150);
        PurpleCar.StartAnimation("straight");
    }
}


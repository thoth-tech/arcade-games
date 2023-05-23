using System;
using System.Diagnostics;
using System.Threading;
using SplashKitSDK;
using System.Collections.Generic;

public class Game{

    private Window GameWindow;
    private Sprite _leftEdge, _rightEdge, greenCar1, greenCar2, greenCar3, redCar2, purpleCar2,redCar,purpleCar;
    private Bitmap carBitmapGreen1, carBitmapRed2, carBitmapPurple2,carBitmapRed, carBitmapPurple, carBitmapGreen2,carBitmapGreen3;
    private AnimationScript carAnimation;
    private int players = 0;
    private Font _GameFont = new Font("pricedown_bl", "Resources/fonts/pricedown_bl.otf");
    private const int FontSize = 200;

    public Game(){}

    public Game(Window gameWindow, int players){
        GameWindow = gameWindow;
        this.players = players;
        drawRoad();
    }

    private void InIt(){
        carBitmapRed = SplashKit.LoadBitmap("redCar", "redCar.png");
        carBitmapPurple = SplashKit.LoadBitmap("purpleCar", "purpleCar.png");
        carBitmapRed2 = SplashKit.LoadBitmap("redCar", "redCar.png");
        carBitmapPurple2 = SplashKit.LoadBitmap("purpleCar", "purpleCar.png");
        carBitmapGreen2 = SplashKit.LoadBitmap("greenCar", "greenCar.png");
        carBitmapGreen3 = SplashKit.LoadBitmap("greenCar", "greenCar.png");
        carBitmapRed.SetCellDetails(75, 125, 3, 1, 3);
        carBitmapPurple.SetCellDetails(75, 125, 3, 1, 3);
        carBitmapRed2.SetCellDetails(75, 125, 3, 1, 3);
        carBitmapPurple2.SetCellDetails(75, 125, 3, 1, 3);
        carBitmapGreen2.SetCellDetails(75, 125, 3, 1, 3);
        carBitmapGreen3.SetCellDetails(75, 125, 3, 1, 3);
        carAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");
        redCar = SplashKit.CreateSprite("redCar", carBitmapRed, carAnimation);
        purpleCar = SplashKit.CreateSprite("purpleCar", carBitmapPurple, carAnimation);
        redCar2 = SplashKit.CreateSprite("redCar", carBitmapRed2, carAnimation);
        purpleCar2 = SplashKit.CreateSprite("purpleCar", carBitmapPurple2, carAnimation);
        greenCar2 = SplashKit.CreateSprite("greenCar", carBitmapGreen2, carAnimation);
        greenCar3 = SplashKit.CreateSprite("greenCar", carBitmapGreen3, carAnimation);
    }

    public void startGame(){
        bool GameExit = false;
        bool countDown = false;
        bool posGreen = false, posRed= false, posPurple = false, raceCarSound = false;
        int index = 900, idx = 900, dx = 900, ex= 900,ix = 900, xx = 900,i = 0, j = 0, k = 0, l = 0, y=0, z=0;
        SoundEffect sndEffect = null;

        while(! GameExit){
            GameWindow.Clear(Color.Black);
            SplashKit.ProcessEvents();

            switch(players){
                case 0:
                SplashKit.DrawAllSprites();
                if(! posGreen){posGreen = postionStart(posGreen);}
                if(! countDown){countDown = countDownRace();InIt();}
                i = spawnRedCars(index,i);
                j = spawnPurpleCars(idx,j);
                k = spawn2ndRedCars(dx,k);
                l = spawn2ndPurpleCars(ex,l);
                y = spawnGeenCars(ix,y);
                z = spawn2ndGeenCars(xx,z);
                if(index == 0){index = 900;i = spawnRedCars(index,i);}
                if(idx == 0){idx = 900;j = spawnPurpleCars(idx,j);}
                if(dx == 0){dx = 900;k = spawn2ndRedCars(dx,k);}
                if(ex == 0){ex = 900;l = spawn2ndRedCars(dx,l);}
                if(ix == 0){ix = 900;y = spawn2ndRedCars(ix,y);}
                if(xx == 0){xx = 900;y = spawn2ndRedCars(xx,z);}
                if(sndEffect == null){
                    SplashKit.LoadSoundEffect("engineSpeed", "Resources/sound/engine-speed.ogg");
                    sndEffect = SplashKit.SoundEffectNamed("engineSpeed");
                    sndEffect.Play();
                }
                if(! SplashKit.SoundEffectPlaying("engineSpeed")){sndEffect.Play();}
                controlCars();
                index -=5;idx -=5;dx -=5;ex -=5;ix -=5;xx -=5;
                SplashKit.UpdateAllSprites();
                if (SplashKit.KeyTyped(KeyCode.EscapeKey)){GameExit = true; SplashKit.FreeAllSprites();}
                if (GameWindow.CloseRequested){GameExit = true; SplashKit.FreeAllSprites();}
                if(SplashKit.SpriteCollision(redCar, greenCar1)){GameExit = true; SplashKit.FreeAllSprites();}
                if(SplashKit.SpriteCollision(purpleCar, greenCar1)){GameExit = true; SplashKit.FreeAllSprites();}
                if(SplashKit.SpriteCollision(redCar2, greenCar1)){GameExit = true; SplashKit.FreeAllSprites();}
                if(SplashKit.SpriteCollision(purpleCar2, greenCar1)){GameExit = true; SplashKit.FreeAllSprites();}
                if(SplashKit.SpriteCollision(greenCar2, greenCar1)){GameExit = true; SplashKit.FreeAllSprites();}
                if(SplashKit.SpriteCollision(greenCar3, greenCar1)){GameExit = true; SplashKit.FreeAllSprites();}
                break;
            }
            GameWindow.Refresh(60); 
        }
    }

    private bool postionStart(bool car){
        carBitmapGreen1 = SplashKit.LoadBitmap("greenCar", "greenCar.png");
        carBitmapGreen1.SetCellDetails(75, 125, 3, 1, 3);
        carAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");
        greenCar1 = SplashKit.CreateSprite("greenCar", carBitmapGreen1, carAnimation);
        int position = 0;
        SplashKit.LoadSoundEffect("acceleration", "Resources/sound/acceleration.ogg");
        SoundEffect sndEffect = SplashKit.SoundEffectNamed("acceleration");
        sndEffect.Play();
        while(!car){
            if(position < 300){
                position +=10;
                greenCar1.MoveTo(GameWindow.Width / 2-30, GameWindow.Height - position);
                greenCar1.StartAnimation("straight");
            }else{
                car = true;
                sndEffect.Stop();
                SplashKit.LoadSoundEffect("idle", "Resources/sound/engine-idle.ogg");
                SoundEffect sdEffect = SplashKit.SoundEffectNamed("idle");
                sdEffect.Play();
            }
            SplashKit.DrawSprite(greenCar1);
            GameWindow.Refresh(60);
        }
        return car;
    }

    private bool countDownRace(){
        SplashKit.LoadSoundEffect("321", "Resources/sound/321.ogg");
        SoundEffect sndEffect = SplashKit.SoundEffectNamed("321");
        sndEffect.Play();
        int countDown = 3;
        int FontSize = 400;

        while(countDown > -1){
            if(countDown == 3){
                GameWindow.Clear(Color.Black);
                SplashKit.DrawAllSprites();
                SplashKit.DrawTextOnWindow(GameWindow, "3", Color.Red, _GameFont, FontSize, GameWindow.Width/2-100,0);
            }
            if(countDown == 2){
                GameWindow.Clear(Color.Black);
                SplashKit.DrawAllSprites();
                SplashKit.DrawTextOnWindow(GameWindow, "2", Color.Red, _GameFont, FontSize, GameWindow.Width/2-100,0);
            }
            if(countDown == 1){
                GameWindow.Clear(Color.Black);
                SplashKit.DrawAllSprites();
                SplashKit.DrawTextOnWindow(GameWindow, "1", Color.Red, _GameFont, FontSize, GameWindow.Width/2 -100,0);
            }
            if(countDown == 0){
                GameWindow.Clear(Color.Black);
                SplashKit.DrawAllSprites();
                SplashKit.DrawTextOnWindow(GameWindow, "Go", Color.Green, _GameFont, FontSize, GameWindow.Width/2 -100,0);
            }
            countDown--;
            GameWindow.Refresh(20); 
            Thread.Sleep(1000);
            
        }
        return true;
    }
//function below belongs to Phil Williams.
    private void drawRoad(){
            int laneSpacing = 100;
            int leftEdgePosition = GameWindow.Width / 2 - laneSpacing * 5 / 2;
            Bitmap roadEdge = SplashKit.CreateBitmap("roadEdge", 10, GameWindow.Height);
            roadEdge.DrawLine(Color.White, 0, 0, 0, GameWindow.Height, SplashKit.OptionLineWidth(10));
            roadEdge.SetupCollisionMask();
            _leftEdge = new Sprite("leftEdge", roadEdge);
            _leftEdge.MoveTo(leftEdgePosition, 0);   // move to left side of screen
            _rightEdge = new Sprite("rightEdge", roadEdge);
            _rightEdge.MoveTo(leftEdgePosition + (laneSpacing * 5) - 5, 0);
            Bitmap road = SplashKit.CreateBitmap("roadmarkings", 10, GameWindow.Height);
            for (int i = 0; i < GameWindow.Height; i += 20){
                road.DrawLine(Color.White, 1, i, 1, i + 10, SplashKit.OptionLineWidth(8));
            }
            Bitmap roadCells = SplashKit.CreateBitmap("roadCells", 50, GameWindow.Height);
            for (int i = 0; i < 10; i++){
                SplashKit.DrawBitmapOnBitmap(roadCells, road, i * 10, i * 8);
            }
            roadCells.SetCellDetails(10, GameWindow.Height, 5, 1, 5);
            roadCells.SetupCollisionMask();
            AnimationScript roadLineAnimation = SplashKit.LoadAnimationScript("roadLineAnimation", "roadLineAnimation.txt");
            Sprite[] roadMarkings = new Sprite[4];
            for (int i = 0; i < 4; i++){
                roadMarkings[i] = SplashKit.CreateSprite("roadMarkings" + i, roadCells, roadLineAnimation);
                roadMarkings[i].StartAnimation("moving");
                roadMarkings[i].MoveTo(leftEdgePosition + (laneSpacing * (i + 1)), 0);
            }
            Bitmap carBitmap = SplashKit.LoadBitmap("greenCar", "greenCar.png");
            carBitmap.SetCellDetails(75, 120, 3, 1, 3);
            AnimationScript carAnimation = SplashKit.LoadAnimationScript("carAnimation", "carAnimation.txt");
    }

    private int spawnGeenCars(int index, int i){
        int[] locations = {30,130,230,-90,-170};
        Random random = new Random(); 
        if(index == 900){
            i = random.Next(5);
            greenCar2.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index-150);
        }else{
            greenCar2.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index-150);
        }
        greenCar2.StartAnimation("straight");  

        return i;       
    }

    private int spawn2ndGeenCars(int index, int i){
        int[] locations = {30,130,230,-90,-170};
        Random random = new Random(); 
        if(index == 900){
            i = random.Next(5);
            greenCar3.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index);
        }else{
            greenCar3.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index);
        }
        greenCar3.StartAnimation("straight");  

        return i;       
    }

    private int spawnRedCars(int index, int i){
        int[] locations = {30,130,230,-90,-170};
        Random random = new Random(); 
        if(index == 900){
            i = random.Next(5);
            redCar.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index);
        }else{
            redCar.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index);
        }
        redCar.StartAnimation("straight");  

        return i;       
    }

    private int spawn2ndRedCars(int index, int i){
        int[] locations = {30,130,230,-90,-170};
        Random random = new Random(); 
        if(index == 900){
            i = random.Next(5);
            redCar2.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index-150);
        }else{
            redCar2.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index-150);
        }
        redCar2.StartAnimation("straight");  

        return i;       
    }

    private int spawnPurpleCars(int index, int i){
        int[] locations = {30,130,230,-90,-170};
        Random random = new Random(); 
        if(index == 900){
            i = random.Next(5);
            purpleCar.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index-150);
        }else{
            purpleCar.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index-150);
        }
        purpleCar.StartAnimation("straight");  

        return i;       
    }

    private int spawn2ndPurpleCars(int index, int i){
        int[] locations = {30,130,230,-90,-170};
        Random random = new Random(); 
        if(index == 900){
            i = random.Next(5);
            purpleCar2.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index);
        }else{
            purpleCar2.MoveTo(GameWindow.Width / 2-locations[i], GameWindow.Height-index);
        }
        purpleCar2.StartAnimation("straight");  

        return i;       
    }

    private void controlCars(){

        if (SplashKit.KeyDown(KeyCode.LeftKey) & greenCar1.AnimationHasEnded){
            greenCar1.StartAnimation("left");
        }
        if (SplashKit.KeyDown(KeyCode.RightKey) & greenCar1.AnimationHasEnded){
            greenCar1.StartAnimation("right");
        }
        if (SplashKit.KeyReleased(KeyCode.RightKey) || SplashKit.KeyReleased(KeyCode.LeftKey)){
            greenCar1.StartAnimation("straight");
        }
    }
}


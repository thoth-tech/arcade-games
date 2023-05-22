using System;
using SplashKitSDK;
using System.Collections.Generic;
using static AsteroidsGame.Program;
public abstract class Score
{

    protected SplashKitSDK.Timer _ScoreTimer;
    //protected List<Bitmap> _LivesBitmap = new List<Bitmap>();
    protected Bitmap _LivesBitmap;
    protected Window _GameWindow;
    public int Lives { get; private set; }
    public bool IsDead { get; private set; }
    protected int _PScore;

    public string Name { get; protected set; }
    public int CurrentScore
    {
        get { return _PScore; }
    }


    public Score(Window gameWindow, string player)
    {
        Lives = 5;
        /*         for (int i = 0; i < Lives; i++)
                {
                    _LivesBitmap.Add(new Bitmap($"Lives {i}", "images/heart.png"));          
                } */
        _ScoreTimer = new SplashKitSDK.Timer($"{player} Timer");
        if (SplashKit.HasBitmap("Lives"))
        {
            _LivesBitmap = SplashKit.BitmapNamed("Lives");
        }
        else
        {
            _LivesBitmap = SplashKit.LoadBitmap("Lives", "heart.png");
        }

        _GameWindow = gameWindow;
        _ScoreTimer.Start();
        IsDead = false;
        Name = player;
    }

    public void StartTimer()
    {
        _ScoreTimer.Start();
    }
    public void PauseTimer()
    {
        _ScoreTimer.Pause();
    }

    public bool isTimerPaused()
    {
        return _ScoreTimer.IsPaused;
    }

    public void ResumeTimer()
    {
        _ScoreTimer.Resume();
    }

    public abstract void Draw();

    public bool DownLife()
    {
        //Returns true once no live are left
        Lives -= 1;
        if (Lives == 0)
        {
            _ScoreTimer.Pause();
            IsDead = true;
            return true;

        }
        return false;
    }

    public void ScoreUp(int Amount)
    {
        _PScore += Amount;

    }


}

public class Player1Score : Score
{
    public Player1Score(Window gameWindow, string Player) : base(gameWindow, Player) { }
    public override void Draw()
    {
        int LivesOffset = (int)(40 * gameScale);
        int ScoreOffsetX = (int)(5 * gameScale);
        int ScoreOffsetY = (int)(40 * gameScale);
        int Scoretmp = Convert.ToInt32(_ScoreTimer.Ticks / 1000);
        int FontSize = (int)(40 * gameScale);
        Scoretmp += _PScore;

        if (IsDead)
        {
            SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, "pricedown_bl.otf", FontSize, 0, 0);

        }
        else
        {
            for (int i = 0; i < Lives; i++)
            {
                _LivesBitmap.Draw(LivesOffset * (i), 0);
            }
        }

        SplashKit.DrawTextOnWindow(_GameWindow, Scoretmp.ToString("D6"), Color.White, "pricedown_bl.otf", FontSize, ScoreOffsetX, 0 + ScoreOffsetY);


    }

}

public class Player2Score : Score
{
    public Player2Score(Window gameWindow, string Player) : base(gameWindow, Player)
    {

    }
    public override void Draw()
    {
        int LivesOffset = (int)(40 * gameScale);
        int ScoreOffsetX = (int)(130 * gameScale);
        int ScoreOffsetY = (int)(40 * gameScale);
        int Scoretmp = Convert.ToInt32(_ScoreTimer.Ticks / 1000);
        int FontSize = (int)(40 * gameScale);
        Scoretmp += _PScore;

        if (IsDead)
        {
            SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, "pricedown_bl.otf", FontSize, _GameWindow.Width - (190 * gameScale), 0);
        }
        else
        {

            for (int i = 0; i < Lives; i++)
            {
                _LivesBitmap.Draw(_GameWindow.Width - LivesOffset * (i + 1), 0);
            }

        }
        SplashKit.DrawTextOnWindow(_GameWindow, Scoretmp.ToString("D6"), Color.White, "pricedown_bl.otf", FontSize, _GameWindow.Width - ScoreOffsetX, 0 + ScoreOffsetY);
    }

}



//Credit for heart assest  'NicoleMarieProductions' https://opengameart.org/content/heart-1616
using System;
using SplashKitSDK;
using System.Collections.Generic;

public class Game
{
    private Window _GameWindow;
    private List<Player> _Players;
    private List<Player> _TempPlayers;
    private Level _gameLevel;
    private List<Score> _PlayerScores = new List<Score>();
    private int _playersNo;
    private string? _p1Ship, _p2Ship;
    public bool GameStarted { get; private set; }
    private bool _GameOverP1 = false;
    private bool _GameOverP2 = false;

    //private Players Player; temp for testing



    public Game(Window gameWindow, int playersNo, string? p1Ship, string? p2Ship)
    {
        _GameWindow = gameWindow;
        _playersNo = playersNo;
        _p1Ship = p1Ship;
        _p2Ship = p2Ship;
        GameStarted = true;
        _Players = new List<Player>();
        _TempPlayers = new List<Player>();

        _Players.Add(new Player(gameWindow, "Player 1", p1Ship, playersNo));
        _PlayerScores.Add(new Player1Score(gameWindow, "Player 1"));
        _Players[0]._PlayerScore = _PlayerScores[0];
        if (playersNo == 2)
        {
            _Players.Add(new Player(gameWindow, "Player 2", p2Ship, playersNo));
            _PlayerScores.Add(new Player2Score(gameWindow, "Player 2"));
            _Players[1]._PlayerScore = _PlayerScores[1];
        }

        SplashKit.FreeAllSprites();
        if (!SplashKit.HasSpritePack("Ships")) SplashKit.CreateSpritePack("Ships");
        if (!SplashKit.HasSpritePack("Enemies")) SplashKit.CreateSpritePack("Enemies");
        if (!SplashKit.HasSpritePack("Shots")) SplashKit.CreateSpritePack("Shots");

        //_gameLevel = new Debuglvl(_GameWindow, this);
        _gameLevel = new Level1(_GameWindow, this);
        //_gameLevel = new Level2(_GameWindow, this);


    }

    public void Nextlvl(Level lvl)
    {
        _gameLevel = lvl;
    }

    public List<Player> Players
    {
        get { return _Players; }
    }
    public void Draw()
    {
        _gameLevel.Draw();

        SplashKit.SelectSpritePack("Shots");
        SplashKit.DrawAllSprites();

        SplashKit.SelectSpritePack("Ships");
        SplashKit.DrawAllSprites();

        SplashKit.SelectSpritePack("Enemies");
        SplashKit.DrawAllSprites();

        // SplashKit.SelectSpritePack("Default");
        // SplashKit.DrawAllSprites();

        foreach (Player p in _Players)
        {
            p.Draw();
        }

        foreach (Score s in _PlayerScores)
        {
            s.Draw();
        }

    }

    public void GameOver()
    {
        Font _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        const int FontSize = 120;

        _GameWindow.Clear(Color.Black);
        foreach (Score s in _PlayerScores)
        {
            s.Draw();
        }
        int X_GameText = _GameWindow.Width / 2 - 270;
        int Y_GameText = _GameWindow.Height / 3;
        //SplashKit.SelectSpritePack("Default");
        //Console.WriteLine(SplashKit.CurrentSpritePack());
        //SplashKit.FreeAllBitmaps();
        //SplashKit.FreeAllSprites(); //There seems to be an issue with the FreeAllSprites Procedure
        //SplashKit.FreeAllAnimationScripts();

        foreach (Enemy e in _gameLevel.Enemies)
        {
            e.freesprite();

        }
        SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
        _GameWindow.Refresh(60);
        SplashKit.Delay(5000);
        GameStarted = false;
    }

    public void HandleInput()
    {
        foreach (Player p in _Players)
        { p.HandleInput(); }
        if (SplashKit.KeyTyped(KeyCode.BackspaceKey)) GameOver();
    }

    public void Updates()
    {
        for (int i = 0; i < _Players.Count(); i++)
        {
            _Players[i].Updates();
            HitCheck(_Players[i]);
            if (_Players[i].IsDead) _Players[i].Respawn(_playersNo);

            if (_Players[i]._PlayerScore.IsDead)
            {
                _Players.Remove(_Players[i]);
                i--;
            }
        }

        _gameLevel.Update();
        SplashKit.SelectSpritePack("Shots");
        SplashKit.UpdateAllSprites();

        SplashKit.SelectSpritePack("Ships");
        SplashKit.UpdateAllSprites();

        SplashKit.SelectSpritePack("Enemies");
        SplashKit.UpdateAllSprites();

        // SplashKit.SelectSpritePack("Default");
        // SplashKit.UpdateAllSprites();

        foreach (Enemy e in _gameLevel.Enemies)
        {
            if (e.CanShoot) HitCheck(e);

        }

        bool _GameOver = true;
        foreach (Score s in _PlayerScores)
        {
            _GameOver = s.IsDead ? _GameOver : false;
        }
        if (_GameOver)
        {
            GameOver();
        }

    }

    public void HitCheck(Player player)
    {
        foreach (Enemy e in _gameLevel.Enemies)
        {
            Tuple<String, int> HitCheckResult = player.HitCheck(e);

            if (HitCheckResult.Item1 == "Life") // Look at re-spwarning Player in centre
            {
                player._PlayerScore.DownLife();
                player.Killed();
            }
            else if (HitCheckResult.Item1 == "Score")
            {
                player._PlayerScore.ScoreUp(HitCheckResult.Item2);
            }
            // if (e.CanShoot)
            // {
            //     Tuple<String, int> PlayerHitCheckResult = player.HitCheck(e);
            // }
        }
    }

    public void HitCheck(Enemy enemy)
    {
        foreach (Player p in _Players)
        {
            Tuple<String, int> HitCheckResult = enemy.HitCheck(p);

            if (HitCheckResult.Item1 == "Life") // Look at re-spwarning Player in centre
            {
                foreach (Score s in _PlayerScores)
                {
                    if (s.Name == p.Name)
                    {
                        s.DownLife();
                        p.Killed();
                    }
                }
            }
        }
    }
}




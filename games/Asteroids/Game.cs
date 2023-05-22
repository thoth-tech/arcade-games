using System;
using SplashKitSDK;
using System.Collections.Generic;
using static AsteroidsGame.Program;

public class Game
{
    private Window _GameWindow;
    private List<Player> _Players;
    private Player[] _TempPlayers;
    private Level _gameLevel;
    private int _playersNo;
    public bool GameStarted { get; private set; }
    private int _GameOverCount;
    private String[] _SpritePacks = { "Shots", "Ships", "Enemies" };



    //private Players Player; temp for testing



    public Game(Window gameWindow, int playersNo, string? p1Ship, string? p2Ship)
    {
        _GameWindow = gameWindow;
        _playersNo = playersNo;
        GameStarted = true;
        _GameOverCount = -1;
        _Players = new List<Player>();
        _TempPlayers = new Player[playersNo];

        _Players.Add(new Player(gameWindow, "Player 1", p1Ship, playersNo));
        _Players[0].PlayerScore = new Player1Score(gameWindow, "Player 1");
        if (playersNo == 2)
        {
            _Players.Add(new Player(gameWindow, "Player 2", p2Ship, playersNo));
            _Players[1].PlayerScore = new Player2Score(gameWindow, "Player 2");
        }

        SplashKit.FreeAllSprites();
        if (!SplashKit.HasSpritePack("Ships")) SplashKit.CreateSpritePack("Ships");
        if (!SplashKit.HasSpritePack("Enemies")) SplashKit.CreateSpritePack("Enemies");
        if (!SplashKit.HasSpritePack("Shots")) SplashKit.CreateSpritePack("Shots");

        //_gameLevel = new Debuglvl(_GameWindow, this);
        _gameLevel = new Level1(_GameWindow, this);
        //_gameLevel = new Level2(_GameWindow, this);
        //_gameLevel = new Jsonlvl(_GameWindow, this, "Level_Test.json");
        //_gameLevel = new EndGame(_GameWindow, this);

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
            if (!p.PlayerScore.IsDead)
                p.Draw();

            p.PlayerScore.Draw();
        }

        if (_GameOverCount > -1)
        {
            DrawGameOver();
        }

    }

    public void GameOver()
    {
        Font _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        int FontSize = (int)(120 * gameScale);

        _GameWindow.Clear(Color.Black);
        foreach (Player p in _Players)
        {
            p.PlayerScore.Draw();
        }
        int X_GameText = _GameWindow.Width / 2 - (int)(270 * gameScale);
        int Y_GameText = _GameWindow.Height / 3;

        foreach (Enemy e in _gameLevel.Enemies)
        {
            e.freesprite();

        }
        SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
        _GameWindow.Refresh(60);
        SplashKit.Delay(5000);
        GameStarted = false;
    }

    public void GameEndCleanup()
    {
        GameStarted = false;
        SplashKit.FreeSpritePack("Shots");
        SplashKit.FreeSpritePack("Ships");
        SplashKit.FreeSpritePack("Enemies");
    }
    public void DrawGameOver()
    {
        Font _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        int FontSize = (int)(120 * gameScale);

        int X_GameText = _GameWindow.Width / 2 - (int)(270 * gameScale);
        int Y_GameText = _GameWindow.Height / 3;
        SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
    }

    public void HandleInput()
    {
        foreach (Player p in _Players)
        { p.HandleInput(); }

    }

    public void Updates()
    {
        bool _GameOver = true;
        for (int i = 0; i < _Players.Count(); i++)
        {
            if (!_Players[i].PlayerScore.IsDead)
            {
                _Players[i].Updates();
                HitCheck(_Players[i]);
                if (_Players[i].IsDead) _Players[i].Respawn(_playersNo);
            }
            _GameOver = _Players[i].PlayerScore.IsDead ? _GameOver : false;
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

        if (_GameOver && _GameOverCount == -1)
        {
            _GameOverCount = 0;
            //GameOver();
        }
        else if (_GameOverCount > -1)
        {
            _GameOverCount++;
            if (_GameOverCount > 120)
            {
                GameEndCleanup();
            }
        }

    }

    public void HitCheck(Player player)
    {
        foreach (Enemy e in _gameLevel.Enemies)
        {
            Tuple<String, int> HitCheckResult = player.HitCheck(e);

            if (HitCheckResult.Item1 == "Life") // Look at re-spwarning Player in centre
            {
                player.PlayerScore.DownLife();
                player.Killed();
            }
            else if (HitCheckResult.Item1 == "Score")
            {
                player.PlayerScore.ScoreUp(HitCheckResult.Item2);
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
                p.PlayerScore.DownLife();
                p.Killed();
            }
        }
    }
}




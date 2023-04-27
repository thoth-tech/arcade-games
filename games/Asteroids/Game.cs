using System;
using SplashKitSDK;
using System.Collections.Generic;

public class Game
{
    private Window _GameWindow;
    private List<Player> _Players;
    private List<Player> _TempPlayers;
    private Level _gameLevel;
    private int _playersNo;
    public bool GameStarted { get; private set; }

    private int _GameOverCount;




    public Game(Window gameWindow, int playersNo, string? p1Ship, string? p2Ship)
    {
        _GameWindow = gameWindow;
        _playersNo = playersNo;
        GameStarted = true;
        _Players = new List <Player>();
        _TempPlayers = new List <Player>();
        _GameOverCount = -1;

        _Players.Add(new Player(gameWindow,"Player 1",p1Ship,playersNo));
        _Players[0]._PlayerScore = new Player1Score(gameWindow,"Player 1");
        if (playersNo == 2)
        {
            _Players.Add(new Player(gameWindow,"Player 2",p2Ship,playersNo));
            _Players[1]._PlayerScore = new Player2Score(gameWindow,"Player 2");
        }

        SplashKit.CreateSpritePack("Game");
        SplashKit.SelectSpritePack("Game");

        //_gameLevel = new Debuglvl(_GameWindow, this);
        _gameLevel = new Jsonlvl(_GameWindow, this, "Level_Test.json");
        //_gameLevel = new Level1(_GameWindow, this);

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

        SplashKit.DrawAllSprites();

        foreach (Player p in _Players)
        {
            if (!p._PlayerScore.IsDead)
            {
                p.Draw();
            }
            p._PlayerScore.Draw();
        }

        if (_GameOverCount > -1)
        {
            DrawGameOver();
        }
    }

    public void GameOver()  // using this version for some reason retains old sprites
    {
        Font _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        const int FontSize = 120;  
        foreach(Player p in _Players)
        {
            p._PlayerScore.Draw();
        }
        int X_GameText = _GameWindow.Width / 2 - 270;
        int Y_GameText = _GameWindow.Height / 3;
        SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
        _GameWindow.Refresh(60);
        SplashKit.Delay(5000);
        SplashKit.FreeSpritePack("Game");
        GameStarted = false;
    }

    public void DrawGameOver()
    {
        Font _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        const int FontSize = 120;  

        int X_GameText = _GameWindow.Width / 2 - 270;
        int Y_GameText = _GameWindow.Height / 3;
        SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
    }

    public void HandleInput()
    {
        foreach (Player p in _Players)
        { p.HandleInput(); }
        if (SplashKit.KeyTyped(KeyCode.BackspaceKey)) GameOver();
    }

    public void Updates()
    {
        SplashKit.UpdateAllSprites();

        bool _GameOver = true;
        for (int i = 0; i < _Players.Count(); i++)
        {
            if (!_Players[i]._PlayerScore.IsDead)
            {
                _Players[i].Updates();
                HitCheck(_Players[i]);
                if (_Players[i].IsDead) _Players[i].Respawn(_playersNo);
            }
            _GameOver = _Players[i]._PlayerScore.IsDead ? _GameOver : false;
        }

        _gameLevel.Update();

        if (_GameOver && _GameOverCount == -1)
        {
            _GameOverCount = 0;
            //GameOver();
        }
        else if (_GameOverCount > -1)
        {
            _GameOverCount++;
            if  (_GameOverCount > 120)
            {
                GameStarted = false;
                SplashKit.FreeSpritePack("Game");
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
                p._PlayerScore.DownLife();
                p.Killed();
            }
        }
    }
}




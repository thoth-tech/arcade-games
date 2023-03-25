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
    public bool GameStarted{get; private set; }
    private bool _GameOverP1 = false;
    private bool _GameOverP2 = false;

    //private Players Player; temp for testing



    public Game(Window gameWindow,int playersNo,string? p1Ship,string? p2Ship)
    {
        _GameWindow = gameWindow;
        _playersNo = playersNo;
        _p1Ship = p1Ship;
        _p2Ship = p2Ship;
        GameStarted = true;
        _Players = new List <Player>();
        _TempPlayers = new List <Player>();
        
        if (playersNo == 1)
        {
            _Players.Add(new Player(gameWindow,"Player 1",p1Ship,playersNo));
            _PlayerScores.Add(new Player1Score(gameWindow,"Player 1"));
        }
        else if (playersNo == 2)
        {
            _Players.Add(new Player(gameWindow,"Player 1",p1Ship,playersNo));
            _Players.Add(new Player(gameWindow,"Player 2",p2Ship,playersNo));
            _PlayerScores.Add(new Player1Score(gameWindow,"Player 1"));
            _PlayerScores.Add(new Player2Score(gameWindow,"Player 2"));
        }

        _gameLevel = new Level1(_GameWindow,this);

    }

    public void Nextlvl(Level lvl)
    {
        _gameLevel = lvl;
    }

    public void Draw()
    {
        _GameWindow.Clear(Color.Black);
        _gameLevel.Draw();
        foreach(Player p in _Players)
        {
            p.Draw();
        }
        
        foreach(Score s in _PlayerScores)
        {
            s.Draw();
        }

        
        _GameWindow.Refresh(60);

    }

    public void GameOver()
    {
        Font _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        const int FontSize = 120;
        _GameWindow.Clear(Color.Black);       
        foreach(Score s in _PlayerScores)
        {
            s.Draw();
        }
        int X_GameText = _GameWindow.Width / 2 - 270;
        int Y_GameText = _GameWindow.Height / 3;
        SplashKit.DrawTextOnWindow(_GameWindow, "Game Over", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
        _GameWindow.Refresh(60);
        SplashKit.Delay(5000);
        GameStarted = false;
    }

    public void HandleInput()
    {
        foreach(Player p in _Players)
        {p.HandleInput();}
        if (SplashKit.KeyTyped(KeyCode.BackspaceKey)) GameOver();
    }
    public void Updates()
    {
        List<Player> KillPlayer = new List<Player>();
        foreach(Player p in _Players)
        {
            p.Updates();
            HitCheck(p);
            if(p.IsDead) KillPlayer.Add(p);
        }
        _gameLevel.Update();

        foreach(Player p in KillPlayer)
        {
            _Players.Remove(p);
            foreach (Score s in _PlayerScores)
            {
                if (!s.IsDead && s.Name == p.Name)
                {
                    if (p.Name == "Player 1")
                    {
                        _Players.Add(new Player(_GameWindow, "Player 1", _p1Ship, _playersNo));
                    }
                    else if (p.Name == "Player 2")
                    {
                        _Players.Add(new Player(_GameWindow, "Player 2", _p2Ship, _playersNo));
                    }
                }
            }
        }
        foreach (Score s in _PlayerScores)
        {
            if (s.IsDead && s.Name == "Player 1") _GameOverP1 = true;
            if (s.IsDead && s.Name == "Player 2") _GameOverP2 = true;
        }
        if (_playersNo == 1)
        {
            if (_GameOverP1) GameOver();
        }
        else
        {
            if (_GameOverP1 && _GameOverP2) GameOver();
        }
    }

    public void HitCheck(Player player)
    {
        foreach (Enemy e in _gameLevel.Enemies)
        {
            Tuple<String, int> HitCheckResult = player.HitCheck(e);

            if (HitCheckResult.Item1 == "Life") // Look at re-spwarning Player in centre
            {
                foreach (Score s in _PlayerScores)
                {
                    if (s.Name == player.Name)
                    {
                        s.DownLife();
                        player.Killed();
                    }
                }
            }
            else if (HitCheckResult.Item1 == "Score")
            {
                foreach (Score s in _PlayerScores)
                {
                    if (s.Name == player.Name)
                    {
                        s.ScoreUp(HitCheckResult.Item2);
                    }
                }
            }
        }


    }

}




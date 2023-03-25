using System;
using SplashKitSDK;
using System.Collections.Generic;

public class Player
{
    private Window _gameWindow;
    private List<Shooting> _shots;
    public double X { get; private set; }
    public double Y { get; private set; }
    private Bitmap _Ship;
    private double _Angle;
    private string _Player;
    private List<Shooting> _KillShots = new List<Shooting>();
    public bool IsDead { get; private set; }
    private Timer _InvulnerableTime;
    private bool _IsInvulnerable;

    public string Name { get { return _Player; } }

    public Player(Window gameWindow, string Player, string PlayerShip, int PlayersNo)
    {
        _gameWindow = gameWindow;
        _Ship = new Bitmap(Player, PlayerShip);
        _Angle = 0;
        _Player = Player;
        _shots = new List<Shooting>();
        IsDead = false;
        _InvulnerableTime = new Timer($"{Player} Invulnerable");
        _InvulnerableTime.Start();
        _IsInvulnerable = true;
        if (PlayersNo == 1)
        {
            Y = (_gameWindow.Height - _Ship.Height) / 2;
            X = (_gameWindow.Width - _Ship.Width) / 2;
        }
        else
        {
            int gameWindow_8th = _gameWindow.Width / 3;
            if (_Player == "Player 1")
            {
                Y = (_gameWindow.Height - _Ship.Height) / 2;
                X = (gameWindow_8th - _Ship.Width / 2);
            }
            else if (_Player == "Player 2")
            {
                Y = (_gameWindow.Height - _Ship.Height) / 2;
                X = (gameWindow_8th * 2 - _Ship.Width / 2);
            }
        }

    }

    public void Killed()
    {
        IsDead = true;
    }
    public void Draw()
    {
        foreach (Shooting s in _shots)
        {
            s.Draw();
        }
        if (_IsInvulnerable)
        {

            if ((_InvulnerableTime.Ticks / 250) % 2 == 0)
            {
                _Ship.Draw(X, Y, SplashKit.OptionRotateBmp(_Angle));
            }
            /* Point2D XY = new Point2D() {X=X,Y=Y};
            _Ship.Draw(X,Y,SplashKit.OptionRotateBmp(_Angle));
            SplashKit.DrawCircle(Color.AliceBlue,_Ship.BoundingCircle(XY)); */

        }
        else
        {
            _Ship.Draw(X, Y, SplashKit.OptionRotateBmp(_Angle));
        }


    }

    private void Rotation(double change)
    {
        _Angle = (_Angle + change) % 360;
    }

    private void Move(double Speed)
    {
        Vector2D XY_Change = new Vector2D();
        Matrix2D Rotate = SplashKit.RotationMatrix(_Angle + 270);
        XY_Change.X += Speed;
        XY_Change = SplashKit.MatrixMultiply(Rotate, XY_Change);
        X += XY_Change.X;
        Y += XY_Change.Y;
    }
    public void HandleInput()
    {
        if (_Player == "Player 1")
        { Player1Controls(); }
        else if (_Player == "Player 2")
        { Player2Controls(); }
        OutOfBounds();
    }

    private void Player1Controls()
    {
        const double Speed = 5;
        if (SplashKit.KeyDown(KeyCode.LeftKey)) Rotation(-Speed);
        if (SplashKit.KeyDown(KeyCode.RightKey)) Rotation(Speed);
        if (SplashKit.KeyDown(KeyCode.UpKey)) Move(Speed);
        if (SplashKit.KeyDown(KeyCode.DownKey)) { }
        if (SplashKit.KeyTyped(KeyCode.RightCtrlKey)) { Shoot(); }

    }

    private void Player2Controls()
    {
        const double Speed = 5;
        if (SplashKit.KeyDown(KeyCode.AKey)) Rotation(-Speed);
        if (SplashKit.KeyDown(KeyCode.DKey)) Rotation(Speed);
        if (SplashKit.KeyDown(KeyCode.WKey)) Move(Speed);
        if (SplashKit.KeyTyped(KeyCode.SpaceKey)) { Shoot(); }

    }

    private void OutOfBounds()
    {
        if (X < 0 - _Ship.Width) X = _gameWindow.Width;
        else if (X > _gameWindow.Width) X = 0 - _Ship.Width;
        else if (Y < 0 - _Ship.Height) Y = _gameWindow.Height;
        else if (Y > _gameWindow.Height) Y = 0 - _Ship.Height;

    }

    private void Shoot()
    {
        Shooting ShotType = new Shooting(_Ship.Center, _Angle, this);
        _shots.Add(ShotType);
    }

    public void Updates()
    {

        foreach (Shooting s in _shots)
        {
            s.Update();
            if (s.IsOffscreen(_gameWindow)) _KillShots.Add(s);
        }

        foreach (Shooting s in _KillShots)
        { _shots.Remove(s); }
        _KillShots.Clear();


        if (_InvulnerableTime.Ticks > 1500) //1500
        {
            _IsInvulnerable = false;
            _InvulnerableTime.Stop();
            _InvulnerableTime.Reset();
        }



    }

    public Tuple<String, int> HitCheck(Enemy enemy)
    {
        if (!_IsInvulnerable && !IsDead)
        {
            bool hit = false;
            
            foreach( Circle e in enemy.HitCircle())
            {
                if(SplashKit.BitmapCircleCollision(_Ship, X, Y, e)) hit = true;
            }

            if(hit)
            { return enemy.HitBy(this); }
        }

        foreach (Shooting s in _shots)
        {
            if (s.HitCheck(enemy))
            {
                _KillShots.Add(s);
                return enemy.HitBy(s);
            }
        }
        return new Tuple<string, int>("False", 0);

    }


}
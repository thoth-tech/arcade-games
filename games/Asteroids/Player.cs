using System;
using SplashKitSDK;
using System.Collections.Generic;
using static AsteroidsGame.Program;


public class Player
{
    private Window _gameWindow;

    public double X { get; private set; }
    public double Y { get; private set; }
    private Bitmap _Ship;
    private double _Angle;
    private string _Player;
    private List<Shooting> _shots = new List<Shooting>();
    //private List<Shooting> _KillShots = new List<Shooting>();
    public bool IsDead { get; private set; }
    private SplashKitSDK.Timer _InvulnerableTime;
    //private bool _IsInvulnerable;
    public Score PlayerScore { get; set; }
    public bool IsInvulnerable { get; private set; }

    public string Name { get { return _Player; } }

    public Player(Window gameWindow, string Player, string PlayerShip, int PlayersNo)
    {
        _gameWindow = gameWindow;
        _Ship = SplashKit.LoadBitmap(Player, PlayerShip);
        _Player = Player;

        Respawn(PlayersNo);

    }

    public void Respawn(int PlayersNo)
    {
        _Angle = 0;
        _shots = new List<Shooting>();
        IsDead = false;
        _InvulnerableTime = new SplashKitSDK.Timer($"{_Player} Invulnerable");
        _InvulnerableTime.Start();
        IsInvulnerable = true;

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
        if (IsInvulnerable)
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
        _Angle = (_Angle + change); //% 360
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
        double MoveSpeed = 5 * gameScale;
        double RotateSpeed = 5;
        if (SplashKit.KeyDown(Controls.Keylookup("P1_left"))) Rotation(-RotateSpeed);
        if (SplashKit.KeyDown(Controls.Keylookup("P1_right"))) Rotation(RotateSpeed);
        if (SplashKit.KeyDown(Controls.Keylookup("P1_up"))) Move(MoveSpeed);
        if (SplashKit.KeyTyped(Controls.Keylookup("P1_button1"))) { Shoot(); }
    }

    private void Player2Controls()
    {
        double MoveSpeed = 5 * gameScale;
        double RotateSpeed = 5;
        if (SplashKit.KeyDown(Controls.Keylookup("P2_left"))) Rotation(-RotateSpeed);
        if (SplashKit.KeyDown(Controls.Keylookup("P2_right"))) Rotation(RotateSpeed);
        if (SplashKit.KeyDown(Controls.Keylookup("P2_up"))) Move(MoveSpeed);
        if (SplashKit.KeyTyped(Controls.Keylookup("P2_button1"))) { Shoot(); }

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
        Shooting ShotType = new PlayerShot(_Ship.Center, _Angle, this);
        _shots.Add(ShotType);
    }

    public void Updates()
    {
        for (int i = 0; i < _shots.Count; i++)
        {
            _shots[i].Update();
            if (_shots[i].IsOffscreen(_gameWindow))
            {
                _shots[i].freesprite();
                _shots.RemoveAt(i--);
            }
        }

        /*
        foreach (Shooting s in _shots)
        {
            s.Update();
            if (s.IsOffscreen(_gameWindow)) _KillShots.Add(s);
        }

        foreach (Shooting s in _KillShots)      // avoiding this, to reduce comparisons for performance.
        { _shots.Remove(s); }
        _KillShots.Clear();
        */


        if (_InvulnerableTime.Ticks > 1500) //1500
        {
            IsInvulnerable = false;
            _InvulnerableTime.Stop();
            _InvulnerableTime.Reset();
        }



    }

    public Bitmap HitBMP()
    {
        return _Ship;
    }

    public Tuple<String, int> HitCheck(Enemy enemy)
    {
        if (!IsInvulnerable && !IsDead)
        {
            bool hit = false;
            
            foreach (Circle e in enemy.HitCircle())
            {
                if (SplashKit.BitmapCircleCollision(_Ship, X, Y, e)) hit = true;
            }
            if (enemy.HitSprite() != null)
            {
                if (SplashKit.SpriteBitmapCollision(enemy.HitSprite(), _Ship, X, Y)) hit = true;
            }
            if (hit) return enemy.HitBy(this);
        }


        for (int i = 0; i < _shots.Count; i++)
        {
            Shooting s = _shots[i];
            if (s.HitCheck(enemy))
            {
                _shots.RemoveAt(i--);
                return enemy.HitBy(s);  // free sprite is called inside hitby currently
            }
        }
        /*
        foreach (Shooting s in _shots)
        {
            if (s.HitCheck(enemy))
            {
                _KillShots.Add(s);
                return enemy.HitBy(s);
            }
        }
        */
        return new Tuple<string, int>("False", 0);

    }


}
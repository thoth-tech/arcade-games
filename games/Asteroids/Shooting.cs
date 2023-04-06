using System;
using SplashKitSDK;
using System.Collections.Generic;

public abstract class Shooting
{
    private double _Angle;
    protected Vector2D _Velocity;
    protected double _X, _Y;


    protected int Radius;


    // public Shooting(Point2D fromPT, double Angle,Player player)
    // {   
    //     Angle += 270;
    //     const int SPEED = 10;
    //     ShotByPlayer = player;
    //     _X = ShotByPlayer.X + fromPT.X;
    //     _Y = ShotByPlayer.Y + fromPT.Y;
    //     Vector2D direction = SplashKit.UnitVector(SplashKit.VectorFromAngle(Angle,10));
    //     _Velocity = SplashKit.VectorMultiply(direction,SPEED);

    // }

    public virtual void Draw()
    {
        SplashKit.FillCircle(Color.White, _X, _Y, Radius);
    }

    public virtual void Update()
    {
        _X += _Velocity.X;
        _Y += _Velocity.Y;
    }

    public virtual bool IsOffscreen(Window screen)
    {
        bool Offscreen = false;

        if (_X < -Radius | _X > screen.Width | _Y < -Radius | _Y > screen.Height)
        {
            Offscreen = true;
        }
        return Offscreen;
    }

    public abstract bool HitCheck(Enemy enemy);
    public abstract bool HitCheck(Player player);


}

public class PlayerShot : Shooting
{
    public Player ShotByPlayer { get; protected set; }
    public PlayerShot(Point2D fromPT, double Angle, Player player)
    {
        Angle += 270;
        const int SPEED = 10;
        Radius = 4;
        ShotByPlayer = player;
        _X = ShotByPlayer.X + fromPT.X;
        _Y = ShotByPlayer.Y + fromPT.Y;
        Vector2D direction = SplashKit.UnitVector(SplashKit.VectorFromAngle(Angle, 10));
        _Velocity = SplashKit.VectorMultiply(direction, SPEED);

    }

    public override bool HitCheck(Enemy enemy)
    {
        bool hit = false;
        Circle tmpShot = SplashKit.CircleAt(_X, _Y, Radius);
        foreach (Circle e in enemy.HitCircle())
        {
            if (SplashKit.CirclesIntersect(tmpShot, e)) hit = true;
        }

        return hit;
    }
    public override bool HitCheck(Player player) { return false; }

}
public class BossSmallShot : Shooting
{
    private Circle _shotCircle;
    public BossSmallShot(Point2D fromPT, double Angle)
    {
        const int SPEED = 5;
        _X = fromPT.X;
        _Y = fromPT.Y;
        Vector2D direction = SplashKit.UnitVector(SplashKit.VectorFromAngle(Angle, 10));
        _Velocity = SplashKit.VectorMultiply(direction, SPEED);
        Radius = 10;
        _shotCircle = SplashKit.CircleAt(_X, _Y, Radius);
    }

    public override bool HitCheck(Player player)
    {
        bool hit = false;
        if (SplashKit.BitmapCircleCollision(player.HitBMP(), player.X, player.Y, _shotCircle)) hit = true;

        return hit;
    }

    public override void Update()
    {
        _X += _Velocity.X;
        _Y += _Velocity.Y;
        _shotCircle = SplashKit.CircleAt(_X, _Y, Radius);
    }

    public override bool HitCheck(Enemy enemy) { return false; }
    public override void Draw()
    {
        SplashKit.FillCircle(Color.Red, _shotCircle);
    }
}

public class RedEnergyBall : Shooting
{

    private Bitmap _EnergyBall;
    //private AnimationScript _EnergyBallScript;
    private Sprite _EnergyBallSprite;
    public RedEnergyBall(Point2D fromPT, Player player)
    {
        const int SPEED = 5;
        _X = fromPT.X;
        _Y = fromPT.Y;
        Point2D toPT = new Point2D();
        toPT.X = player.X;
        toPT.Y = player.Y;
        Vector2D direction = SplashKit.UnitVector(SplashKit.VectorPointToPoint(fromPT, toPT));
        _Velocity = SplashKit.VectorMultiply(direction, SPEED);

        //_EnergyBall = new Bitmap("RedEnergyBall", "RedEnergyBall.png");
        _EnergyBall = new Bitmap("RedEnergyBall", "Player.png");
        //_EnergyBall.SetCellDetails(100, 100, 3, 3, 9);
        //_EnergyBallScript = SplashKit.LoadAnimationScript("RedEnergyBall", "RedEnergyBall.txt");
        //_EnergyBallSprite = SplashKit.CreateSprite(_EnergyBall, _EnergyBallScript);
        _EnergyBallSprite = SplashKit.CreateSprite(_EnergyBall);
        _EnergyBallSprite.Position = fromPT;

    }
    public override bool HitCheck(Player player)
    {
        bool hit = false;
        if (SplashKit.SpriteBitmapCollision(_EnergyBallSprite, player.HitBMP(), player.X, player.Y))
        {
            hit = true;
            SplashKit.FreeSprite(_EnergyBallSprite);
        }

        return hit;
    }
    public override bool HitCheck(Enemy enemy) { return false; }
    public override void Update()
    {
        _EnergyBallSprite.X += Convert.ToSingle(_Velocity.X);
        _EnergyBallSprite.Y += Convert.ToSingle(_Velocity.Y);
        //_EnergyBallSprite.AddToVelocity(_Velocity);

    }

    public override bool IsOffscreen(Window screen)
    {
        bool Offscreen = false;

        if (_EnergyBallSprite.Offscreen())
        {
            Offscreen = true;
            SplashKit.FreeSprite(_EnergyBallSprite);
        }
        return Offscreen;


    }

}

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

    public bool IsOffscreen(Window screen)
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
using System;
using SplashKitSDK;
using System.Collections.Generic;

public class Shooting
{
    private double _Angle;
    private Vector2D Velocity;
    private double X, Y;
    public Player ShotByPlayer {get;private set;}

    private const int Radius = 4;
    public Shooting(Point2D fromPT, double Angle,Player player)
    {   
        Angle += 270;
        const int SPEED = 10;
        ShotByPlayer = player;
        X = ShotByPlayer.X + fromPT.X;
        Y = ShotByPlayer.Y + fromPT.Y;
        Vector2D direction = SplashKit.UnitVector(SplashKit.VectorFromAngle(Angle,10));
        Velocity = SplashKit.VectorMultiply(direction,SPEED);
        
    }

    public bool HitCheck(Enemy enemy)
    {
        bool hit = false;
        Circle tmpShot = SplashKit.CircleAt(X,Y,Radius);
        foreach( Circle e in enemy.HitCircle())
        {
            if(SplashKit.CirclesIntersect(tmpShot,e)) hit = true;
        }
    

        return hit;

    }

    public void Draw()
    {
        const int Radius = 4;
        SplashKit.FillCircle(Color.White,X,Y,Radius);
    }

    public void Update()
    {
        X += Velocity.X;
        Y += Velocity.Y;
    }

    public bool IsOffscreen (Window screen)
    {
        bool Offscreen = false;
        
        if (X < -Radius | X > screen.Width | Y < -Radius | Y > screen.Height)
        {
            Offscreen = true;
        }
        return Offscreen;
    }

}
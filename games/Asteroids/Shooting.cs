using System;
using SplashKitSDK;
using System.Collections.Generic;
using static AsteroidsGame.Program;

public abstract class Shooting
{
    private double _Angle;
    protected Vector2D _Velocity;
    public double X { get; protected set; }
    public double Y { get; protected set; }


    protected int Radius;

    public Shooting()
    {
        SplashKit.SelectSpritePack("Shots");
    }

    // public Shooting(Point2D fromPT, double Angle,Player player)
    // {   
    //     Angle += 270;
    //     const int SPEED = 10;
    //     ShotByPlayer = player;
    //     X = ShotByPlayer.X + fromPT.X;
    //     Y = ShotByPlayer.Y + fromPT.Y;
    //     Vector2D direction = SplashKit.UnitVector(SplashKit.VectorFromAngle(Angle,10));
    //     _Velocity = SplashKit.VectorMultiply(direction,SPEED);

    // }


    public virtual void freesprite()
    {
        //Does nothing but allows inherited classes to free their own sprite
    }

    public virtual void Draw()
    {
        SplashKit.FillCircle(Color.White, X, Y, Radius);
    }

    public virtual void Update()
    {
        X += _Velocity.X;
        Y += _Velocity.Y;
    }

    public virtual bool IsOffscreen(Window screen)
    {
        bool Offscreen = false;

        if (X < -Radius | X > screen.Width | Y < -Radius | Y > screen.Height)
        {
            Offscreen = true;
        }
        return Offscreen;
    }
    public virtual float damage()
    {
        return 1; //default damage for a shot
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
        int SPEED = (int)(10 * gameScale);
        Radius = (int)(4 * gameScale);
        ShotByPlayer = player;
        X = ShotByPlayer.X + fromPT.X;
        Y = ShotByPlayer.Y + fromPT.Y;
        Vector2D direction = SplashKit.UnitVector(SplashKit.VectorFromAngle(Angle, 10));
        _Velocity = SplashKit.VectorMultiply(direction, SPEED);

    }

    public override bool HitCheck(Enemy enemy)
    {
        bool hit = false;
        Circle tmpShot = SplashKit.CircleAt(X, Y, Radius);
        foreach (Circle e in enemy.HitCircle())
        {
            if (SplashKit.CirclesIntersect(tmpShot, e)) hit = true;
        }
        // if (enemy.HitSprite() != null)
        // {
        //     if (SplashKit.CirclesIntersect(enemy.HitSprite().CollisionCircle(), tmpShot)&&enemy) hit = true; //change this over to sprites if everything is a sprite
        // }

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
        X = fromPT.X;
        Y = fromPT.Y;
        Vector2D direction = SplashKit.UnitVector(SplashKit.VectorFromAngle(Angle, 10));
        _Velocity = SplashKit.VectorMultiply(direction, SPEED);
        Radius = (int)(10 * gameScale);
        _shotCircle = SplashKit.CircleAt(X, Y, Radius);
    }

    public override bool HitCheck(Player player)
    {
        bool hit = false;
        if (SplashKit.BitmapCircleCollision(player.HitBMP(), player.X, player.Y, _shotCircle) && player.PlayerScore.IsDead == false)
        {
            hit = true;
        }

        return hit;
    }

    public override void Update()
    {
        X += _Velocity.X;
        Y += _Velocity.Y;
        _shotCircle = SplashKit.CircleAt(X, Y, Radius);
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
    private AnimationScript _EnergyBallScript;
    private Sprite _EnergyBallSprite;
    public RedEnergyBall(Point2D fromPT, Player player)
    {
        int SPEED = (int)(5 * gameScale);
        X = fromPT.X;
        Y = fromPT.Y;
        Point2D toPT = new Point2D();
        toPT.X = player.X;
        toPT.Y = player.Y;
        Vector2D direction = SplashKit.UnitVector(SplashKit.VectorPointToPoint(fromPT, toPT));
        _Velocity = SplashKit.VectorMultiply(direction, SPEED);

        if (SplashKit.HasBitmap("RedEnergyBall"))
        {
            _EnergyBall = SplashKit.BitmapNamed("RedEnergyBall");
        }
        else
        {
            _EnergyBall = SplashKit.LoadBitmap("RedEnergyBall", "RedEnergyBall.png");
            _EnergyBall.SetCellDetails(150, 150, 3, 3, 9);
        }
        if (SplashKit.HasAnimationScript("RedEnergyBall"))
        {
            _EnergyBallScript = SplashKit.AnimationScriptNamed("RedEnergyBall");
        }
        else
        {
            _EnergyBallScript = SplashKit.LoadAnimationScript("RedEnergyBall", "RedEnergyBall.txt");
        }

        _EnergyBallSprite = SplashKit.CreateSprite(_EnergyBall, _EnergyBallScript);
        _EnergyBallSprite.StartAnimation("Start");
        _EnergyBallSprite.AnchorPoint = new Point2D() { X = _EnergyBallSprite.Width / 2, Y = _EnergyBallSprite.Height / 2 };
        _EnergyBallSprite.Position = fromPT;



    }
    public override bool HitCheck(Player player)
    {
        bool hit = false;
        if (SplashKit.SpriteBitmapCollision(_EnergyBallSprite, player.HitBMP(), player.X, player.Y) && player.PlayerScore.IsDead == false)
        {
            hit = true;
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
        }
        return Offscreen;


    }

    public override void freesprite()
    {
        SplashKit.FreeSprite(_EnergyBallSprite);
    }

    ~RedEnergyBall()
    {
        Console.WriteLine("RedEnergyBall is destroyed");

    }

}

public class Laser : Shooting
{

    private Bitmap _laserImg;
    private AnimationScript _LaserScript;
    private Sprite _laserSprite;
    public Laser(Point2D fromPT, int Frame)
    {
        int SPEED = (int)(5 * gameScale);

        if (SplashKit.HasBitmap("ReallySmallLaser"))
        {
            _laserImg = SplashKit.BitmapNamed("ReallySmallLaser");
        }
        else
        {
            _laserImg = SplashKit.LoadBitmap("ReallySmallLaser", "ReallySmallLaser.png");
            _laserImg.SetCellDetails(300, 50, 1, 40, 40);
        }

        if (SplashKit.HasAnimationScript("LaserScript"))
        {
            _LaserScript = SplashKit.AnimationScriptNamed("LaserScript");
        }
        else
        {
            _LaserScript = SplashKit.LoadAnimationScript("LaserScript", "Laser40.txt");
        }

        _laserSprite = SplashKit.CreateSprite(_laserImg, _LaserScript);

        _laserSprite.Position = fromPT;

        _laserSprite.MoveTo(fromPT.X, fromPT.Y);
        _laserSprite.Dy = SPEED;
        _laserSprite.StartAnimation(Frame);

    }

    public override bool HitCheck(Player player)
    {
        bool hit = false;
        if (SplashKit.SpriteBitmapCollision(_laserSprite, player.HitBMP(), player.X, player.Y))
        {
            hit = true;

        }

        return hit;
    }
    public override bool HitCheck(Enemy enemy) { return false; }
    public override void Update()
    {
        _laserSprite.X += Convert.ToSingle(_Velocity.X);
        _laserSprite.Y += Convert.ToSingle(_Velocity.Y);

        Y = _laserSprite.Y;

    }

    public override bool IsOffscreen(Window screen)
    {
        bool Offscreen = false;

        if (_laserSprite.Offscreen())
        {
            Offscreen = true;

        }
        return Offscreen;


    }

    public override void freesprite()
    {
        SplashKit.FreeSprite(_laserSprite);
    }



}

using System;
using SplashKitSDK;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public abstract class Enemy
{

    private Window _gameWindow;
    public double X { get; protected set; }
    public double Y { get; protected set; }
    public bool IsDead { get; protected set; }
    protected bool _IsDying;
    protected int _IsDyingCount;
    public bool SpawnSmallRocks { get; protected set; }
    public bool SmallRocksSpawned { get; set; }


    protected double _Angle;
    protected Vector2D _Velocity;
    protected double _RotationSpeed;

    public double Height { get; protected set; }
    public double Width { get; protected set; }
    public int GetSpeed { get; protected set; }

    public virtual Circle[] HitCircle()
    {
        Circle[] Cir = {SplashKit.CircleAt(X + (Height / 2), Y + (Width / 2), Width / 4)};
        return Cir;
    }

    public Enemy(Window gameWindow)
    {
        _gameWindow = gameWindow;
        _IsDying = false;
        SpawnSmallRocks = false;
        SmallRocksSpawned = false;

    }

    public abstract void Draw();

    public void KillTest() { _IsDying = true; }

    private void Rotation(double change)
    {
        _Angle = (_Angle + change) % 360;
    }


    public virtual void Update()
    {

        Rotation(_RotationSpeed);
        if (_IsDying)
        {
            _IsDyingCount += 1;
        }
        else
        {
            X += _Velocity.X;
            Y += _Velocity.Y;
        }

    }

    public virtual Tuple<String, int> HitBy(Player wasHitBy)
    {
        if (_IsDying)
        { return new Tuple<string, int>("False", 0); }
        _IsDying = true;
        return new Tuple<string, int>("Life", -1);
    }

    public virtual Tuple<String, int> HitBy(Shooting wasHitBy)
    {
        if (_IsDying)
        { return new Tuple<string, int>("False", 0); }
        _IsDying = true;
        return new Tuple<string, int>("Score", 10);
    }

    public bool IsOffscreen(Window screen)
    {
        bool Offscreen = false;

        if (X < -Width | X > screen.Width | Y < -Height | Y > screen.Height)
        {
            Offscreen = true;
        }
        return Offscreen;
    }

    protected Point2D RndfromPt()
    {
        if (SplashKit.Rnd() < 0.5)
        {
            X = SplashKit.Rnd(_gameWindow.Width);
            if (SplashKit.Rnd() < 0.5)
            { Y = -Height; }
            else
            { Y = _gameWindow.Height; }
        }
        else
        {
            Y = SplashKit.Rnd(_gameWindow.Height);

            if (SplashKit.Rnd() < 0.5)
            { X = -Width; }
            else
            { X = _gameWindow.Width; }
        }

        return new Point2D()
        { X = X, Y = Y };
    }
}

public class RockLarge : Enemy
{
    private Bitmap _Rock;
    public RockLarge(Window gameWindow, int Speed, double RotationSpeed, int fX = -1, int fY = -1, int tX = -1, int tY = -1) : base(gameWindow)
    {
        _RotationSpeed = RotationSpeed;
        _Angle = 0;
        _Rock = new Bitmap("Rock1", "RockLarge.png");
        _Rock.SetCellDetails(200, 200, 3, 3, 9);
        Height = _Rock.CellHeight;
        Width = _Rock.CellWidth;
        GetSpeed = Speed;

        Point2D fromPt;
        Point2D tmpPT = base.RndfromPt();
        if (fX == -1)
        {
            fromPt.X = tmpPT.X;
        }
        else
        {
            X = fX;
            fromPt.X = fX;
        }

        if (fY == -1)
        {
            fromPt.Y = tmpPT.Y;
        }
        else
        {
            Y = fY;
            fromPt.Y = fY;
        }

        Point2D toPt;
        if (tX == -1)
        {
            toPt.X = gameWindow.Width / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.X = tX;
        }

        if (tY == -1)
        {
            toPt.Y = gameWindow.Height / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.Y = tY;
        }

        Vector2D dir;
        dir = SplashKit.UnitVector(SplashKit.VectorPointToPoint(fromPt, toPt));

        _Velocity = SplashKit.VectorMultiply(dir, Speed);
    }


    public override void Draw()
    {
        DrawingOptions opt;


        if (_IsDying == false)
        {
            opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));
            _Rock.Draw(X, Y, opt);
        }
        else
        {
            switch (_IsDyingCount)
            {
                case < 12:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(1));
                    break;
                case < 23:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(2));
                    break;
                case < 34:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(3));
                    break;
                case < 45:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(4));
                    SpawnSmallRocks = true;
                    break;
                case < 56:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(5));
                    break;
                case < 68:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(6));
                    break;
                case < 79:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(7));
                    break;
                case < 90:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(8));
                    IsDead = true;

                    break;
                default:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));

                    break;
            }
            _Rock.Draw(X, Y, opt);
        }
    }


}
public class RockMed : Enemy
{
    private Bitmap _Rock;

    public RockMed(Window gameWindow, int Speed, double RotationSpeed, int fX = -1, int fY = -1, int tX = -1, int tY = -1) : base(gameWindow)
    {
        _RotationSpeed = RotationSpeed;
        _Angle = 0;
        _Rock = new Bitmap("Rock2", "MedRockAll.png");
        _Rock.SetCellDetails(100, 100, 3, 2, 6);
        Height = _Rock.CellHeight;
        Width = _Rock.CellWidth;

        Point2D fromPt;
        Point2D tmpPT = base.RndfromPt();
        if (fX == -1)
        {
            fromPt.X = tmpPT.X;
        }
        else
        {
            X = fX;
            fromPt.X = fX;
        }

        if (fY == -1)
        {
            fromPt.Y = tmpPT.Y;
        }
        else
        {
            Y = fY;
            fromPt.Y = fY;
        }

        Point2D toPt;
        if (tX == -1)
        {
            toPt.X = gameWindow.Width / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.X = tX;
        }

        if (tY == -1)
        {
            toPt.Y = gameWindow.Height / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.Y = tY;
        }


        Vector2D dir;
        dir = SplashKit.UnitVector(SplashKit.VectorPointToPoint(fromPt, toPt));

        _Velocity = SplashKit.VectorMultiply(dir, Speed);
    }


    public override void Draw()
    {
        DrawingOptions opt;


        if (_IsDying == false)
        {
            opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));
            _Rock.Draw(X, Y, opt);
        }
        else
        {
            switch (_IsDyingCount)
            {
                case < 12:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(1));
                    break;
                case < 23:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(2));
                    break;
                case < 34:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(3));
                    break;
                case < 45:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(4));
                    break;
                case < 56:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(5));
                    IsDead = true;
                    break;
                default:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));

                    break;
            }
            _Rock.Draw(X, Y, opt);
        }
    }
}
public class RockSmall : Enemy
{
    private Bitmap _Rock;
    public RockSmall(Window gameWindow, int Speed, double RotationSpeed, int fX = -1, int fY = -1, int tX = -1, int tY = -1) : base(gameWindow)



    {
        _RotationSpeed = RotationSpeed;
        _Angle = 0;
        _Rock = new Bitmap("Rock3", "SmallRockAll.png");
        _Rock.SetCellDetails(75, 75, 3, 2, 5);
        Height = _Rock.CellHeight;
        Width = _Rock.CellWidth;



        Point2D fromPt;
        Point2D tmpPT = base.RndfromPt();
        if (fX == -1)
        {
            fromPt.X = tmpPT.X;
        }
        else
        {
            X = fX;
            fromPt.X = fX;
        }

        if (fY == -1)
        {
            fromPt.Y = tmpPT.Y;
        }
        else
        {
            Y = fY;
            fromPt.Y = fY;
        }

        Point2D toPt;
        if (tX == -1)
        {
            toPt.X = gameWindow.Width / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.X = tX;
        }

        if (tY == -1)
        {
            toPt.Y = gameWindow.Height / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.Y = tY;
        }

        Vector2D dir;
        dir = SplashKit.UnitVector(SplashKit.VectorPointToPoint(fromPt, toPt));

        _Velocity = SplashKit.VectorMultiply(dir, Speed);
    }


    public override void Draw()
    {
        DrawingOptions opt;


        if (_IsDying == false)
        {
            opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));
            _Rock.Draw(X, Y, opt);
        }
        else
        {
            switch (_IsDyingCount)
            {
                case < 12:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(1));
                    break;
                case < 23:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(2));
                    break;
                case < 34:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(3));
                    break;
                case < 45:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(4));
                    IsDead = true;
                    break;
                default:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));

                    break;
            }
            _Rock.Draw(X, Y, opt);
        }
    }

}




public class RockSmallTriple : Enemy
{
    private Bitmap _Rock;
    public RockSmallTriple(Window gameWindow, int Speed, double RotationSpeed, double sX, double sY, int rNum) : base(gameWindow)
    //You Are here trying to make this work for creating a tripple rocks 


    {
        _RotationSpeed = RotationSpeed;
        _Angle = 0;
        _Rock = new Bitmap("Rock3", "SmallRockAll.png");
        _Rock.SetCellDetails(75, 75, 3, 2, 6);
        Height = _Rock.CellHeight;
        Width = _Rock.CellWidth;

        X = sX; Y = sY;

        Point2D fromPt = new Point2D()
        { X = sX, Y = sY };
        Point2D toPt = new Point2D();
        switch (rNum)
        {
            case 1:
                toPt.X = gameWindow.Width / 2;
                toPt.Y = gameWindow.Height / 2;
                break;
            case 2:
                toPt.X = gameWindow.Width / 2;
                toPt.Y = gameWindow.Height / 2 - 100;
                break;
            case 3:
                toPt.X = gameWindow.Width / 2;
                toPt.Y = gameWindow.Height / 2 + 100;
                break;
            default:
                toPt.X = gameWindow.Width / 2 + SplashKit.Rnd(-100, 100);
                toPt.Y = gameWindow.Height / 2 + SplashKit.Rnd(-100, 100);
                break;
        }

        Vector2D dir;
        dir = SplashKit.UnitVector(SplashKit.VectorPointToPoint(fromPt, toPt));

        _Velocity = SplashKit.VectorMultiply(dir, Speed);
    }


    public override void Draw()
    {
        DrawingOptions opt;


        if (_IsDying == false)
        {
            opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));
            _Rock.Draw(X, Y, opt);
        }
        else
        {
            switch (_IsDyingCount)
            {
                case < 12:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(1));
                    break;
                case < 23:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(2));
                    break;
                case < 34:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(3));

                    break;
                case < 45:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(4));
                    IsDead = true;
                    break;
                default:
                    opt = SplashKit.OptionRotateBmp(_Angle, SplashKit.OptionWithBitmapCell(0));

                    break;
            }
            _Rock.Draw(X, Y, opt);
        }
    }

}

public class Boss1 : Enemy
{
    Bitmap _Boss;
    private bool _ShieldUp;
    private int _ShieldHealth;
    private bool _ShieldFlash;
    private int _DamageLoop;
    private string _Phase;
    private bool _XRight;
    private bool _YDown;

    public Boss1(Window gameWindow) : base(gameWindow)
    {
        _Boss = new Bitmap("Boss1", "MotherShipAll.png");
        _Boss.SetCellDetails(400, 300, 3, 2, 6);
        _RotationSpeed = 0;
        _Angle = 0;
        Height = _Boss.CellHeight;
        Width = _Boss.CellWidth;
        _ShieldUp = true;
        _ShieldHealth = 15; // was 300
        _ShieldFlash = false;
        _DamageLoop = 1;
        _Phase = "Start";
        X = 200;
        Y = -299;
        _XRight = true;
        _YDown = true;




    }

    public override void Draw()
    {
        DrawingOptions opt;

        if (_IsDying == false)
        {
            if (_ShieldUp)
            {
                if (_ShieldFlash == true)
                {
                    opt = SplashKit.OptionWithBitmapCell(1);
                    _ShieldFlash = false;
                }
                else
                {
                    opt = SplashKit.OptionWithBitmapCell(0);
                }
            }
            else
            {
                switch(_DamageLoop)
                {
                    case 1:
                        opt = SplashKit.OptionWithBitmapCell(3);
                        _DamageLoop = 2;
                        break;
                    case 2:
                        opt = SplashKit.OptionWithBitmapCell(4);
                        _DamageLoop = 3;
                        break;
                    case 3:
                        opt = SplashKit.OptionWithBitmapCell(5);
                        _DamageLoop = 1;
                        break;
                    default:
                        opt = SplashKit.OptionWithBitmapCell(2);
                        break;
                }
            }
            _Boss.Draw(X, Y, opt);
        }

    }

    public override void Update()
    {
        switch(_Phase)
        {
            case "Start":
                if(Y<=50)
                {
                    Y++;
                }
                else
                {
                    _Phase = "Mid";                   
                }
                break;
            case "Mid":
                    if(X<100) _XRight = true;
                    if(X+Width>700) _XRight = false;
                    if(Y<50) _YDown = true;
                    if(Y>70) _YDown = false;

                    if(_XRight == true)
                    {
                        X++;
                    }
                    else
                    {
                        X--;
                    }

                    if(_YDown == true)
                    {
                        Y++;
                    }
                    else
                    {
                        Y--;
                    }                   
                break;
            
            
        }
        if(_ShieldHealth<0) _ShieldUp = false;
    }



        public override Tuple<String,int> HitBy(Player wasHitBy)
    { 
/*             if(_IsDying)
        {return new Tuple<string, int>("False",0);}
        _IsDying = true;
        return new Tuple<string, int>("Life",-1);  */
        return new Tuple<string, int>("False",0);
    }
    
    public override Tuple<String, int> HitBy(Shooting wasHitBy)
    {
        /*         if(_IsDying)
                {return new Tuple<string, int>("False",0);} */
        //_IsDying = true;
        if (_ShieldUp)
        {
            _ShieldHealth -= 1;
            _ShieldFlash = true;
        }

        return new Tuple<string, int>("Score", 10);
    }
    public override Circle[] HitCircle()
    {
        Circle[] Cir = 
        {
            SplashKit.CircleAt(X + 198, Y + 116, 102),
            SplashKit.CircleAt(X + 293, Y + 112, 78),
            SplashKit.CircleAt(X + 102, Y + 112, 78)
        };
        return Cir;
    }

}







/* 
Game Asteroid Assets provided by
Hansjörg Malthaner, http://opengameart.org/users/varkalandar
Écrivain  https://opengameart.org/users/%C3%A9crivain, https://opengameart.org/content/rocks

 */

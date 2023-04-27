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
    public bool CanShoot { get; protected set; } //{ get { return CanShoot; } protected set { CanShoot = false; } }


    protected double _Angle;
    public Vector2D _Velocity {get; protected set;}
    protected double _RotationSpeed;

    public double Height { get; protected set; }
    public double Width { get; protected set; }
    public int GetSpeed { get; protected set; }

    public virtual Circle[] HitCircle()
    {
        Circle[] Cir = { SplashKit.CircleAt(X + (Height / 2), Y + (Width / 2), Width / 4) };
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

    public virtual Tuple<String, int> HitCheck(Player player)
    {
        return new Tuple<string, int>("False", 0);
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

    protected void SetCourse(int fX = -1, int fY = -1, int tX = -1, int tY = -1)
    {
        Point2D fromPt;
        Point2D tmpPT = RndfromPt();
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
            toPt.X = _gameWindow.Width / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.X = tX;
        }

        if (tY == -1)
        {
            toPt.Y = _gameWindow.Height / 2 + SplashKit.Rnd(-100, 100);
        }
        else
        {
            toPt.Y = tY;
        }

        X = fromPt.X;
        Y = fromPt.Y;

        Vector2D dir;
        dir = SplashKit.UnitVector(SplashKit.VectorPointToPoint(fromPt, toPt));
        _Velocity = SplashKit.VectorMultiply(dir, GetSpeed);
    }
}

public class RockLarge : Enemy
{
    protected AstGameObj _AGO;

    public RockLarge(Window gameWindow, int Speed, double RotationSpeed, int fX = -1, int fY = -1, int tX = -1, int tY = -1) : base(gameWindow)
    {
        _AGO = new AstGameObj(SplashKit.JsonFromFile("Enemy_Rock_Large.json"));     // load sprite with cell details and script
        _AGO.rotSpeed = (float)RotationSpeed;       // set rotation speed
        GetSpeed = Speed;           // set velocity magnitude

        SetCourse(fX,fY,tX,tY);     // randomized start and direction

        X -= _AGO._sprite.Width / 2;        // starting completely offscreen breaks the enemy list, original XY is based on top left corner of image
        Y -= _AGO._sprite.Height / 2;
        _AGO._sprite.X = (float)X;          // set sprite coords
        _AGO._sprite.Y = (float)Y;
        _AGO.setVelocity(_Velocity);        // set sprite velocity
    }

    public override Circle[] HitCircle()    // returns sprite circle that fits within rectangle
    {
        return new Circle[] {_AGO._sprite.CollisionCircle()};
    }

    public override Tuple<String, int> HitBy(Player wasHitBy)   // if called, start animation
    {
        if (!_IsDying)
        {
            _AGO._sprite.StartAnimation("explode");
        }
        return base.HitBy(wasHitBy);
    }

    public override Tuple<String, int> HitBy(Shooting wasHitBy) 
    {
        if (!_IsDying)
        {
            _AGO._sprite.StartAnimation("explode");
        }
        return base.HitBy(wasHitBy);
    }

    public override void Update()       // update rotation value, check if animations finished
    {
        X = _AGO._sprite.CenterPoint.X;
        Y = _AGO._sprite.CenterPoint.Y;
        _AGO.updateAngle();

        if (_AGO._sprite.AnimationName() == "explode" && _AGO._sprite.AnimationHasEnded)    // first stage, end will set spawn flag
        {
            SpawnSmallRocks = true;
            _AGO._sprite.StartAnimation("explode2");
        }
        else if (_AGO._sprite.AnimationName() == "explode2" && _AGO._sprite.AnimationHasEnded)  // second stage, end should destroy object, freesprite is not called in level due to currently set for bitmap
        {
            IsDead = true;
            SplashKit.FreeSprite(_AGO._sprite);
        }

        //base.Update();      // not used
    }

    public override void Draw()     
    {
        //Circle c = _AGO._sprite.CollisionCircle();    // used for debugging
        //c.Draw(Color.White);
        //_AGO._sprite.Draw();
    }

}
public class RockMed : Enemy
{
    protected AstGameObj _AGO;

    public RockMed(Window gameWindow, int Speed, double RotationSpeed, int fX = -1, int fY = -1, int tX = -1, int tY = -1) : base(gameWindow)
    {
        _AGO = new AstGameObj(SplashKit.JsonFromFile("Enemy_Rock_Medium.json"));

        _AGO.rotSpeed = (float)RotationSpeed;
        GetSpeed = Speed;

        SetCourse(fX,fY,tX,tY);

        X -= _AGO._sprite.Width / 2;
        Y -= _AGO._sprite.Height / 2;
        _AGO._sprite.X = (float)X;
        _AGO._sprite.Y = (float)Y;
        _AGO.setVelocity(_Velocity);
    }

    public override Circle[] HitCircle()
    {
        return new Circle[] {_AGO._sprite.CollisionCircle()};
    }

    public override Tuple<String, int> HitBy(Player wasHitBy)
    {
        if (!_IsDying)
        {
            _AGO._sprite.StartAnimation("explode");
        }
        return base.HitBy(wasHitBy);
    }

    public override Tuple<String, int> HitBy(Shooting wasHitBy)
    {
        if (!_IsDying)
        {
            _AGO._sprite.StartAnimation("explode");
        }
        return base.HitBy(wasHitBy);
    }

    public override void Update()
    {
        X = _AGO._sprite.CenterPoint.X;
        Y = _AGO._sprite.CenterPoint.Y;
        _AGO.updateAngle();

        if (_AGO._sprite.AnimationName() == "explode" && _AGO._sprite.AnimationHasEnded)
        {
            IsDead = true;
            SplashKit.FreeSprite(_AGO._sprite);
        }

        base.Update();
    }

    public override void Draw()
    {
        //Circle c = _AGO._sprite.CollisionCircle();
        //c.Draw(Color.White);
        //_AGO._sprite.Draw();
    }
}
public class RockSmall : Enemy
{
    protected AstGameObj _AGO;
        public RockSmall(Window gameWindow, int Speed, double RotationSpeed, int fX = -1, int fY = -1, int tX = -1, int tY = -1) : base(gameWindow)
    {
        _AGO = new AstGameObj(SplashKit.JsonFromFile("Enemy_Rock_Small.json"));

        _AGO.rotSpeed = (float)RotationSpeed;
        GetSpeed = Speed;

        SetCourse(fX,fY,tX,tY);

        X -= _AGO._sprite.Width / 2;
        Y -= _AGO._sprite.Height / 2;
        _AGO._sprite.X = (float)X;
        _AGO._sprite.Y = (float)Y;
        _AGO.setVelocity(_Velocity);
    }

    public RockSmall(Window gameWindow, Vector2D Vel, double Rot, Point2D Start) : base(gameWindow)
    {
        _AGO = new AstGameObj(SplashKit.JsonFromFile("Enemy_Rock_Small.json"));
        _AGO.rotSpeed = (float)Rot;
        GetSpeed = (int)SplashKit.VectorMagnitude(Vel);

        _AGO._sprite.X = (float)Start.X;
        _AGO._sprite.Y = (float)Start.Y;
        _AGO.setVelocity(Vel);
    }

    public override Circle[] HitCircle()
    {
        return new Circle[] {_AGO._sprite.CollisionCircle()};
    }

    public override Tuple<String, int> HitBy(Player wasHitBy)
    {
        if (!_IsDying)
        {
            _AGO._sprite.StartAnimation("explode");
        }
        return base.HitBy(wasHitBy);
    }

    public override Tuple<String, int> HitBy(Shooting wasHitBy)
    {
        if (!_IsDying)
        {
            _AGO._sprite.StartAnimation("explode");
        }
        return base.HitBy(wasHitBy);
    }


    public override void Update()
    {
        X = _AGO._sprite.CenterPoint.X;
        Y = _AGO._sprite.CenterPoint.Y;
        _AGO.updateAngle();

        if (_AGO._sprite.AnimationName() == "explode" && _AGO._sprite.AnimationHasEnded)
        {
            IsDead = true;
            SplashKit.FreeSprite(_AGO._sprite);
        }

        base.Update();
    }


    public override void Draw()
    {
        //Circle c = _AGO._sprite.CollisionCircle();
        //c.Draw(Color.White);
        //_AGO._sprite.Draw();
    }

}

public class Boss1 : Enemy
{
    private Bitmap _Boss;
    private Animation _BossAnimation;
    private AnimationScript _BossScript;
    private DrawingOptions _opt;
    private bool _ShieldUp;
    private int _ShieldHealth, _ShipHealth;
    private bool _ShieldFlash;
    private string _Phase;
    private bool _XRight;
    private bool _YDown;
    private List<Shooting> _shots = new List<Shooting>();
    private List<Shooting> _KillShots = new List<Shooting>();
    private SplashKitSDK.Timer _shootingTime;
    private Window _gameWindow;
    private int _shootingSmallShot, _shootingEnergyShot;
    private Game _game;


    public Boss1(Window gameWindow, Game game) : base(gameWindow)
    {
        _Boss = new Bitmap("Boss1", "MotherShipAll.png");
        _Boss.SetCellDetails(400, 300, 3, 2, 6);
        _BossScript = SplashKit.LoadAnimationScript("MotherShip", "MotherShip1.txt");
        _BossAnimation = _BossScript.CreateAnimation("ShieldUp");
        _RotationSpeed = 0;
        _Angle = 0;
        Height = _Boss.CellHeight;
        Width = _Boss.CellWidth;
        _ShieldUp = true;
        _ShieldHealth = 100; // was 100
        _ShieldFlash = false;
        _ShipHealth = 50;
        _Phase = "Start";
        X = gameWindow.Width / 2 - _Boss.CellWidth / 2;
        Y = -299;
        _XRight = true;
        _YDown = true;
        CanShoot = true;
        _opt = SplashKit.OptionWithAnimation(_BossAnimation);
        _shootingTime = SplashKit.CreateTimer("Boss1Shooting");
        _shootingTime.Stop();
        _shootingTime.Reset();
        _shootingSmallShot = 1;
        _shootingEnergyShot = 1;
        _gameWindow = gameWindow;
        _game = game;




    }

    public override void Draw()
    {


        if (_IsDying == false)
        {
            if (_ShieldUp)
            {
                if (_ShieldFlash == true)
                {
                    if (_BossAnimation.Ended) _BossAnimation.Assign("ShieldFlash");
                    _ShieldFlash = false;
                }
                else
                {
                    if (_BossAnimation.Ended) _BossAnimation.Assign("ShieldUp");
                }
            }
            else
            {
                if (_ShipHealth <= 10)
                {
                    if (_BossAnimation.Ended) _BossAnimation.Assign("CriticalDamage");
                }
                else
                {
                    if (_BossAnimation.Ended) _BossAnimation.Assign("ShieldDown");
                }

            }
        }
        else
        {
            if (_BossAnimation.Ended) _BossAnimation.Assign("CriticalDamage");
            //if (_BossAnimation.Ended) _BossAnimation.Assign("Dying"); // to be added to the animation
        }

        foreach (Shooting s in _shots)
        {
            s.Draw();
        }

        _Boss.Draw(X, Y, _opt);

    }

    public override void Update()
    {
        _BossAnimation.Update();
        switch (_Phase)
        {
            case "Start":
                if (Y <= 50)
                {
                    Y++;

                }
                else
                {
                    _Phase = "Mid";
                }
                break;
            case "Mid":
                if (X < 200) _XRight = true;
                if (X + Width > _gameWindow.Width - 200) _XRight = false;
                if (Y < 50) _YDown = true;
                if (Y > 70) _YDown = false;

                if (_XRight == true)
                {
                    X += 2;
                }
                else
                {
                    X -= 2;
                }

                if (_YDown == true)
                {
                    Y++;
                }
                else
                {
                    Y--;
                }

                if (!_shootingTime.IsStarted) _shootingTime.Start();

                if (_shootingTime.Ticks / 800 == _shootingSmallShot)
                {
                    ShootSmall();
                    _shootingSmallShot++;
                }

                if (_shootingTime.Ticks / 5000 == _shootingEnergyShot)
                {
                    RedEnergyBall();
                    _shootingEnergyShot++;
                }
                break;
            case "End":
                if (X < 200) _XRight = true;
                if (X + Width > _gameWindow.Width - 200) _XRight = false;
                if (Y < 50) _YDown = true;
                if (Y > 70) _YDown = false;

                if (_XRight == true)
                {
                    X += 2;
                }
                else
                {
                    X -= 2;
                }

                if (_YDown == true)
                {
                    Y++;
                }
                else
                {
                    Y--;
                }
                if (_shootingTime.Ticks / 2000 == _shootingEnergyShot)
                {
                    RedEnergyBall();
                    _shootingEnergyShot++;
                }
                break;
            case "Dead":
                if (_BossAnimation.Ended && _shots.Count == 0)  IsDead = true;

                break;
        }
        if (_ShieldHealth < 0)
        {
            if (_ShieldUp == true)
            {
                _Phase = "End";
                _shootingTime.Reset();
                _shootingEnergyShot = 1;
            }
            _ShieldUp = false;

        }
        if (_ShipHealth < 0)
        {
            _IsDying = true;
            _Phase = "Dead";
        }


        foreach (Shooting s in _shots)
        {
            s.Update();
            if (s.IsOffscreen(_gameWindow)) _KillShots.Add(s);
        }

        foreach (Shooting s in _KillShots)
        {
            s.freesprite();
            _shots.Remove(s);
        }
        _KillShots.Clear();

    }



    public override Tuple<String, int> HitBy(Player wasHitBy)
    {
        /*             if(_IsDying)
                {return new Tuple<string, int>("False",0);}
                _IsDying = true;
                return new Tuple<string, int>("Life",-1);  */
        return new Tuple<string, int>("Life", -1);
        // return new Tuple<string, int>("False", 0);
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
        else
        {
            _ShipHealth -= 1;
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

    public override Tuple<String, int> HitCheck(Player player)
    {
        foreach (Shooting s in _shots)
        {
            if (s.HitCheck(player))
            {
                _KillShots.Add(s);
                if (!player.IsInvulnerable)
                {
                    return new Tuple<string, int>("Life", -1);
                }
            }
        }
        return new Tuple<string, int>("False", 0);

    }

    private void ShootSmall()
    {
        Shooting ShotType;
        Point2D pt1 = new Point2D();
        Point2D pt2 = new Point2D();
        Point2D pt3 = new Point2D();
        pt1.X = X + _Boss.CellCenter.X;
        pt1.Y = Y + _Boss.CellCenter.Y;
        pt2.X = X + _Boss.CellCenter.X - 20;
        pt2.Y = Y + _Boss.CellCenter.Y;
        pt3.X = X + _Boss.CellCenter.X + 20;
        pt3.Y = Y + _Boss.CellCenter.Y;

        ShotType = new BossSmallShot(pt1, 90);
        _shots.Add(ShotType);
        ShotType = new BossSmallShot(pt1, 70);
        _shots.Add(ShotType);
        ShotType = new BossSmallShot(pt1, 110);
        _shots.Add(ShotType);

    }

    private void RedEnergyBall()
    {
        Point2D fromPT = new Point2D();
        fromPT.X = X + _Boss.CellCenter.X;
        fromPT.Y = Y + _Boss.CellCenter.Y;
        foreach (Player p in _game.Players)
        {
            Shooting ShotType = new RedEnergyBall(fromPT, p);
            _shots.Add(ShotType);
        }

    }
}







/* 
Game Asteroid Assets provided by
Hansjörg Malthaner, http://opengameart.org/users/varkalandar
Écrivain  https://opengameart.org/users/%C3%A9crivain, https://opengameart.org/content/rocks
Boss 1 Assets constructed form paid canva assets

 */

using System;
using SplashKitSDK;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using static AsteroidsGame.Program;

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
    public Vector2D _Velocity { get; protected set; }
    protected double _RotationSpeed;

    public double Height { get; protected set; }
    public double Width { get; protected set; }
    public int GetSpeed { get; protected set; }

    public virtual Circle[] HitCircle()
    {
        Circle[] Cir = { SplashKit.CircleAt(X + (Height / 2), Y + (Width / 2), Width / 4) };
        return Cir;
    }
    public virtual Sprite HitSprite()
    {
        return null;
    }
    public Enemy(Window gameWindow)
    {
        _gameWindow = gameWindow;
        _IsDying = false;
        SpawnSmallRocks = false;
        SmallRocksSpawned = false;
        SplashKit.SelectSpritePack("Enemies");

    }

    public abstract void Draw();

    public void KillTest() { _IsDying = true; }

    protected void Rotation(double change)
    {
        _Angle = (_Angle + change) % 360;
    }

    public virtual Tuple<String, int> HitCheck(Player player)
    {
        return new Tuple<string, int>("False", 0);
    }

    public virtual void freesprite()
    {
        //Does nothing but allows inherited classes to free their own sprite
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

        wasHitBy.freesprite();
        return new Tuple<string, int>("Score", 10);
    }

    public virtual bool IsOffscreen(Window screen)
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
        //_AGO._sprite.Scale = 0.5F;
        _AGO.rotSpeed = (float)RotationSpeed;       // set rotation speed

        GetSpeed = (int)(Speed * gameScale);           // set velocity magnitude
        if (GetSpeed == 0) { GetSpeed = 1; }            // prevent zero velocity

        SetCourse(fX, fY, tX, tY);     // randomized start and direction

        X -= _AGO._sprite.Width / 2;        // starting completely offscreen breaks the enemy list, original XY is based on top left corner of image
        Y -= _AGO._sprite.Height / 2;
        _AGO._sprite.X = (float)X;          // set sprite coords
        _AGO._sprite.Y = (float)Y;
        _AGO.setVelocity(_Velocity);        // set sprite velocity
        Height = _AGO._sprite.Height;       // set height and width
        Width = _AGO._sprite.Width;
    }

    public override Circle[] HitCircle()    // returns sprite circle that fits within rectangle
    {
        return new Circle[] { _AGO._sprite.CollisionCircle() };
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

    public override bool IsOffscreen(Window screen)
    {
        return _AGO._sprite.Offscreen();
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
        GetSpeed = (int)(Speed * gameScale);           // set velocity magnitude
        if (GetSpeed == 0) { GetSpeed = 1; }            // prevent zero velocity

        SetCourse(fX, fY, tX, tY);

        X -= _AGO._sprite.Width / 2;
        Y -= _AGO._sprite.Height / 2;
        _AGO._sprite.X = (float)X;
        _AGO._sprite.Y = (float)Y;
        _AGO.setVelocity(_Velocity);
        Height = _AGO._sprite.Height;       // set height and width
        Width = _AGO._sprite.Width;
    }

    public override Circle[] HitCircle()
    {
        return new Circle[] { _AGO._sprite.CollisionCircle() };
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

    public override bool IsOffscreen(Window screen)
    {
        return _AGO._sprite.Offscreen();
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
        GetSpeed = (int)(Speed * gameScale);           // set velocity magnitude
        if (GetSpeed == 0) { GetSpeed = 1; }            // prevent zero velocity

        SetCourse(fX, fY, tX, tY);

        X -= _AGO._sprite.Width / 2;
        Y -= _AGO._sprite.Height / 2;
        _AGO._sprite.X = (float)X;
        _AGO._sprite.Y = (float)Y;
        _AGO.setVelocity(_Velocity);
        Height = _AGO._sprite.Height;       // set height and width
        Width = _AGO._sprite.Width;
    }

    public RockSmall(Window gameWindow, Vector2D Vel, double Rot, Point2D Start) : base(gameWindow)
    {
        _AGO = new AstGameObj(SplashKit.JsonFromFile("Enemy_Rock_Small.json"));
        _AGO.rotSpeed = (float)Rot;
        GetSpeed = (int)SplashKit.VectorMagnitude(Vel);

        _AGO._sprite.X = (float)(Start.X - _AGO._sprite.CenterPoint.X);
        _AGO._sprite.Y = (float)(Start.Y - _AGO._sprite.CenterPoint.Y);
        _AGO.setVelocity(Vel);
    }

    public override Circle[] HitCircle()
    {
        return new Circle[] { _AGO._sprite.CollisionCircle() };
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

    public override bool IsOffscreen(Window screen)
    {
        return _AGO._sprite.Offscreen();
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

public class BlueRock : Enemy
{
    private Bitmap _Rock;
    private Sprite _RockSprite;
    private AnimationScript _RockAnimation;
    public BlueRock(Window gameWindow, int Speed, double RotationSpeed, int fX = -1, int fY = -1, int tX = -1, int tY = -1) : base(gameWindow)
    {
        _RotationSpeed = RotationSpeed;
        _Angle = 0;
        if (SplashKit.HasBitmap("RockBlue"))
        {
            _Rock = SplashKit.BitmapNamed("RockBlue");
        }
        else
        {
            _Rock = SplashKit.LoadBitmap("Rock4", "BlueRockSpriteSheet.png");
            _Rock.SetCellDetails(150, 150, 3, 3, 9);
        }

        if (SplashKit.HasAnimationScript("RockBlueAnimation"))
        {
            _RockAnimation = SplashKit.AnimationScriptNamed("RockBlueAnimation");
        }
        else
        {
            _RockAnimation = SplashKit.LoadAnimationScript("RockBlueAnimation", "BlueRock.txt");
        }

        _RockSprite = SplashKit.CreateSprite(_Rock, _RockAnimation);
        //_RockSprite.AnchorPoint = new Point2D() { X = 75, Y = 75 };
        _RockSprite.AnchorPoint = _RockSprite.CenterPoint;
        _RockSprite.StartAnimation("normal");

        _RockSprite.AddValue("Health", 5);
        Height = _Rock.CellHeight;
        Width = _Rock.CellWidth;
        GetSpeed = (int)(Speed * gameScale);           // set velocity magnitude
        if (GetSpeed == 0) { GetSpeed = 1; }            // prevent zero velocity
        SetCourse(fX, fY, tX, tY);
    }
    public override Circle[] HitCircle()
    {
        if (!_IsDying) return new Circle[] { _RockSprite.CollisionCircle() };
        return new Circle[] { SplashKit.CircleAt(-5, -5, 1) };
    }
    public override void Update()
    {

        if (_IsDying)
        {
            _IsDyingCount += 1;
        }
        else
        {
            Rotation(_RotationSpeed);
            _RockSprite.Rotation = Convert.ToSingle(_Angle);
            X += _Velocity.X;
            Y += _Velocity.Y;
            _RockSprite.MoveTo(X, Y);
        }

        if (_RockSprite.AnimationName() == "dying" && _RockSprite.AnimationHasEnded) IsDead = true;

    }
    public override Tuple<String, int> HitBy(Shooting wasHitBy)
    {
        int tmpscore = 0;
        if (_IsDying) return new Tuple<string, int>("False", 0);
        if (_RockSprite.Value("Health") > 0)
        {
            _RockSprite.SetValue("Health", _RockSprite.Value("Health") - 1);
            _RockSprite.StartAnimation("hit");
            tmpscore = 10;
        }
        if (_RockSprite.Value("Health") == 0)
        {
            _IsDying = true;
            _RockSprite.StartAnimation("dying");
            tmpscore = +50; // may need to check this, can't remember if it's meant to be trying to add 50 or just set it to 50 

        }
        return new Tuple<string, int>("Score", tmpscore);
    }

    public override Tuple<String, int> HitBy(Player wasHitBy)
    {
        if (_IsDying)
        { return new Tuple<string, int>("False", 0); }
        _IsDying = true;
        _RockSprite.StartAnimation("dying");
        return new Tuple<string, int>("Life", -1);
    }
    public override void freesprite()
    {
        SplashKit.FreeSprite(_RockSprite);
    }

    public override Sprite HitSprite()
    {
        return _RockSprite;
    }
    public override void Draw()
    { }



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
        if (SplashKit.HasBitmap("Boss1"))
        {
            _Boss = SplashKit.BitmapNamed("Boss1");
        }
        else
        {
            _Boss = SplashKit.LoadBitmap("Boss1", "MotherShipAll.png");
            _Boss.SetCellDetails(400, 300, 3, 2, 6);
        }

        if (SplashKit.HasAnimationScript("MotherShip"))
        {
            _BossScript = SplashKit.AnimationScriptNamed("MotherShip");
        }
        else
        {
            _BossScript = SplashKit.LoadAnimationScript("MotherShip", "MotherShip1.txt");
        }

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
        X = (gameWindow.Width / 2 - _Boss.CellWidth / 2);
        Y = -299 * gameScale;
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
                if (Y <= 50 * gameScale)
                {
                    Y++;

                }
                else
                {
                    _Phase = "Mid";
                }
                break;
            case "Mid":
                if (X < 200 * gameScale) _XRight = true;
                if (X + Width > _gameWindow.Width - (200 * gameScale)) _XRight = false;
                if (Y < 50 * gameScale) _YDown = true;
                if (Y > 70 * gameScale) _YDown = false;

                if (_XRight == true)
                {
                    X += 2 * gameScale;
                }
                else
                {
                    X -= 2 * gameScale;
                }

                if (_YDown == true)
                {
                    // Y++;
                    Y += 1 * gameScale;
                }
                else
                {
                    // Y--;
                    Y -= 1 * gameScale;
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
                if (X < 200 * gameScale) _XRight = true;
                if (X + Width > _gameWindow.Width - (200 * gameScale)) _XRight = false;
                if (Y < 50 * gameScale) _YDown = true;
                if (Y > 70 * gameScale) _YDown = false;

                if (_XRight == true)
                {
                    X += 2 * gameScale;
                }
                else
                {
                    X -= 2 * gameScale;
                }

                if (_YDown == true)
                {
                    Y += 1 * gameScale;
                }
                else
                {
                    Y -= 1 * gameScale;
                }
                if (_shootingTime.Ticks / 2000 == _shootingEnergyShot)
                {
                    RedEnergyBall();
                    _shootingEnergyShot++;
                }
                break;
            case "Dead":
                if (_BossAnimation.Ended && _shots.Count == 0) IsDead = true;

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

    public override void freesprite()
    {
        foreach (Shooting s in _KillShots)
        {
            s.freesprite();
            _shots.Remove(s);
        }
        foreach (Shooting s in _shots)
        {
            s.freesprite();
            _shots.Remove(s);
        }
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
            SplashKit.CircleAt(X + (198 * gameScale), Y + (116 * gameScale), 102 * gameScale),
            SplashKit.CircleAt(X + (293 * gameScale), Y + (112 * gameScale), 78 * gameScale),
            SplashKit.CircleAt(X + (102 * gameScale), Y + (112 * gameScale), 78 * gameScale)
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
        pt2.X = X + _Boss.CellCenter.X - (20 * gameScale);
        pt2.Y = Y + _Boss.CellCenter.Y;
        pt3.X = X + _Boss.CellCenter.X + (20 * gameScale);
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
            if (!p.PlayerScore.IsDead)
            {
                Shooting ShotType = new RedEnergyBall(fromPT, p);
                _shots.Add(ShotType);
            }
        }

    }
}



public class Boss2 : Enemy
{
    protected Bitmap _Boss;
    protected Animation _BossAnimation;
    protected AnimationScript _BossScript;

    protected string _Phase;
    private List<Shooting> _shots = new List<Shooting>();
    private List<Shooting> _KillShots = new List<Shooting>();
    protected SplashKitSDK.Timer _MoveTimer, _ShotTimer;
    protected Window _gameWindow;
    private int _shootingSmallShot, _shootingEnergyShot;
    protected Game _game;
    private List<Sprite> _ShipList = new List<Sprite>();
    private List<double> _time = new List<double>();
    private double _radius;
    private int _Y_point;
    protected int _playerKillThreshold;




    public Boss2(Window gameWindow, Game game) : base(gameWindow)
    {

        _gameWindow = gameWindow;
        _game = game;

        if (SplashKit.HasBitmap("Boss2"))
        {
            _Boss = SplashKit.BitmapNamed("Boss2");
        }
        else
        {
            _Boss = SplashKit.LoadBitmap("Boss2", "MotherShipAll.png");
            _Boss.SetCellDetails(400, 300, 3, 2, 6);
        }
        if (SplashKit.HasAnimationScript("SmallBossShips"))
        {
            _BossScript = SplashKit.AnimationScriptNamed("SmallBossShips");
        }
        else
        {
            _BossScript = SplashKit.LoadAnimationScript("SmallBossShips", "SmallBossShips.txt");
        }
        double window_3rd = _gameWindow.Width / 4;
        Height = _Boss.CellHeight;
        Width = _Boss.CellWidth;

        Console.WriteLine("Ship Spawn");
        for (int i = 1; i <= 3; i++)
        {

            // _tmpShip.VectorTo(toPoint);

        }
        _Y_point = (int)(-Height);
        _Phase = "Main";
        Console.WriteLine("Constructor Done");

        if (!SplashKit.HasTimer("Boss2MoveTimer")) _MoveTimer = SplashKit.CreateTimer("Boss2MoveTimer");
        else _MoveTimer = SplashKit.TimerNamed("Boss2MoveTimer");
        if (!_MoveTimer.IsStarted) _MoveTimer.Start();

        if (!SplashKit.HasTimer("Boss2Shoot")) _ShotTimer = SplashKit.CreateTimer("Boss2Shoot");
        else _ShotTimer = SplashKit.CreateTimer("Boss2Shoot");
        _ShotTimer.Stop();



        //if player goes above this they will be killed
        _playerKillThreshold = (int)(300 * gameScale);

        // Set up variables for the figure 8 pattern
        _time.Add((-1000)); //far left
        _time.Add((0)); //middle
        _time.Add(+1000); //far right

        CanShoot = true;


        // _BossAnimation = _BossScript.CreateAnimation("ShieldUp");
        // _RotationSpeed = 0;
        // _Angle = 0;


        // X = gameWindow.Width / 2 - _Boss.CellWidth / 2;
        // Y = -299;


        // _opt = SplashKit.OptionWithAnimation(_BossAnimation);
        // _shootingTime = SplashKit.CreateTimer("Boss1Shooting");
        // _shootingTime.Stop();
        // _shootingTime.Reset();
    }

    public override void Draw()
    {
        if (_IsDying == false)
        {
            foreach (Shooting s in _shots)
            {
                s.Draw();
            }
        }
    }
    public override void Update()
    {

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

    protected Shooting RedEnergyBall(Player p, Sprite s)
    {
        Point2D fromPT = new Point2D();
        fromPT.X = s.X + _Boss.CellCenter.X;
        fromPT.Y = s.Y + _Boss.CellCenter.Y;
        Shooting ShotType = new RedEnergyBall(fromPT, p);
        return ShotType;

    }
}


public class smallShip : Boss2
{
    private Sprite _Ship;
    private int _Ypoint;
    private int _time;
    private List<Shooting> _shots = new List<Shooting>();
    private List<Shooting> _LaserShots = new List<Shooting>();
    private List<Shooting> _KillShots = new List<Shooting>();
    private List<Shooting> _KillLaserShots = new List<Shooting>();
    protected SplashKitSDK.Timer _RedEnergyBallTimer, _shootingTime;
    private int _FrameCount;
    private bool _firstShot;
    private bool _LaserFiring = false;

    public smallShip(Window gameWindow, Game game, int ShipNo) : base(gameWindow, game)
    {

        SplashKit.SelectSpritePack("Enemies");
        _Ship = SplashKit.CreateSprite(_Boss, _BossScript);
        _Ship.AddValue("Health", 50);
        _Ship.StartAnimation("ShieldUp");
        //_Ship.MoveTo(window_3rd * i - (Width / 2), -Height);
        // Point2D toPoint = new Point2D { X = window_3rd * i - (Width / 2), Y = 200 };
        _Ypoint = (int)(-Height);

        if (!SplashKit.HasTimer("Boss2RedEnergyBall" + ShipNo)) _RedEnergyBallTimer = SplashKit.CreateTimer("Boss2RedEnergyBall" + ShipNo);
        else _RedEnergyBallTimer = SplashKit.CreateTimer("Boss2RedEnergyBall" + ShipNo);
        _RedEnergyBallTimer.Stop();

        if (!SplashKit.HasTimer("Boss2Shooting" + ShipNo)) _shootingTime = SplashKit.CreateTimer("Boss2Shooting" + ShipNo);
        else _shootingTime = SplashKit.CreateTimer("Boss2Shooting" + ShipNo);
        _shootingTime.Reset();
        if (!_shootingTime.IsStarted) _shootingTime.Start();

        _MoveTimer.Resume();

        switch (ShipNo) // Set up variables for the figure 8 pattern
        {
            case 0:
                //_time = (int)(-1000 * gameScale); //far left
                _time = -1000; //far left
                break;
            case 1:
                _time = 0; //middle
                break;
            case 2:
                //_time = (int)(1000 * gameScale); //far right
                _time = 1000; //far right
                break;
            default:
                _time = 0;
                break;
        }
    }

    public override void Update()
    {
        int yChange = _Ypoint > 60 ? 0 : 2;
        _Ypoint += yChange;


        int b = (int)(600 * gameScale);
        int c = (int)(200 * gameScale);
        double t = (_MoveTimer.Ticks + _time) * 2 * Math.PI / 5000;
        //Console.WriteLine(_MoveTimer.Ticks / 1000);
        // Calculate the new position based on the Lemniscate of Bernoulli curve
        float x = (float)((_gameWindow.Width / 2 - Width / 2) + b * Math.Cos(t) / (1 + Math.Pow(Math.Sin(t), 2)));
        float y = (float)(_Ypoint + c * Math.Sin(t) * Math.Cos(t) / (1 + Math.Pow(Math.Sin(t), 2)));

        // Set the sprite position
        _Ship.X = x;
        _Ship.Y = y;


        if (_MoveTimer.Ticks / 1000 > 2)
        {
            if (_shootingTime.Ticks / 100 > 40)
            {
                _MoveTimer.Pause();
                _shootingTime.Stop();
                if (!_ShotTimer.IsStarted) _ShotTimer.Start();
                FireLaser();

            }

            // if (_ShotTimer.Ticks / 1000 > 2)
            // {
            //     _shootingTime.Start();
            //     _ShotTimer.Stop();
            //     _MoveTimer.Resume();
            // }
        }


        if (_Ship.Value("Health") <= 0)
        {
            _IsDying = true;
            if (!(_Ship.AnimationName() == "criticaldamage")) _Ship.StartAnimation("criticaldamage");
        }
        if (_Ship.AnimationName() == "criticaldamage")
        {
            if (_Ship.AnimationHasEnded) IsDead = true;
        }
        PlayerUpdate();
        ShotUpdate();
        LaserUpdate();


    }

    private void PlayerUpdate()
    {
        foreach (Player p in _game.Players)
        {
            if (p.Y < _playerKillThreshold && !p.PlayerScore.IsDead)
            {
                if (!_RedEnergyBallTimer.IsStarted) _RedEnergyBallTimer.Start();
                if (_RedEnergyBallTimer.Ticks / 500 > 1)
                {
                    _RedEnergyBallTimer.Stop();

                    _shots.Add(RedEnergyBall(p, _Ship));
                }
            }
        }
    }

    private void FireLaser()
    {
        Console.WriteLine("Fire Laser Called");
        _LaserFiring = true;
        _firstShot = true;
        _FrameCount = 39;
    }
    private void LaserUpdate()
    {
        if (_LaserFiring)
        {
            int ShotCount = _shots.Count();


            Point2D fromPT = new Point2D { X = _Ship.X + (int)(50 * gameScale), Y = _Ship.CenterPoint.Y - (int)(40 * gameScale) };

            //Console.WriteLine("LaserFiring");
            if (_firstShot)
            {
                //Console.WriteLine("First Shot");
                Shooting ShotType = new Laser(fromPT, _FrameCount);
                _LaserShots.Add(ShotType);
                _FrameCount--;
                _firstShot = false;

            }
            else
            {
                Shooting target = _LaserShots.Last();
                // Console.WriteLine("Target y " + target.Y);
                // Console.WriteLine("Shot Count " + _shots.Count());
                //Console.WriteLine("Remaining Shots");
                if (target.Y > _Ship.CenterPoint.Y - (int)(40 * gameScale) + SplashKit.BitmapNamed("ReallySmallLaser").CellHeight - 1)
                {
                    // Console.WriteLine("Second All" +u);
                    Shooting ShotType = new Laser(fromPT, _FrameCount);
                    _LaserShots.Add(ShotType);
                    _FrameCount--;
                }
            }

            // Console.WriteLine("Ouside Loop");
            // if (Array.TrueForAll(_FrameCount, value =>
            // {
            //     Console.WriteLine((value < 0));
            //     return (value < 0);
            // }))
            if (_FrameCount < 0)
            {
                //Console.WriteLine("LaserFiring = false");
                _LaserFiring = false;
                _ShotTimer.Stop();
                _shootingTime.Start();
                _MoveTimer.Resume();
            }
        }
    }

    private void ShotUpdate()
    {
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

        foreach (Shooting s in _LaserShots)
        {
            s.Update();
            if (s.IsOffscreen(_gameWindow)) _KillLaserShots.Add(s);
        }

        foreach (Shooting s in _KillLaserShots)
        {
            s.freesprite();
            _LaserShots.Remove(s);
        }
        _KillLaserShots.Clear();
    }
    public override void freesprite()
    {
        foreach (Shooting s in _KillShots)
        {
            s.freesprite();
            _shots.Remove(s);
        }
        foreach (Shooting s in _shots)
        {
            s.freesprite();
        }

        foreach (Shooting s in _KillLaserShots)
        {
            s.freesprite();
            _shots.Remove(s);
        }
        foreach (Shooting s in _LaserShots)
        {
            s.freesprite();
        }
        SplashKit.FreeSprite(_Ship);


    }

    public override Tuple<String, int> HitBy(Shooting wasHitBy)
    {
        if (_IsDying) return new Tuple<string, int>("False", 0);

        _Ship.SetValue("Health", _Ship.Value("Health") - 1);
        Console.WriteLine(_Ship.Value("Health"));
        _Ship.StartAnimation("ShieldFlash");

        return new Tuple<string, int>("Score", 10);
    }
    public override Circle[] HitCircle()
    {
        Circle[] Cir =
        {
            SplashKit.CircleAt(_Ship.X  + (198 * gameScale), _Ship.Y  + (116 * gameScale), 102 * gameScale),
            SplashKit.CircleAt(_Ship.X  + (293 * gameScale), _Ship.Y  + (112 * gameScale), 78 * gameScale),
            SplashKit.CircleAt(_Ship.X  + (102 * gameScale), _Ship.Y  + (112 * gameScale), 78 * gameScale)
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
        foreach (Shooting s in _LaserShots)
        {
            if (s.HitCheck(player))
            {
                if (!player.IsInvulnerable)
                {
                    return new Tuple<string, int>("Life", -1);
                }
            }
        }
        return new Tuple<string, int>("False", 0);
    }




}



/* 
Game Asteroid Assets provided by
Hansjörg Malthaner, http://opengameart.org/users/varkalandar
Écrivain  https://opengameart.org/users/%C3%A9crivain, https://opengameart.org/content/rocks
Boss 1 Assets constructed form paid canva assets

 */

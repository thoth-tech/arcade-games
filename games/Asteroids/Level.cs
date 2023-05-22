using System;
using SplashKitSDK;
using System.Collections.Generic;
using static AsteroidsGame.Program;

public abstract class Level
{

    public List<Enemy> Enemies { get; protected set; } = new List<Enemy>();
    private List<Enemy> _tmpEnemySpawn = new List<Enemy>();
    protected Dictionary<string, bool> _EnemySpawned = new Dictionary<string, bool>();
    protected Window _gameWindow;
    protected Game _game;
    protected int _wHeight, _wWidth;
    protected Bitmap lvlComplete;
    private SplashKitSDK.Timer _completeTimer;
    private Level? _NextLevel;
    public enum rockTypes
    {
        Small = 1,
        Medium = 2,
        Large = 4,
        Blue = 8,
        All = 16
    }
    public Level(Window GameWindow, Game game)
    {
        _gameWindow = GameWindow;
        _game = game;
        _wHeight = _gameWindow.Height;
        _wWidth = _gameWindow.Width;
        if (SplashKit.HasBitmap("levelComplete"))
        {
            lvlComplete = SplashKit.BitmapNamed("levelComplete");
        }
        else
        {
            lvlComplete = SplashKit.LoadBitmap("levelComplete", "Lvlcomplete.png");
        }

        _completeTimer = new SplashKitSDK.Timer("completeTimer");

    }

    public virtual void Draw()
    {
        foreach (Enemy e in Enemies)
        {
            e.Draw();
        }
        levelCompleteDraw();
    }

    protected void levelComplete(Level NextLevel)
    {
        if (!_completeTimer.IsStarted) _completeTimer.Start();
        _NextLevel = NextLevel;
    }

    private void levelCompleteDraw()
    {
        double Timer = Math.Round((_completeTimer.Ticks / 1000.0), 1);
        int X_GameText = _gameWindow.Width / 2 - lvlComplete.Width / 2;
        int Y_GameText = _gameWindow.Height / 6;
        if (_completeTimer.IsStarted && Timer < 5.0)
        {
            SplashKit.DrawBitmap(lvlComplete, X_GameText, Y_GameText);
        }
        else if (_completeTimer.IsStarted && Timer > 5.0)
        {
            _completeTimer.Stop();
            _completeTimer.Reset();
            if (_NextLevel != null) _game.Nextlvl(_NextLevel);

        }

    }

    public Enemy createEnemy(rockTypes Type, int Speed = 4, int sX = -1, int sY = -1, int tX = -1, int tY = -1)
    {
        //Enemy rock = new Enemy(_gameWindow,4,0.3);
        Enemy rock;

        switch (Type)
        {
            case rockTypes.Large:
                rock = new RockLarge(_gameWindow, Speed, 0.3, sX, sY, tX, tY);
                break;
            case rockTypes.Small:
                rock = new RockSmall(_gameWindow, Speed, 0.3, sX, sY, tX, tY);
                break;
            case rockTypes.Medium:
                rock = new RockMed(_gameWindow, Speed, 0.3, sX, sY, tX, tY);
                break;
            case rockTypes.Blue:
                rock = new BlueRock(_gameWindow, Speed, 0.3, sX, sY, tX, tY);
                break;
            default:
                rock = new RockLarge(_gameWindow, Speed, 0.3);
                break;
        }
        return rock;
    }

    /*
    public void createEnemy(double sX, double sY, int rNum, int Speed = 4)
    {
        Enemy rock = new RockSmallTriple(_gameWindow, Speed, 0.3, sX, sY, rNum);
        _tmpEnemySpawn.Add(rock);
    }
    */


    public void spawnTriple(Enemy e)
    {
        Point2D start = new Point2D() { X = e.X, Y = e.Y };
        Vector2D vel = e._Velocity;
        double velMag = SplashKit.VectorMagnitude(vel);
        double velAngle = SplashKit.VectorAngle(vel);

        _tmpEnemySpawn.Add(new RockSmall(_gameWindow, vel, 0.3, start));
        _tmpEnemySpawn.Add(new RockSmall(_gameWindow, SplashKit.VectorFromAngle(velAngle - (int)(30 * gameScale), velMag), 0.3, start));
        _tmpEnemySpawn.Add(new RockSmall(_gameWindow, SplashKit.VectorFromAngle(velAngle + (int)(30 * gameScale), velMag), 0.3, start));

    }

    protected void SpawnRockWall(rockTypes rockType, string Location, int speed = 4)
    {
        int X, Y, xChange, yChange, changeEnd, change, targetxy;
        bool moveX = false; //If you are not moving X then we are moving Y
        Enemy RockRef;
        switch (Location)
        {
            case "Top":
                X = 0;
                Y = 0;
                moveX = true;
                RockRef = createEnemy(rockType, speed, X, Y, X, _wHeight);
                Enemies.Add(RockRef);
                xChange = Convert.ToInt32(RockRef.Width) + 10;
                yChange = 0;
                change = xChange;
                changeEnd = _wWidth;
                targetxy = _wHeight;
                break;

            case "Bottom":
                X = 0;
                Y = _wHeight;
                moveX = false;
                RockRef = createEnemy(rockType, speed, X, _wHeight, X, 0);
                Enemies.Add(RockRef);
                xChange = Convert.ToInt32(RockRef.Width) + 10;
                yChange = 0;
                change = xChange;
                changeEnd = _wWidth;
                targetxy = 0;

                break;

            case "Left":
                X = 0;
                Y = 0;
                moveX = false;
                RockRef = createEnemy(rockType, speed, X, Y, _wWidth, Y);
                Enemies.Add(RockRef);
                xChange = 0;
                yChange = Convert.ToInt32(RockRef.Height) + 10;
                change = yChange;
                changeEnd = _wHeight;
                targetxy = _wWidth;
                break;

            case "Right":
                X = _wWidth;
                Y = 0;
                moveX = false;
                RockRef = createEnemy(rockType, speed, X, Y, 0, Y);
                Enemies.Add(RockRef);
                xChange = 0;
                yChange = Convert.ToInt32(RockRef.Height) + 10;
                change = yChange;
                changeEnd = _wHeight;
                targetxy = 0;
                break;
            default:
                Console.WriteLine("Direction value must be Top, Bottom, Right or Left");
                X = 0;
                Y = 0;
                xChange = 0;
                yChange = 0;
                change = 10;
                changeEnd = 0;
                targetxy = 0;
                break;

        }

        while (change < changeEnd)
        {
            X += xChange;
            Y += yChange;
            if (moveX)
            {
                Enemies.Add(createEnemy(rockType, speed, X, Y, X, targetxy));
                change += xChange;
            }
            else
            {
                Enemies.Add(createEnemy(rockType, speed, X, Y, targetxy, Y));
                change += yChange;
            }
        }
    }

    public virtual void Update()
    {
        List<Enemy> KillEnemy = new List<Enemy>();

        _tmpEnemySpawn.Clear();

        foreach (Enemy e in Enemies)
        {
            e.Update();
            if (e.IsOffscreen(_gameWindow)) KillEnemy.Add(e);
            if (e.SpawnSmallRocks && !e.SmallRocksSpawned)
            {
                spawnTriple(e);
                e.SmallRocksSpawned = true;
            }
            if (e.IsDead) KillEnemy.Add(e);
        }

        foreach (Enemy e in _tmpEnemySpawn)
        {
            Enemies.Insert(0, e);
        }


        foreach (Enemy e in KillEnemy)
        {
            e.freesprite();
            Enemies.Remove(e);
        }


    }

    public void RockRandomSpawn(int Speed = 4, double SpawnRate = 0.01, rockTypes spawnTypes = rockTypes.Small | rockTypes.Medium | rockTypes.Large) //Deafult Random Spawn rate for rocks 
    {
        if (SplashKit.Rnd() < SpawnRate)
        {

            List<rockTypes> allowedRocks = new List<rockTypes>();
            if ((spawnTypes & rockTypes.Small) != 0) allowedRocks.Add(rockTypes.Small);
            if ((spawnTypes & rockTypes.Medium) != 0) allowedRocks.Add(rockTypes.Medium);
            if ((spawnTypes & rockTypes.Large) != 0) allowedRocks.Add(rockTypes.Large);
            if ((spawnTypes & rockTypes.Blue) != 0) allowedRocks.Add(rockTypes.Blue);
            if ((spawnTypes & rockTypes.All) != 0)
            {
                allowedRocks.Add(rockTypes.Small);
                allowedRocks.Add(rockTypes.Medium);
                allowedRocks.Add(rockTypes.Large);
                allowedRocks.Add(rockTypes.Blue);
            }

            if (allowedRocks.Count > 0)
            {
                rockTypes selectedRock = allowedRocks[SplashKit.Rnd(allowedRocks.Count)];
                Enemies.Add(createEnemy(selectedRock, Speed));
            }
        }
    }
}
public class Level1 : Level
{
    private SplashKitSDK.Timer _lvlTimer;
    private Font _GameFont;


    public Level1(Window GameWindow, Game game) : base(GameWindow, game)
    {
        _lvlTimer = new SplashKitSDK.Timer("lvl1Timer");
        _lvlTimer.Start();
        _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        _EnemySpawned.Clear();

    }

    public override void Draw()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        int FontSize = (int)(80 * gameScale);
        int Y_GameText = _gameWindow.Height / 6;
        string gameHeading = "Level 1";
        double gameHeading_X = _gameWindow.Width / 2 - SplashKit.TextWidth(gameHeading, _GameFont, FontSize) / 2;

        if (lvlTimer < 1.5) //1.5
        {
            SplashKit.DrawTextOnWindow(_gameWindow, gameHeading, Color.White, _GameFont, FontSize, gameHeading_X, Y_GameText);
        }
        /*         else if (lvlTimer > 151 && lvlTimer < 160) //151
                {
                    SplashKit.DrawBitmap(lvlComplete,129,262);
                } */

        base.Draw();

    }

    public override void Update()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);


        switch (lvlTimer)
        {
            case < 1.5:
                //don't spawn until level starts
                break;
            case < 10:
                RockRandomSpawn(1);
                break;
            case < 20:
                RockRandomSpawn(2);
                break;
            case < 30:
                RockRandomSpawn(3);
                break;
            case < 40:
                RockRandomSpawn(4, 0.02);
                break;
            case < 50:
                RockRandomSpawn(5, 0.03);
                break;
            case 55: //150.0
                if (!_EnemySpawned.ContainsKey("55 Time"))
                {
                    _EnemySpawned.Add("55 Time", true);
                    Enemies.Add(createEnemy(rockTypes.Large, 4, _wWidth / 2, 0, _wWidth / 2, _wHeight / 2));
                    Enemies.Add(createEnemy(rockTypes.Large, 4, _wWidth / 2, _wHeight, _wWidth / 2, _wHeight / 2));
                    Enemies.Add(createEnemy(rockTypes.Large, 4, 0, _wHeight / 2, _wWidth / 2, _wHeight / 2));
                    Enemies.Add(createEnemy(rockTypes.Large, 4, _wWidth, _wHeight / 2, _wWidth / 2, _wHeight / 2));
                }
                break;
            case 60: //153
                if (!_EnemySpawned.ContainsKey("Boss1"))
                {
                    _EnemySpawned.Add("Boss1", true);
                    Enemies.Add(new Boss1(_gameWindow, _game));
                }
                break;

        }

        if (_EnemySpawned.ContainsKey("Boss1") && Enemies.Count == 0)
        {
            levelComplete(new Level2(_gameWindow, _game));
        }

        base.Update();
    }
}

public class Level2 : Level
{
    private SplashKitSDK.Timer _lvlTimer;
    private Font _GameFont;
    private int X_GameText, Y_GameText;

    public Level2(Window GameWindow, Game game) : base(GameWindow, game)
    {
        _lvlTimer = new SplashKitSDK.Timer("lvl2Timer");
        _lvlTimer.Start();
        _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        _game = game;
        _EnemySpawned.Clear();

    }

    public override void Draw()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        int FontSize = (int)(80 * gameScale);
        Y_GameText = _gameWindow.Height / 6;
        string gameHeading = "Level 2";
        double gameHeading_X = _gameWindow.Width / 2 - SplashKit.TextWidth(gameHeading, _GameFont, FontSize) / 2;
        if (lvlTimer < 2) //1.5
        {
            SplashKit.DrawTextOnWindow(_gameWindow, gameHeading, Color.White, _GameFont, FontSize, gameHeading_X, Y_GameText);
        }

        base.Draw();

    }


    public override void Update()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        switch (lvlTimer)
        {
            case < 1.5:
                //don't spawn until level starts
                break;
            case < 15:
                RockRandomSpawn(4);
                break;
            case 15.1:
                if (!_EnemySpawned.ContainsKey("BlueRock 15.1"))
                {
                    _EnemySpawned.Add("BlueRock 15.1", true);
                    for (int i = 0; i < 5; i++)
                    {
                        Enemies.Add(createEnemy(rockTypes.Blue));
                    }
                }
                break;
            case < 30:
                RockRandomSpawn(4, 0.02, rockTypes.All);
                break;

            case 35:
                if (!_EnemySpawned.ContainsKey("RockwallSmall"))
                {
                    _EnemySpawned.Add("RockwallSmall", true);
                    SpawnRockWall(rockTypes.Small, "Top");
                }
                break;

            case 36:
                if (!_EnemySpawned.ContainsKey("RockwallBlue"))
                {
                    _EnemySpawned.Add("RockwallBlue", true);
                    SpawnRockWall(rockTypes.Blue, "Top");
                }
                break;

            case 37:
                if (!_EnemySpawned.ContainsKey("RockwallLarge"))
                {
                    _EnemySpawned.Add("RockwallLarge", true);
                    SpawnRockWall(rockTypes.Large, "Top");
                }
                break;

            case 39:
                if (!_EnemySpawned.ContainsKey("RockwallSmall1"))
                {
                    _EnemySpawned.Add("RockwallSmall1", true);
                    SpawnRockWall(rockTypes.Small, "Top");
                }
                break;
            case 40:
                if (!_EnemySpawned.ContainsKey("RockwallSmall2"))
                {
                    _EnemySpawned.Add("RockwallSmall2", true);
                    SpawnRockWall(rockTypes.Small, "Top");
                }
                break;
            case 41:
                if (!_EnemySpawned.ContainsKey("RockwallSmall3"))
                {
                    _EnemySpawned.Add("RockwallSmall3", true);
                    SpawnRockWall(rockTypes.Small, "Top");
                }
                break;
            case 45: //153

                if (!_EnemySpawned.ContainsKey("Boss2"))
                {
                    _EnemySpawned.Add("Boss2", true);
                    Enemies.Add(new smallShip(_gameWindow, _game, 0));
                    Enemies.Add(new smallShip(_gameWindow, _game, 1));
                    Enemies.Add(new smallShip(_gameWindow, _game, 2));
                }
                break;

        }

        if (_EnemySpawned.ContainsKey("Boss2") && Enemies.Count == 0)
        {

            levelComplete(new EndGame(_gameWindow, _game));


        }

        base.Update();
    }
}

public class EndGame : Level
{
    private SplashKitSDK.Timer _lvlTimer;
    private Font _GameFont;
    private int X_GameText, Y_GameText;

    public EndGame(Window GameWindow, Game game) : base(GameWindow, game)
    {
        _lvlTimer = new SplashKitSDK.Timer("EndGameTimer");
        _lvlTimer.Start();
        _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        _game = game;
        _EnemySpawned.Clear();
        foreach (Player p in _game.Players)
        {
            p.PlayerScore.PauseTimer();
        }

    }

    public override void Draw()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        int FontSize = (int)(80 * gameScale);
        int FontSize2 = (int)(60 * gameScale);
        X_GameText = _gameWindow.Width / 2 - (int)(115 * gameScale);
        Y_GameText = _gameWindow.Height / 6;
        string gameHeading = "Game Complete";
        string gameText = "Congratulations, You have reached the end of the game,";
        string gameText2 = "we hope you enjoyed it as much as we did making it.";

        double gameHeading_X = _gameWindow.Width / 2 - SplashKit.TextWidth(gameHeading, _GameFont, FontSize) / 2;
        double gameText_X = _gameWindow.Width / 2 - SplashKit.TextWidth(gameText, _GameFont, FontSize2) / 2;
        double gameText2_X = _gameWindow.Width / 2 - SplashKit.TextWidth(gameText2, _GameFont, FontSize2) / 2;
        double gameHeading_Y_Offset = SplashKit.TextHeight(gameHeading, _GameFont, FontSize);
        double gameText_Y_Offset = SplashKit.TextHeight(gameText, _GameFont, FontSize2);

        if (lvlTimer < 10) //1.5
        {
            SplashKit.DrawTextOnWindow(_gameWindow, gameHeading, Color.White, _GameFont, FontSize, gameHeading_X, Y_GameText);
            SplashKit.DrawTextOnWindow(_gameWindow, gameText, Color.White, _GameFont, FontSize2, gameText_X, Y_GameText + gameHeading_Y_Offset);
            SplashKit.DrawTextOnWindow(_gameWindow, gameText2, Color.White, _GameFont, FontSize2, gameText2_X, Y_GameText + gameHeading_Y_Offset + gameText_Y_Offset);
        }
        else
        {
            _game.GameEndCleanup();
        }


        base.Draw();

    }
}
public class Debuglvl : Level
{
    private SplashKitSDK.Timer _lvlTimer;
    private Font _GameFont;
    private int X_GameText, Y_GameText;

    public Debuglvl(Window GameWindow, Game game) : base(GameWindow, game)
    {
        _lvlTimer = new SplashKitSDK.Timer("DebugTimer");
        _lvlTimer.Start();
        _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");
        _game = game;

    }

    public override void Draw()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        int FontSize = (int)(80 * gameScale);
        Y_GameText = _gameWindow.Height / 6;
        string gameHeading = "Testing";
        double gameHeading_X = _gameWindow.Width / 2 - SplashKit.TextWidth(gameHeading, _GameFont, FontSize) / 2;

        if (lvlTimer == 0)
        {
            Console.WriteLine("level timer reset");
        }



        if (lvlTimer < 1.5) //1.5
        {
            SplashKit.DrawTextOnWindow(_gameWindow, gameHeading, Color.White, _GameFont, FontSize, gameHeading_X, Y_GameText);
        }

        base.Draw();

    }

    public override void Update()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        // Enemies.Add(createEnemy("Large", 4, _wWidth / 2 - 100, -200, _wWidth / 2 - 100, _wHeight / 2 - 100));
        switch (lvlTimer)
        {
            case < 1.5:
                //don't spawn until level starts
                break;
            case > 1.5: //153
                if (!_EnemySpawned.ContainsKey("Boss1"))
                {
                    _EnemySpawned.Add("Boss1", true);
                    Enemies.Add(new Boss1(_gameWindow, _game));
                }
                break;
                // if (SplashKit.Rnd() < 0.01)
                // {
                //     Enemies.Add(new BlueRock(_gameWindow, 4, 3));
                // }
                // break;
                // RockRandomSpawn(4, 0.01, rockTypes.Large);
                // break;
                // if (!_EnemySpawned.ContainsKey("Rockwall"))
                // {
                //     _EnemySpawned.Add("Rockwall", true);
                //     SpawnRockWall(rockTypes.Blue, "Top");
                // }
                // break;

                // if (!_EnemySpawned.ContainsKey("Boss2"))
                // {
                //     _EnemySpawned.Add("Boss2", true);
                //     Enemies.Add(new smallShip(_gameWindow, _game, 0));
                //     Enemies.Add(new smallShip(_gameWindow, _game, 1));
                //     Enemies.Add(new smallShip(_gameWindow, _game, 2));
                // }
                // break;
        }
        if (_EnemySpawned.ContainsKey("Boss1") && Enemies.Count == 0)
        {
            //levelComplete(new Level2(_gameWindow, _game));
            levelComplete(null);

        }

        base.Update();
    }


}
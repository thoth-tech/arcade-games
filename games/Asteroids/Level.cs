using System;
using SplashKitSDK;
using System.Collections.Generic;

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

    public Level(Window GameWindow, Game game)
    {
        _gameWindow = GameWindow;
        _game = game;
        _wHeight = _gameWindow.Height;
        _wWidth = _gameWindow.Width;
        lvlComplete = new Bitmap("levelComplete", "Lvlcomplete.png");
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

    public Enemy createEnemy(string Type, int Speed = 4, int sX = -1, int sY = -1, int tX = -1, int tY = -1)
    {
        //Enemy rock = new Enemy(_gameWindow,4,0.3);
        Enemy rock;

        switch (Type)
        {
            case "Large":
                rock = new RockLarge(_gameWindow, Speed, 0.3, sX, sY, tX, tY);

                break;
            case "Small":
                rock = new RockSmall(_gameWindow, Speed, 0.3);

                break;
            case "Med":
                rock = new RockMed(_gameWindow, Speed, 0.3);
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

    // function reuses existing small rock enemy
    // velocity vector is inherited from original, instead of moving towards a center point again
    public void spawnTriple(Enemy e)
    {
        Point2D start = new Point2D() {X = e.X, Y = e.Y};
        Vector2D vel = e._Velocity;
        double velMag = SplashKit.VectorMagnitude(vel);
        double velAngle = SplashKit.VectorAngle(vel);


        _tmpEnemySpawn.Add(new RockSmall(_gameWindow, vel,0.3,start));
        _tmpEnemySpawn.Add(new RockSmall(_gameWindow, SplashKit.VectorFromAngle(velAngle - 30,velMag),0.3,start));
        _tmpEnemySpawn.Add(new RockSmall(_gameWindow, SplashKit.VectorFromAngle(velAngle + 30,velMag),0.3,start));

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
            Enemies.Remove(e);
        }


    }

    public void RockRandomSpawn(int Speed = 4, double SpawnRate = 0.01) //Deafult Random Spawn rate for rocks 
    {
        if (SplashKit.Rnd() < SpawnRate)
        {
            int TempNo = SplashKit.Rnd(0, 90);
            if (TempNo < 30)
            {
                Enemies.Add(createEnemy("Small", Speed));
            }
            else if (TempNo < 60)
            {
                Enemies.Add(createEnemy("Med", Speed));
            }
            else
            {
                Enemies.Add(createEnemy("Large", Speed));
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

    }

    public override void Draw()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        const int FontSize = 80;
        int X_GameText = _gameWindow.Width / 2 - 115;
        int Y_GameText = _gameWindow.Height / 6;

        if (lvlTimer < 1.5) //1.5
        {
            SplashKit.DrawTextOnWindow(_gameWindow, "Level 1", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
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
                    Enemies.Add(createEnemy("Large", 4, _wWidth / 2 - 100, -200, _wWidth / 2 - 100, _wHeight / 2 - 100));
                    Enemies.Add(createEnemy("Large", 4, _wWidth / 2 - 100, _wHeight, _wWidth / 2 - 100, _wHeight / 2 - 100));
                    Enemies.Add(createEnemy("Large", 4, -200, _wHeight / 2 - 100, _wWidth / 2 - 100, _wHeight / 2 - 100));
                    Enemies.Add(createEnemy("Large", 4, _wWidth, _wHeight / 2 - 100, _wWidth / 2 - 100, _wHeight / 2 - 100));
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

    }

    public override void Draw()
    {
        double lvlTimer = Math.Round((_lvlTimer.Ticks / 1000.0), 1);
        const int FontSize = 80;
        X_GameText = _gameWindow.Width / 2 - 115;
        Y_GameText = _gameWindow.Height / 6;

        if (lvlTimer < 2) //1.5
        {
            SplashKit.DrawTextOnWindow(_gameWindow, "Level 2 Place Hold", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
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
            case 2: //153
                _game.GameOver();
                break;

        }


        base.Update();
    }
}

public class Jsonlvl : Level
{
    private SplashKitSDK.Timer _lvlTimer;       // this will reset every spawn time it reaches
    private Font _GameFont;
    
    private Json _JsonLevel;
    private List<Json> _JsonSpawns;
    private int _JsonIndex;
    private Json? _Wave;

    public Jsonlvl(Window GameWindow, Game game, String lvlFP) : base(GameWindow, game)
    {
        _lvlTimer = new SplashKitSDK.Timer("lvl1Timer");
        _lvlTimer.Start();
        _GameFont = new Font("pricedown_bl", "fonts/pricedown_bl.otf");

        _JsonLevel = SplashKit.JsonFromFile(lvlFP);
        _JsonIndex = 0;
        _JsonSpawns = new List<Json>();
        _JsonLevel.ReadArray("waves", ref _JsonSpawns);


    }


    public override void Draw()
    {
        int framesOn = _JsonLevel.ReadInteger("title_frameson");
        if (framesOn < _JsonLevel.ReadInteger("title_framesdur"))    // COUNTS BY FRAMES
        {
            DrawTitle();
            _JsonLevel.AddNumber("title_frameson",framesOn + 1);
        }   

        base.Draw();
    }
    
    public override void Update()
    {
        
        if (_JsonIndex < _JsonSpawns.Count)     // while spawn waves exists
        {
            /*
            if (_Wave != null)  // delayed sequential spawn section, wave has started but not ended yet, needs json keys to used
            {

            }
            else */
            if ( _lvlTimer.Ticks >= _JsonSpawns[_JsonIndex].ReadInteger("ticks"))    // when timer has reached tick count
            {
                
                _Wave = _JsonSpawns[_JsonIndex];

                WaveSpawnAll();
                _JsonIndex++;
                _lvlTimer.Reset();
            }
        }
        else if (Enemies.Count == 0)
        {
            levelComplete(new Level1(_gameWindow, _game));
        }
        

        base.Update();
    }

    private void DrawTitle()
    {
        const int FontSize = 80;
        String text = _JsonLevel.ReadString("title");
        int X_GameText = (_gameWindow.Width - SplashKit.TextWidth(text,_GameFont,FontSize))/ 2;
        int Y_GameText = _gameWindow.Height / 6;

        SplashKit.DrawTextOnWindow(_gameWindow, text, Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
        
    }

    private void WaveSpawnAll()
    {
        int count = _Wave.ReadInteger("num");
        String type = _Wave.ReadString("type");
        int speed = _Wave.HasKey("speed") ? _Wave.ReadInteger("speed") : 4;

        for (int i = 0; i < count; i++)
        {
            Enemies.Add(createEnemy(type,speed));
        }

        _Wave = null;
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
        const int FontSize = 80;
        X_GameText = _gameWindow.Width / 2 - 115;
        Y_GameText = _gameWindow.Height / 6;

        if (lvlTimer == 0)
        {
            Console.WriteLine("level timer reset");
        }



        if (lvlTimer < 1.5) //1.5
        {
            SplashKit.DrawTextOnWindow(_gameWindow, "Testing", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
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
            case 5: //153
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
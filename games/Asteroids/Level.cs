using System;
using SplashKitSDK;
using System.Collections.Generic;

public abstract class Level
{

    public List<Enemy> Enemies{get; protected set;} = new List<Enemy>();
    private List<Enemy> _tmpEnemySpawn = new List<Enemy>();
    protected Dictionary<string,bool> _EnemySpawned = new Dictionary<string, bool>();
    protected Window _gameWindow;
    protected Game _game;
    protected int _wHeight, _wWidth;
    protected Bitmap lvlComplete;


    public Level(Window GameWindow, Game game)
    {
        _gameWindow = GameWindow;
        _game = game;
        _wHeight = _gameWindow.Height;
        _wWidth = _gameWindow.Width;
        lvlComplete = new Bitmap("levelComplete","Lvlcomplete.png");
    }

    public virtual void Draw()
    {
        foreach(Enemy e in Enemies)
        {e.Draw();}
      
    }

    public Enemy createEnemy(string Type,int Speed = 4,int sX = -1, int sY = -1, int tX = -1, int tY = -1)
    {
         //Enemy rock = new Enemy(_gameWindow,4,0.3);
                Enemy rock;

        switch (Type)
        {
            case "Large":
                rock = new RockLarge(_gameWindow,Speed,0.3,sX,sY,tX,tY);
                
                break;
            case "Small":
                rock = new RockSmall(_gameWindow,Speed,0.3);
                
                break;
            case "Med":
                rock = new RockMed(_gameWindow,Speed,0.3);
                break;
            default:
                rock = new RockLarge(_gameWindow,Speed,0.3);
                break;
        }
        return rock;
    }

    public void createEnemy(double sX, double sY,int rNum,int Speed = 4)
    {
        Enemy rock = new RockSmallTriple(_gameWindow,Speed,0.3,sX,sY,rNum);
        _tmpEnemySpawn.Add(rock);
    }


    public virtual void Update()
    {
        List<Enemy> KillEnemy = new List<Enemy>();
        
        _tmpEnemySpawn.Clear();

        foreach(Enemy e in Enemies)
        {
            e.Update();
            if (e.IsOffscreen(_gameWindow)) KillEnemy.Add(e);
            if (e.SpawnSmallRocks && !e.SmallRocksSpawned)
            {
                createEnemy(e.X + e.Width / 2, e.Y + e.Height / 2, 1,e.GetSpeed);
                createEnemy(e.X + e.Width / 2, e.Y + e.Height / 2, 2,e.GetSpeed);
                createEnemy(e.X + e.Width / 2, e.Y + e.Height / 2, 3,e.GetSpeed);
                e.SmallRocksSpawned = true;
            }
            if (e.IsDead) KillEnemy.Add(e);
        }

        foreach(Enemy e in _tmpEnemySpawn)
        {
            Enemies.Insert(0,e);
        }

        
        foreach(Enemy e in KillEnemy)
        {
            Enemies.Remove(e);
        }
            

    }

    public void RockRandomSpawn(int Speed = 4, double SpawnRate = 0.01) //Deafult Random Spawn rate for rocks 
    {
        if (SplashKit.Rnd() < SpawnRate) 
        {
            int TempNo = SplashKit.Rnd(0,90);
            if(TempNo<30)
            {
                    Enemies.Add(createEnemy("Small",Speed));
            }
            else if(TempNo<60)
            {
                    Enemies.Add(createEnemy("Med",Speed));
            }
            else
            {
                    Enemies.Add(createEnemy("Large",Speed));
            }           
        }
    }


}

public class Level1: Level
{
    private Timer _lvlTimer;
    private Font _GameFont;
    

    public Level1(Window GameWindow,Game game): base(GameWindow,game )
    {
        _lvlTimer = new Timer("lvl1Timer");
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
        double lvlTimer = Math.Round((_lvlTimer.Ticks /1000.0),1);      

        switch(lvlTimer)
        {
            case < 1.5:
                //don't spawn until level starts
            break;
            case < 50:
                RockRandomSpawn(2);
            break;
            case < 100:
                RockRandomSpawn(4);
            break;
            case < 125:
                RockRandomSpawn(6);
            break; 
            case 126: //150.0
                if(!_EnemySpawned.ContainsKey("150 Time"))
                {   
                    _EnemySpawned.Add("150 Time",true);
                    Enemies.Add(createEnemy("Large",4,_wWidth/2-100,-200,_wWidth/2-100,_wHeight/2-100));
                    Enemies.Add(createEnemy("Large",4,_wWidth/2-100,_wHeight,_wWidth/2-100,_wHeight/2-100));
                    Enemies.Add(createEnemy("Large",4,-200,_wHeight/2-100,_wWidth/2-100,_wHeight/2-100));
                    Enemies.Add(createEnemy("Large",4,_wWidth,_wHeight/2-100,_wWidth/2-100,_wHeight/2-100));

/*                  Enemies.Add(createEnemy("Large",4,0,0,_wWidth/2-100,_wHeight/2-100));
                    Enemies.Add(createEnemy("Large",4,0,_wHeight,_wWidth/2-100,_wHeight/2-100));
                    Enemies.Add(createEnemy("Large",4,_wWidth,_wHeight,_wWidth/2-100,_wHeight/2-100));
                    Enemies.Add(createEnemy("Large",4,0,0,_wWidth/2-100,_wHeight/2-100)); */
                }
                break;
            case 128: //153
                if(!_EnemySpawned.ContainsKey("Boss1"))
                {
                    _EnemySpawned.Add("Boss1",true);
                    Enemies.Add(new Boss1(_gameWindow));
                }
            break;

        } 
        base.Update();
    }
    
}
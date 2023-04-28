using SplashKitSDK;

    

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

    // OLD IMPLEMENTATION, string type, not enumerable
    public  Enemy createEnemy(string Type, int Speed = 4, int sX = -1, int sY = -1, int tX = -1, int tY = -1)
    {
        //Enemy rock = new Enemy(_gameWindow,4,0.3);
        Enemy rock;

        switch (Type)
        {
            case "Large":
                rock = new RockLarge(_gameWindow, Speed, 0.3, sX, sY, tX, tY);

                break;
            case "Small":
                rock = new RockSmall(_gameWindow, Speed, 0.3,sX,sY,tX,tY);

                break;
            case "Med":
                rock = new RockMed(_gameWindow, Speed, 0.3,sX,sY,tX,tY);
                break;
            case "Blue":
                rock = new BlueRock(_gameWindow, Speed, 0.3, sX, sY, tX, tY);
                break;
            case "Boss1":

                //break;
            default:
                rock = new RockLarge(_gameWindow, Speed, 0.3);
                break;
        }
        return rock;
    }


}

using System;
using SplashKitSDK;
using System.Collections.Generic;

// 
// 
// 
// 
// 
// 

public class Menu
{
    private Window _gameWindow;
    public bool GameStarted { get; private set; }
    public int players { get; private set; }
    public string? p1Ship { get; private set; }
    public string? p2Ship { get; private set; }
    public bool quit { get; private set; }
    private MenuOption _Menu;
    private Font _GameFont;
    private int _MainMenuOption;
    private int _ShipSelection;
    private int _Lockout;
    private LinkedList<Bitmap> _ShipsBMP;
    private SplashKitSDK.Triangle _tri;

    private List<AstGameObj> _GameObjs;      // sprite object list

    
    int X_GameText;               // given the window should be fixed, this should be only done once.
    int Y_GameText;


    public enum MenuOption
    {
        MainMenu,
        Player1ShipSelection,
        Player2ShipSelection,

    }

    public Menu(Window gameWindow)
    {
        _gameWindow = gameWindow;
        _GameFont = new Font("pricedown_bl", "pricedown_bl.otf");

        X_GameText = _gameWindow.Width / 2 - 125;               // given the window should be fixed, this should be only done once.
        Y_GameText = _gameWindow.Height / 6;

        SplashKit.CreateSpritePack("Menu");

        ReSetup();

        _tri = new Triangle();                              // main menu selection triangle
        
    }

    private void initRocks()
    {
        Json j = SplashKit.JsonFromFile("Enemy_Rock_Large.json");
        _GameObjs = new List<AstGameObj>();
        //SplashKit.CreateSpritePack(j.ReadString("pack"));
        //SplashKit.SelectSpritePack(j.ReadString("pack"));

        Vector2D vect = new Vector2D();
        vect.X = 1;
        vect.Y = 1;

        AstGameObj aso;
        const int numRocks = 5;
        for (int i = 0; i < numRocks; i++)
        {
            aso = new AstGameObj(j);

            initMove(aso);

            aso._sprite.CollisionKind = CollisionTestKind.PixelCollisions;
            
            _GameObjs.Add(aso);
        }

    }

    private void initMove(AstGameObj aso)
    {
        const double VELRANGE = 10, VELMIN = 5;
        const float ROTRANGE = 10, ROTMIN = 5;    
        int randStart = SplashKit.Rnd(0,4);         // 1 - 3 range
        float randPercS = SplashKit.Rnd();
        Point2D start = new Point2D(), end = SplashKit.RandomWindowPoint(_gameWindow);

        int sprWidth = aso._sprite.Width, sprHeight = aso._sprite.Height;

       
        if (randStart < 2) // of four values, initialize a side to start on.
        {
            start.X = randStart == 0 ? -sprWidth : _gameWindow.Width + sprWidth;        // left or right to start
            start.Y = randPercS * (_gameWindow.Height + sprHeight*2) - sprHeight;         // randomize height
        }
        else
        {
            start.Y = randStart == 2 ? -sprHeight : _gameWindow.Height + sprHeight;       // top or bottom to start
            start.X = randPercS * (_gameWindow.Width + sprWidth*2) - sprWidth;          // randomize width
        }

        //Console.Out.WriteLine(String.Format("R:{0}  ({1},{2})",randInt,start.X,start.Y));

        aso.rotSpeed = ROTRANGE * SplashKit.Rnd() - ROTMIN;     // value can be negative

        aso._sprite.Position = start;           // set start point
        Vector2D vect = SplashKit.UnitVector(SplashKit.VectorPointToPoint(start,end));      // get unit vector of path
        vect = SplashKit.VectorMultiply(vect,VELRANGE * SplashKit.Rnd() + VELMIN);      //  randomize speed
        aso.setVelocity(vect);                    // set velocity

    }

    private void runSprite()
    {
        
        //SplashKit.SelectSpritePack("Enemy");
        SplashKit.DrawAllSprites();
        SplashKit.UpdateAllSprites();

        const int FRAMESBEFORERESET = 60;
        for (int i = 0; i < _GameObjs.Count; i++)
        {
            AstGameObj tempASO = _GameObjs[i];
            tempASO.updateAngle();
            
            if (tempASO._sprite.Offscreen())
            {
                int framesOff = tempASO._values.ReadInteger("frames_offscreen") + 1;
                
                if (framesOff > FRAMESBEFORERESET)
                {   
                    initMove(tempASO);
                    framesOff = 0;
                }

                tempASO._values.AddNumber("frames_offscreen",framesOff);      // ADD METHOD IS ACTUALLY SET, OVERWRITES VALUE
            }
            else
                tempASO._values.AddNumber("frames_offscreen",0);

        }

        checkCol();
    
        //Console.Out.WriteLine(String.Format("{0},{1} : {2},{3}",temp._sprite.X,temp._sprite.Y,temp._sprite.Dx,temp._sprite.Dy));

    }

    private void checkCol()
    {

        for (int i = 0; i < _GameObjs.Count - 1; i++)
        {
            for (int j = i + 1; j < _GameObjs.Count; j++)
            {
                if (SplashKit.SpriteCollision(_GameObjs[i]._sprite,_GameObjs[j]._sprite))
                {
                    //Console.Out.WriteLine("COL");
                }
            }
        }

    }

    private LinkedList<Bitmap> retrieveShipsJSON()
    {
        LinkedList<Bitmap> tempShips = new LinkedList<Bitmap>();
        Json jsonShips = SplashKit.JsonFromFile("Playerships.json");
        List<String> listShips = new List<string>();
        SplashKit.JsonReadArray(jsonShips, "Ships", ref listShips);
        
        for (int i = 0; i < listShips.Count(); i++)
        {
            tempShips.AddLast(SplashKit.LoadBitmap("Ship" + (i+1).ToString(), listShips[i]));
        }

        return tempShips;
    }

    public void ReSetup()
    {
            GameStarted = false;
            _MainMenuOption = 0;
            _Menu = MenuOption.MainMenu;
            players = 0;
            _Lockout = -5;
            _ShipSelection = 0;

            _ShipsBMP = retrieveShipsJSON();

            if (SplashKit.HasBitmap("Player 1"))
            {
                SplashKit.FreeBitmap(SplashKit.BitmapNamed("Player 1"));
            }

            if (SplashKit.HasBitmap("Player 2"))
            {
                SplashKit.FreeBitmap(SplashKit.BitmapNamed("Player 2"));
            }

            _titleColor = Color.White;

            SplashKit.SelectSpritePack("Menu");

            initRocks();

    }

    public void DrawMenu()
    {
        switch (_Menu)
        {
            case MenuOption.MainMenu:
                DrawMainMenu();
                break;
            case MenuOption.Player1ShipSelection:
                DrawShipSelection("1","Left Arrow","Right Arrow","Enter to Select");
                
                break;
            case MenuOption.Player2ShipSelection:
                //if (_Lockout == _ShipSelection) _ShipSelection++;
                DrawShipSelection("2","A to Move Left","D to Move Right","Space to Select");
                break;
        }
    }

    private void DrawMainMenu()
    {

        const int Offset_AddY = 300;
        const int Offset_GameText = 80;
        const int FontSize = 80;

        runSprite();

        DrawMainMenuTitle();

        SplashKit.DrawTextOnWindow(_gameWindow, "1  Player", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_AddY);
        SplashKit.DrawTextOnWindow(_gameWindow, "2 Player", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_AddY + Offset_GameText);
        SplashKit.DrawTextOnWindow(_gameWindow, "Quit", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_AddY + Offset_GameText * 2);

         
        double Y_triangle = Y_GameText + Offset_GameText * _MainMenuOption + Offset_AddY;
        SplashKit.FillTriangle(Color.White, X_GameText - 40, Y_triangle + 35, X_GameText - 20, Y_triangle + 55, X_GameText - 40, Y_triangle + 75);

        
    }


    private Color _titleColor;
    private int _titleColorDelta;
    private const int _titleColorDeltaRate = 3;
    private void DrawMainMenuTitle()
    {  
        
        const int FontSize = 200;
        int titleX = (int)(_gameWindow.Width / 2 - (SplashKit.TextWidth("Asteroids",_GameFont,FontSize)/2));
        const int titleY = 40;

        if (_titleColor.R >= 1)
        {
            _titleColorDelta = -_titleColorDeltaRate;
        }
        else if (_titleColor.R <= .1)
        {
            _titleColorDelta = _titleColorDeltaRate;
        }

        int tempColorVal = (int)(_titleColor.R * 255 + _titleColorDelta);
        _titleColor =  SplashKit.RGBColor(tempColorVal,tempColorVal,tempColorVal);
        

        SplashKit.DrawTextOnWindow(_gameWindow, "ASTEROIDS", _titleColor, _GameFont, FontSize, titleX, titleY);

        //String tempRGB = String.Format("{0},{1},{2},{3}   {4}",_titleColor.R*255,_titleColor.G*255,_titleColor.B*255,_titleColor.A*255,_titleColorDelta); 
       //SplashKit.DrawTextOnWindow(_gameWindow, tempRGB, Color.White, _GameFont, 40, 30, 400);

    }

    private void DrawShipSelection(string Player,string MoveLeft, string MoveRight,string Select)
    {
        
        int Y_Ships = (_gameWindow.Height - _ShipsBMP.First().Height) / 2;
        int X_Ships = (_gameWindow.Width - _ShipsBMP.First().Width) / (_ShipsBMP.Count + 1);
        const int FontSize = 60;
        
        for (int i = 0; i < _ShipsBMP.Count(); i++)
        {
            Bitmap temp = _ShipsBMP.ElementAt(i);

            temp.Draw(X_Ships * (i+1), Y_Ships);

            // draw rectangle around ship currently selected
            if (_ShipSelection == i)
            {
                SplashKit.DrawRectangle(Color.White, SplashKit.BitmapBoundingRectangle(temp, X_Ships * (i+1), Y_Ships));
            }

            // use if original lockout 
            // if (_Lockout == i)
            // {
            //     SplashKit.DrawRectangle(Color.Red, SplashKit.BitmapBoundingRectangle(temp, X_Ships * (i+1), Y_Ships));
            //     temp.DrawLine(Color.Red, 0, 0, temp.Width, temp.Height);
            //     temp.DrawLine(Color.Red, temp.Width, 0, 0, temp.Height);
            // }
        }

        SplashKit.DrawTextOnWindow(_gameWindow, $"Player {Player} Select Your Ship", Color.White, _GameFont, FontSize,75,200);
        SplashKit.DrawTextOnWindow(_gameWindow, "Controls", Color.White, _GameFont, 40,75,550);
        SplashKit.DrawTextOnWindow(_gameWindow, MoveLeft, Color.White, _GameFont, 30,75,600);
        SplashKit.DrawTextOnWindow(_gameWindow, MoveRight, Color.White, _GameFont, 30,75,640);
        SplashKit.DrawTextOnWindow(_gameWindow, Select, Color.White, _GameFont, 30,75,680);

    }

    public void Selection()
    {
        
        switch (_Menu)
        {
            case MenuOption.MainMenu:
                HandleInputMainMenu();
                break;
            case MenuOption.Player1ShipSelection:
                HandleInputPlayer1Selection();
                break;
            case MenuOption.Player2ShipSelection:
                HandleInputPlayer2Selection();
                break;
        }
    }

    private void HandleInputMainMenu()
    {
        if (SplashKit.KeyTyped(KeyCode.UpKey))
        {
            _MainMenuOption = _MainMenuOption <= 0 ? 2 : _MainMenuOption - 1;
        }
        else if (SplashKit.KeyTyped(KeyCode.DownKey))
        {
            _MainMenuOption = _MainMenuOption >= 2 ? 0 : _MainMenuOption + 1;
        }
        else if (SplashKit.KeyTyped(KeyCode.ReturnKey))
        {
            switch (_MainMenuOption)
            {
                case 0:
                    _Menu = MenuOption.Player1ShipSelection;
                    players = 1;
                    break;

                case 1:
                    _Menu = MenuOption.Player1ShipSelection;
                    players = 2;
                    break;
                case 2:
                    quit = true;
                    break;
            }
        }
    }

    
    private int LockOutCheck(int selection, int change)
    {
        int newSelection = selection + change;
        if (newSelection == _Lockout)
        {
            if (newSelection == 1) return 4;
            else if (newSelection == 4) return 1;
            else return selection + change * 2;
        }
        else
        {
            if (newSelection < 1)
                if (_Lockout == 4) return 3;
                else return 4;
            else if (newSelection > 4)
                if (_Lockout == 1) return 2;
                else return 1;
            else return newSelection;
        }
    }
    

    // does not check for lock out after out of array bounds check
    private int indexCheck(int selection, int change)
    {
        int newSel = selection + change;                           // changed value
        //newSel = _Lockout == newSel ? newSel + change : newSel;     // check if value overlaps with lock out selection, push further
        if (newSel < 0)     newSel = _ShipsBMP.Count()-1;             // if value below min
        else if (newSel > _ShipsBMP.Count() - 1) newSel = 0;            // if value above max

        return newSel;
    }


    private void HandleInputPlayer1Selection()
    {
        if (SplashKit.KeyTyped(KeyCode.LeftKey))
        {
            _ShipSelection = indexCheck(_ShipSelection,-1);
        }
        else if (SplashKit.KeyTyped(KeyCode.RightKey))
        {
            _ShipSelection = indexCheck(_ShipSelection,1);
        }
        else if (SplashKit.KeyTyped(KeyCode.ReturnKey))
        {
            p1Ship = _ShipsBMP.ElementAt(_ShipSelection).Filename;
            //_Lockout = _ShipSelection;

            _ShipsBMP.Remove(_ShipsBMP.ElementAt(_ShipSelection));          

            if (players == 1)
            {
                GameStarted = true;
            }
            else if (players == 2)
            {
                _Menu = MenuOption.Player2ShipSelection;
                _ShipSelection = 0;
            }

        }
        else if (SplashKit.KeyTyped(KeyCode.BackspaceKey)) // New key needed to back out of ship selection
        {
            ReSetup();
        }

    }

    private void HandleInputPlayer2Selection()
    {
        if (SplashKit.KeyTyped(KeyCode.AKey))
        {
            _ShipSelection = indexCheck(_ShipSelection,-1);
        }
        else if (SplashKit.KeyTyped(KeyCode.DKey))
        {
            _ShipSelection = indexCheck(_ShipSelection,1);
        }
        else if (SplashKit.KeyTyped(KeyCode.SpaceKey))
        {
            p2Ship = _ShipsBMP.ElementAt(_ShipSelection).Filename;
            GameStarted = true;
        }
        else if (SplashKit.KeyTyped(KeyCode.BackspaceKey)) // New key needed to back out of ship selection
        {
            ReSetup();
        }

    }

}
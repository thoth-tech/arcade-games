using System;
using SplashKitSDK;

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
    private KeyCallback kcbP1;

    
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

        ReSetup();

        _tri = new Triangle();                              // main menu selection triangle
        

        testSpr = testSprite();
        testSpr.X = 200;
        testSpr.Y = 200;

        sprHand = new SpriteEventHandler(testFunction);     // test delegate function
        testSpr.CallOnEvent(sprHand);
        //SplashKit.SpriteCallOnEvent(testSpr,sprHand);       // Sprite does not fire event, does not work

        kcbP1 = new KeyCallback(HandleInputP1);
        SplashKit.RegisterCallbackOnKeyTyped(kcbP1);        // key callback works
       
    }

    Sprite testSpr;
    private Sprite testSprite()
    {
        Bitmap b = SplashKit.LoadBitmap("testRock", "RockLarge.png");
        b.SetCellDetails(200,200,3,3,9);
        AnimationScript scr = SplashKit.LoadAnimationScript("testRock", "rock.txt");

        Sprite spr = SplashKit.CreateSprite(b, scr);

        spr.StartAnimation("normal");        

        return spr;

    }

    private void runSprite()
    {
        
        testSpr.Draw();

        
        testSpr.Update();

        // String temp = String.Format("{0},{1},{2},{3}   {4}",testSpr.AnimationName(),0,0,0,0); 
       //SplashKit.DrawTextOnWindow(_gameWindow, temp, Color.White, _GameFont, 40, 30, 400);

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

    private void ReSetup()
    {
            GameStarted = false;
            _MainMenuOption = 0;
            _Menu = MenuOption.MainMenu;
            players = 0;
            _Lockout = -5;
            _ShipSelection = 0;

            _ShipsBMP = retrieveShipsJSON();

            _titleColor = Color.White;

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
        _gameWindow.Clear(Color.Black);

        const int Offset_AddY = 300;
        const int Offset_GameText = 80;
        const int FontSize = 80;

        DrawMainMenuTitle();

        SplashKit.DrawTextOnWindow(_gameWindow, "1  Player", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_AddY);
        SplashKit.DrawTextOnWindow(_gameWindow, "2 Player", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_AddY + Offset_GameText);
        SplashKit.DrawTextOnWindow(_gameWindow, "Quit", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_AddY + Offset_GameText * 2);

         
        double Y_triangle = Y_GameText + Offset_GameText * _MainMenuOption + Offset_AddY;
        SplashKit.FillTriangle(Color.White, X_GameText - 40, Y_triangle + 35, X_GameText - 20, Y_triangle + 55, X_GameText - 40, Y_triangle + 75);

        runSprite();
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
        _gameWindow.Clear(Color.Black);
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


    private void testFunction(IntPtr s, int e)
    {
        Console.Out.Write("BAD");
    }

    private void HandleInputP1(int key)
    {
        Console.Out.WriteLine("TYPED: " + key);

        switch ((KeyCode)key)
        {
            case KeyCode.LeftKey:
            break;
            case KeyCode.RightKey:
            break;
            case KeyCode.UpKey:
            break;
            case KeyCode.DownKey:
            break;

            case KeyCode.ReturnKey:
            break;
        }
    }

    private void InputLeftKey()
    {
        if (_Menu == MenuOption.Player1ShipSelection || _Menu == MenuOption.Player2ShipSelection)
        {
            _ShipSelection = indexCheck(_ShipSelection,-1);
        }
    }

    SpriteEventHandler sprHand;
    private void HandleInputMainMenu()
    {
        if (SplashKit.KeyTyped(KeyCode.UpKey))
        {
            _MainMenuOption = _MainMenuOption <= 0 ? 2 : _MainMenuOption - 1;
            testSpr.StartAnimation("explode");
            //testSpr.CallOnEvent();
        }
        else if (SplashKit.KeyTyped(KeyCode.DownKey))
        {
            _MainMenuOption = _MainMenuOption >= 2 ? 0 : _MainMenuOption + 1;
            testSpr.StartAnimation("normal");
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
        else if (SplashKit.KeyTyped(KeyCode.EscapeKey)) // New key needed to back out of ship selection
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
        else if (SplashKit.KeyTyped(KeyCode.EscapeKey)) // New key needed to back out of ship selection
        {
            ReSetup();
        }

    }

}
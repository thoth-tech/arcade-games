using System;
using SplashKitSDK;

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
    private Bitmap _Ship1, _Ship2, _Ship3, _Ship4;


    public enum MenuOption
    {
        MainMenu,
        Player1ShipSelection,
        Player2ShipSelection,

    }

    public Menu(Window gameWindow)
    {
        _gameWindow = gameWindow;
        GameStarted = false;
        _GameFont = new Font("pricedown_bl", "pricedown_bl.otf");
        _MainMenuOption = 1;
        _Menu = MenuOption.MainMenu;
        _ShipSelection = 1;
        players = 0;
        _Lockout = -5;
        _Ship1 = SplashKit.LoadBitmap("Ship1", "BlueShip.png");
        _Ship2 = SplashKit.LoadBitmap("Ship2", "SpaceShip.png");
        _Ship3 = SplashKit.LoadBitmap("Ship3", "RedShip.png");
        _Ship4 = SplashKit.LoadBitmap("Ship4", "FighterShip.png");
    }

    private void ReSetup()
    {
            _Menu = MenuOption.MainMenu;
            players = 0;
            _Lockout = -5;
            _Ship1.Free();
            _Ship2.Free();
            _Ship3.Free();
            _Ship4.Free();
            _Ship1 = SplashKit.LoadBitmap("Ship1", "BlueShip.png");
            _Ship2 = SplashKit.LoadBitmap("Ship2", "SpaceShip.png");
            _Ship3 = SplashKit.LoadBitmap("Ship3", "RedShip.png");
            _Ship4 = SplashKit.LoadBitmap("Ship4", "FighterShip.png");
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
                if (_Lockout == _ShipSelection) _ShipSelection += 1;
                DrawShipSelection("2","A to Move Left","D to Move Right","Space to Select");
                break;
        }
    }

    private void DrawMainMenu()
    {
        _gameWindow.Clear(Color.Black);
        int X_GameText = _gameWindow.Width / 2 - 125;
        int Y_GameText = _gameWindow.Height / 6;
        const int Offset_GameText = 80;
        const int FontSize = 80;

        SplashKit.DrawTextOnWindow(_gameWindow, "1  Player", Color.White, _GameFont, FontSize, X_GameText, Y_GameText);
        SplashKit.DrawTextOnWindow(_gameWindow, "2 Player", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_GameText);
        SplashKit.DrawTextOnWindow(_gameWindow, "Quit", Color.White, _GameFont, FontSize, X_GameText, Y_GameText + Offset_GameText * 2);

        if (_MainMenuOption == 1)
        {
            SplashKit.FillTriangle(Color.White, X_GameText - 40, Y_GameText + 33, X_GameText - 20, Y_GameText + 53, X_GameText - 40, Y_GameText + 73);
        }
        else if (_MainMenuOption == 2)
        {
            SplashKit.FillTriangle(Color.White, X_GameText - 40, Y_GameText + Offset_GameText + 35, X_GameText - 20, Y_GameText + Offset_GameText + 55, X_GameText - 40, Y_GameText + Offset_GameText + 75);
        }
        else if (_MainMenuOption == 3)
        {
            SplashKit.FillTriangle(Color.White, X_GameText - 40, Y_GameText + Offset_GameText * 2 + 35, X_GameText - 20, Y_GameText + Offset_GameText * 2 + 55, X_GameText - 40, Y_GameText + Offset_GameText * 2 + 75);
        }

    }

    private void DrawShipSelection(string Player,string MoveLeft, string MoveRight,string Select)
    {
        _gameWindow.Clear(Color.Black);
        int Y_Ships = (_gameWindow.Height - _Ship1.Height) / 2;
        int X_Ships = (_gameWindow.Width - _Ship1.Width) / 5;
        int X_Ship1 = X_Ships * 1, X_Ship2 = X_Ships * 2, X_Ship3 = X_Ships * 3, X_Ship4 = X_Ships * 4;
        int Y_Ship1 = Y_Ships, Y_Ship2 = Y_Ships, Y_Ship3 = Y_Ships, Y_Ship4 = Y_Ships;
        const int FontSize = 60;

        _Ship1.Draw(X_Ship1, Y_Ship1);
        _Ship2.Draw(X_Ship2, Y_Ship2);
        _Ship3.Draw(X_Ship3, Y_Ship3);
        _Ship4.Draw(X_Ship4, Y_Ship4);

        SplashKit.DrawTextOnWindow(_gameWindow, $"Player {Player} Select Your Ship", Color.White, _GameFont, FontSize,75,200);
        SplashKit.DrawTextOnWindow(_gameWindow, "Controls", Color.White, _GameFont, 40,75,550);
        SplashKit.DrawTextOnWindow(_gameWindow, MoveLeft, Color.White, _GameFont, 30,75,600);
        SplashKit.DrawTextOnWindow(_gameWindow, MoveRight, Color.White, _GameFont, 30,75,640);
        SplashKit.DrawTextOnWindow(_gameWindow, Select, Color.White, _GameFont, 30,75,680);

        switch (_ShipSelection)
        {
            case 1:
                SplashKit.DrawRectangle(Color.White, SplashKit.BitmapBoundingRectangle(_Ship1, X_Ship1, Y_Ship1));
                break;
            case 2:
                SplashKit.DrawRectangle(Color.White, SplashKit.BitmapBoundingRectangle(_Ship2, X_Ship2, Y_Ship2));
                break; 
            case 3:
                SplashKit.DrawRectangle(Color.White, SplashKit.BitmapBoundingRectangle(_Ship3, X_Ship3, Y_Ship3));
                break;
            case 4:
                SplashKit.DrawRectangle(Color.White, SplashKit.BitmapBoundingRectangle(_Ship4, X_Ship4, Y_Ship4));
                break;
        }

        switch (_Lockout)
        {
            case 1:
                SplashKit.DrawRectangle(Color.Red, SplashKit.BitmapBoundingRectangle(_Ship1, X_Ship1, Y_Ship1));
                _Ship1.DrawLine(Color.Red, 0, 0, _Ship1.Width, _Ship1.Height);
                _Ship1.DrawLine(Color.Red, _Ship1.Width, 0, 0, _Ship1.Height);
                break;
            case 2:
                SplashKit.DrawRectangle(Color.Red, SplashKit.BitmapBoundingRectangle(_Ship2, X_Ship2, Y_Ship2));
                _Ship2.DrawLine(Color.Red, 0, 0, _Ship1.Width, _Ship1.Height);
                _Ship2.DrawLine(Color.Red, _Ship1.Width, 0, 0, _Ship1.Height);
                break;
            case 3:
                SplashKit.DrawRectangle(Color.Red, SplashKit.BitmapBoundingRectangle(_Ship3, X_Ship3, Y_Ship3));
                _Ship3.DrawLine(Color.Red, 0, 0, _Ship1.Width, _Ship1.Height);
                _Ship3.DrawLine(Color.Red, _Ship1.Width, 0, 0, _Ship1.Height);
                break;
            case 4:
                SplashKit.DrawRectangle(Color.Red, SplashKit.BitmapBoundingRectangle(_Ship4, X_Ship4, Y_Ship4));
                _Ship4.DrawLine(Color.Red, 0, 0, _Ship1.Width, _Ship1.Height);
                _Ship4.DrawLine(Color.Red, _Ship1.Width, 0, 0, _Ship1.Height);
                break;
        }

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
            if (_MainMenuOption == 1) { _MainMenuOption = 3; }
            else { _MainMenuOption -= 1; }
        }
        else if (SplashKit.KeyTyped(KeyCode.DownKey))
        {
            if (_MainMenuOption == 3) { _MainMenuOption = 1; }
            else { _MainMenuOption += 1; }
        }
        else if (SplashKit.KeyTyped(KeyCode.ReturnKey))
        {
            switch (_MainMenuOption)
            {
                case 1:
                    _Menu = MenuOption.Player1ShipSelection;
                    players = 1;
                    break;

                case 2:
                    _Menu = MenuOption.Player1ShipSelection;
                    players = 2;
                    break;
                case 3:
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


    private void HandleInputPlayer1Selection()
    {
        if (SplashKit.KeyTyped(KeyCode.LeftKey))
        {
            _ShipSelection = LockOutCheck(_ShipSelection,-1);
        }
        else if (SplashKit.KeyTyped(KeyCode.RightKey))
        {
            _ShipSelection = LockOutCheck(_ShipSelection,1);
        }
        else if (SplashKit.KeyTyped(KeyCode.ReturnKey))
        {
            switch (_ShipSelection)
            {
                case 1:
                    p1Ship = _Ship1.Filename;
                    _Lockout = 1;
                    break;
                case 2:
                    p1Ship = _Ship2.Filename;
                    _Lockout = 2;
                    break;
                case 3:
                    p1Ship = _Ship3.Filename;
                    _Lockout = 3;
                    break;
                case 4:
                    p1Ship = _Ship4.Filename;
                    _Lockout = 4;
                    break;
            }

            if (players == 1)
            {
                GameStarted = true;
            }
            else if (players == 2)
            {
                _Menu = MenuOption.Player2ShipSelection;
                _ShipSelection = 1;
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
            _ShipSelection = LockOutCheck(_ShipSelection,-1);
        }
        else if (SplashKit.KeyTyped(KeyCode.DKey))
        {
            _ShipSelection = LockOutCheck(_ShipSelection,1);
        }
        else if (SplashKit.KeyTyped(KeyCode.SpaceKey))
        {
            switch (_ShipSelection)
            {
                case 1:
                    p2Ship = _Ship1.Filename;
                    break;
                case 2:
                    p2Ship = _Ship2.Filename;
                    break;
                case 3:
                    p2Ship = _Ship3.Filename;
                    break;
                case 4:
                    p2Ship = _Ship4.Filename;
                    break;
            }

            GameStarted = true;
        }
        else if (SplashKit.KeyTyped(KeyCode.EscapeKey)) // New key needed to back out of ship selection
        {
            ReSetup();
        }

    }

}
using System;
using SplashKitSDK;

namespace _2dRacerDemo
{
    public class Program{
        public static void Main(){
            Window gameWindow = new Window("2D Racing", 1600, 900);
           // DrawDemo DrawDemo = new DrawDemo(gameWindow);
            Menu Menu = new Menu(gameWindow);
            Game game;
            bool GameExit = false;
            int index = 0;
            bool started = false;
            int count = 0;

            while (!GameExit)
            {
                gameWindow.Clear(Color.Black);
                SplashKit.ProcessEvents();

                if (SplashKit.KeyTyped(KeyCode.EscapeKey)) GameExit = true;
                if (gameWindow.CloseRequested) GameExit = true;

                if(! started){
                    count = Menu.getselection(count);
                    started = true;
                }
                else{
                    switch(count){
                        case 2:
                            GameExit = true;
                            break;
                        default:
                            game = new Game(gameWindow,count);
                            game.startGame();
                            GameExit = true;
                            break;
                    }
                }
                gameWindow.Refresh(60);         // draw frame to window
            }
        }

    }
}



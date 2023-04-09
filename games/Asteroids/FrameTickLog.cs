using System;
using SplashKitSDK;

public class FrameTickLog
{
    private Window _gameWindow;
    private Font _font;
    private const int _FONTSIZE = 20;
    private int _windowWidth, _windowHeight;
    private int _textHeight;


    private SplashKitSDK.Timer frameTimer = SplashKit.CreateTimer("frameTimer");    // timer from splashkit
    private int frameTicksNum;                                                 // number of elements for frame ticks to retain
    private List<uint> frameTicks;                  // list contains ticks needed to complete a frame after refresh
    private int listElementToReplace;                                           // this value is for frameticks to replace the indexed value, rather than create new objects in a linked list, remove first, add last
    private long frameTicksSum;                                                 // Total frame ticks value in list

    public uint currentLongestFrame {get; private set;}
    private int currentLongestFrameDuration;
    


    // general use form
    // 1: initialise object
    // 2: start timer
    // 3: reset start of frame loop
    // 4: update end of frame loop
    // 5: get average and draw to window

    // pass window and font if you want to directly call its draw function, otherwise get values and draw externally
    public FrameTickLog(Window gameWindow, string font, int framesToCount = 10)
    {
        _gameWindow = gameWindow;
        LoadFont(font);
        if (_gameWindow != null)
        {
            _windowWidth = _gameWindow.Width;
            _windowHeight = _gameWindow.Height;
        }
        _textHeight = SplashKit.TextHeight("1",_font,_FONTSIZE);

        frameTicksNum = framesToCount;
        Reinitialise();
    }

    private void LoadFont(string font)
    {
        
        if (SplashKit.HasFont(font))
        {
            _font = SplashKit.FontNamed(font);
        }
        else 
        {
            _font = SplashKit.LoadFont("pricedown_bl","pricedown_bl.otf");
        }
    }

    public void Reinitialise()
    {
        frameTimer.Stop();          // stop timer
        frameTimer.Reset();         // reset timer

        // create new list, if not existing, else clear values
        if (frameTicks == null)
            frameTicks = new List<uint>(new uint[frameTicksNum]);
        else
            frameTicks.Clear();

        // reset index value and tick sum
        listElementToReplace = 0;
        frameTicksSum = 0;

        currentLongestFrame = 0;
        currentLongestFrameDuration = 0;
    }

    public void Start()
    {
        frameTimer.Start();
    }

    public void Stop()
    {
        frameTimer.Stop();
    }

    public void Reset()
    {
        frameTimer.Reset();
    }

    public void update()
    {
        // update current index, reduce sum before replace, add sum after replace
        frameTicksSum -= frameTicks[listElementToReplace];
        frameTicks[listElementToReplace] = frameTimer.Ticks;
        frameTicksSum += frameTicks[listElementToReplace];

        // check for longest frame
        UpdateLongestFrame();

        // increment index or wrap to 0 if out of bounds
        listElementToReplace = ++listElementToReplace >= frameTicksNum ? 0 : listElementToReplace;

        // maybe implement own reset line, comment out if calling reset seperately from here
        frameTimer.Reset();
    }

    // handles checking if new frame ticks is higher than current highest
    private void UpdateLongestFrame()
    {
        // is new count higher?
        if (frameTicks[listElementToReplace] > currentLongestFrame)
        {
            currentLongestFrame = frameTicks[listElementToReplace];
            currentLongestFrameDuration = 0;
        }
        else
        {
            // remains for another frame
            currentLongestFrameDuration++;
            // has it remained for longer than retained in list?
            if (currentLongestFrameDuration >= frameTicksNum)
            {   // get next highest frame value
                currentLongestFrame = GetLongestFrameInList();
                currentLongestFrameDuration = 0;
            }
        }
    }

    // simple iterator to get largest value, as list is not ordered by value, ordered by First In, First Out
    public uint GetLongestFrameInList()
    {
        // get largest frame
        uint tempFT = 0;
        foreach (uint i in frameTicks)
        {
            if (tempFT < i)
                tempFT = i;
        }

        return tempFT;
    }

    // tick sum divided by number of frames counted
    public long ReadMean()
    {
        return frameTicksSum / frameTicksNum;
    }

    // int type may be implemented to change where to draw counter
    public void draw()
    {
        string[] displayStrings = {
                                    ReadMean().ToString(), 
                                    currentLongestFrame.ToString()
                                };

        for (int i = 0, width; i < displayStrings.Count(); i++)
        {
            width = SplashKit.TextWidth(displayStrings[i], _font, _FONTSIZE);
            //Console.WriteLine(displayStrings[i]);
            SplashKit.DrawTextOnWindow(_gameWindow,displayStrings[i],Color.White, _font, _FONTSIZE, _windowWidth - width, _windowHeight - _textHeight * (i + 1));
        }

    }

}
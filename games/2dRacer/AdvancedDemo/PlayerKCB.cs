using System;
using SplashKitSDK;

// mostly a key callback infrastructure and demonstration
// keycallbacks are used to find the value in a dictionary pertaining to their key
// values are actions, which are function calls

public class PlayerKCB : GameObj
{
    private Vector2D accelVect; // vector for controlled acceleration
    private Vector2D forceVect; // vector for uncontrolled movement

    // function calls for key events, seperated by event type
    private Dictionary<String,Action> inputKeysDown;   // keys to check for, on down event (Continuous)
    private Dictionary<String,Action> inputKeysTyped;   // keys to check for, on typed event (Once, on press)
    private Dictionary<String,Action> inputKeysUp;   // keys to check for, on up event (Once, on unpress)
    private KeyCallback kcbTyped, kcbUp, kcbDown;

    public PlayerKCB(Json jsonInfo, Json inputKeys) : base(jsonInfo)
    {

        accelVect = new Vector2D();
        forceVect = new Vector2D();

        setupInputKeys(inputKeys);
        kcbTyped = new KeyCallback(onTyped);
        kcbUp = new KeyCallback(onUp);
        kcbDown = new KeyCallback(onDown);
    }

    public void update()
    {
        //obj._sprite.AddToVelocity();
    }

    // externally added vector
    public void addForceVector(Vector2D vect)
    {
        SplashKit.VectorAdd(forceVect,vect);
        // add magnitude limitation if desired, to avoid excessive values
    }

    // remove callback, then remove sprite
    public override void Dispose()
    {
        stopCallback();
        base.Dispose();
    }
    
    // ---------- INPUT CALLS

    // {} if you want to run anonymous functions,

    private void setupInputKeys(Json inputKeys)
    {
        // repeating held keys
        inputKeysDown = new Dictionary<string, Action>{
            {inputKeys.ReadString("left"),  () => {}},
            {inputKeys.ReadString("right"), () => {}},
            {inputKeys.ReadString("up"),    () => {}},
            {inputKeys.ReadString("down"),  () => {}},
            {inputKeys.ReadString("btn1"),  () => {}},
            {inputKeys.ReadString("btn2"),  () => {}},
            {inputKeys.ReadString("btn3"),  () => {}},
            {inputKeys.ReadString("btn4"),  () => {}},
            {inputKeys.ReadString("btn5"),  () => {}},
            {inputKeys.ReadString("btn6"),  () => {}}
        };

        // releasing key
        inputKeysUp = new Dictionary<string, Action>{
            {inputKeys.ReadString("left"),  () => left(false)},
            {inputKeys.ReadString("right"), () => right(false)},
            {inputKeys.ReadString("up"),    () => {}},
            {inputKeys.ReadString("down"),  () => {}},
            {inputKeys.ReadString("btn1"),  () => {}},
            {inputKeys.ReadString("btn2"),  () => {}},
            {inputKeys.ReadString("btn3"),  () => {}},
            {inputKeys.ReadString("btn4"),  () => {}},
            {inputKeys.ReadString("btn5"),  () => {}},
            {inputKeys.ReadString("btn6"),  () => {}}
        };

        // first pressing of key
        inputKeysTyped = new Dictionary<string, Action>{
            {inputKeys.ReadString("left"),  () => left(true)},
            {inputKeys.ReadString("right"), () => right(true)},
            {inputKeys.ReadString("up"),    () => {}},
            {inputKeys.ReadString("down"),  () => {}},
            {inputKeys.ReadString("btn1"),  () => {}},
            {inputKeys.ReadString("btn2"),  () => {}},
            {inputKeys.ReadString("btn3"),  () => {}},
            {inputKeys.ReadString("btn4"),  () => {}},
            {inputKeys.ReadString("btn5"),  () => {}},
            {inputKeys.ReadString("btn6"),  () => {}}
        };
    }

    public void startCallback()
    {
        SplashKit.RegisterCallbackOnKeyTyped(kcbTyped);
        SplashKit.RegisterCallbackOnKeyUp(kcbUp);
        SplashKit.RegisterCallbackOnKeyDown(kcbDown);
    }

    public void stopCallback()
    {
        SplashKit.DeregisterCallbackOnKeyTyped(kcbTyped);
        SplashKit.DeregisterCallbackOnKeyUp(kcbUp);
        SplashKit.DeregisterCallbackOnKeyDown(kcbDown);
    }

    // key callback function on down event
    public void onTyped(int key)
    {
        Console.Out.Write("Typed: ");
        inputEvent(inputKeysTyped,key);
    }

    public void onUp(int key)
    {
        Console.Out.Write("Up: ");
        inputEvent(inputKeysUp,key); 
    }

    public void onDown(int key)
    {
        Console.Out.Write("Down: ");
        inputEvent(inputKeysDown,key);
    }
    
    // get action from dictionary and run it
    private void inputEvent(Dictionary<String,Action> actions, int key)
    {
        String k = SplashKit.KeyName((KeyCode)key);     // json stores string, there is no string to keycode conversion, or integer table to lookup
        Action act;
        if (actions.TryGetValue(k,out act))       // check if key exists in dictionary, if it does return true with value set to act
        {
            act();
            
        }

        Console.Out.WriteLine(k);
    }

    // player input left
    // this is intended to run with a repeating animation
    public void left(bool state)
    {
        if (state) 
        {
            sprite.StartAnimation("left");
        }
        else
        {
            if (sprite.AnimationName() == "left")
            {
                sprite.StartAnimation("straight");
            }
        }
    }

    // player input right
    public void right(bool state)
    {
        if (state)
        {
            sprite.StartAnimation("right");
        }
        else
        {
            if (sprite.AnimationName() == "right")
            {
                sprite.StartAnimation("straight");
            }
        }
    }


    // ---------------------------------------------- accessors, setters

    
}
using System;
using SplashKitSDK;

public class VehicleObstacle : GameObj
{
    public SpriteEventHandler SEH;  // ENSURE THIS IS STOPPED BEFORE DISPOSAL
    

    public VehicleObstacle(Json jsonInfo) : base(jsonInfo)
    {
        sprite.CollisionKind = CollisionTestKind.PixelCollisions;   // probably default already
        SEH = new SpriteEventHandler(SprEventHandler);
    }

    public void update()
    {
        int dx_c = jsonVal.ReadInteger("Dx_count");
        if (dx_c > 0)                           // remaining steps
        {
            jsonVal.AddNumber("Dx_count", dx_c - 1);
        }
        else if (dx_c == 0)             // stop when zero
        {
            sprite.Dx = 0;
        }

        
    }

    // function sets X position and Dy for sprite, Y is set to height of sprite 
    public void start(float X, float speed)
    {
        setX(X);
        sprite.Y = -sprite.Height;
        setSpeed(speed);
    }

    public bool isOffscreen()
    {   
        // if moving downwards, and is not above screen
        // or if moving upwards and is above screen
        if ((sprite.Dy <= 0 && !(sprite.Y > 0)) || (sprite.Dy > 0 && sprite.Y < -sprite.Height))
        {
            return false;
        }
        return true;
    }

    public void changeX(float newX, int steps)      // reach new X value by number of updates
    {

        jsonVal.AddNumber("Dx_count", steps);
        float deltaX = newX - X();
        sprite.Dx = deltaX;
    }

    public override void  Dispose()
    {
        StopEventCall();
        base.Dispose();
    }

    // ------------------ event

    // example event code for sprite
    private void SprEventHandler(IntPtr ptr, int ev)
    {
        if ((SpriteEventKind) ev == SpriteEventKind.SpriteTouchedEvent)
        {

        }
        else if ((SpriteEventKind) ev == SpriteEventKind.SpriteAnimationEndedEvent)
        {
            
        }
    }

    public void StartEventCall()
    {
        sprite.CallOnEvent(SEH);
    }

    public void StopEventCall()
    {
        sprite.StopCallingOnEvent(SEH);
    }

    // ------------------ accessors

    public float X()
    {
        return sprite.Dx;
    }

    public void setX(float X)
    {
        sprite.X = X;
    }

    public float Speed()
    {
        return sprite.Dy;
    }

    public void setSpeed(float speed)
    {
        sprite.Dy = speed;
    }

    public float Mass()
    {
        return sprite.Mass;
    }

}

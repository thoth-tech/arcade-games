using System;
using SplashKitSDK;

// static one sprite marking for a line, use if repeating pattern continually drawn
// currently does not have a handle if it should end
public class RoadMarking 
{
    public GameObj line { get; }
    private int length;
    private int spacing;
    private int segment;

    public RoadMarking(Json jsonInfo)
    {
        line = new GameObj(jsonInfo);
        length = line.jsonVal.ReadInteger("length");
        spacing = line.jsonVal.ReadInteger("spacing");
        segment =  length + spacing;
        
    }

    // update new segment position, change sprite position based on new offset
    // call every game loop
    public void update()
    {
        while (line.sprite.Y * 2 > segment)    // if moved by an entire segment, reset position up
            line.sprite.Y -= segment;
    }

    public void setSpeed(float newSpeed)
    {
        line.sprite.Dy = newSpeed;
    }

    // change x value, y should be 0
    public void setX(float newX)
    {
        line.sprite.X = newX;
    }

}
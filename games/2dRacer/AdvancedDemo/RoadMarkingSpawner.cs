using System;
using SplashKitSDK;

// dynamic marking spawner, able to shift position
public class RoadMarkingSpawner
{
    private Bitmap markingTemplate;
    private LinkedList<GameObj> marks; // linkedlist is used to make removing front elements easier
    private Json values;

    private float rot;
    /*  json values to exist in file
    speed       value tracks speed of level
    spacing     used as segment length (overall length between marking spawns)
    X           X pixel position of spawner
    Dx          change of X per update
    Dx_Count    remaining changes
    offset      vertical position tracker
    */

    // initialize then set x and speed
    public RoadMarkingSpawner(Json jsonInfo)
    {
        markingTemplate = SplashKit.LoadBitmap(jsonInfo.ReadString("name"),jsonInfo.ReadString("bmp_fp"));
        marks = new LinkedList<GameObj>();

        values = jsonInfo.ReadObject("values");
        rot = 0;

    }

    public void update()
    {
        float currentOffset = Offset() + Speed();   // update "position" of spawner
        float segment = values.ReadNumber("spacing");

        while (currentOffset >= segment)    // if speed is beyond segment length
        {
            currentOffset -= segment;

            spawnMarking(currentOffset);
        }

        if (Dx_count() > 0)     // spawner in process of moving
        {
            setX(X() + Dx());
            setDx_count(Dx_count()-1);

            if (Dx_count() == 0)    // stop moving
                setDx(0);
        }

        setOffset(currentOffset);

        removeOffscreen();      // dispose
    }

    public void changeX(float newX, int steps)
    {

        setDx_count(steps);
        float deltaX = newX - X();
        setDx(deltaX/steps);
    }

    public void spawnMarking(float offset)
    {
        GameObj go = new GameObj(markingTemplate);
        

        go.sprite.X = X();            // set horizontal position
        go.sprite.Y = -go.sprite.Height + offset;    // push offscreen completely, offset is used to maintain consistent spacing with different speeds
        go.sprite.Dy = Speed();       // set downward speed

        go.sprite.AnchorPoint = new Point2D {X = go.sprite.Width/2, Y = go.sprite.Height/2};  // for rotations

        go.rotateSprite(rot);   // keep original vector

        marks.AddLast(go);     // add to list
    }

    private void updateSpeed()
    {
        for (LinkedListNode<GameObj> s = marks.First; s != null; s = s.Next)     // iteration through list
        {
            s.Value.sprite.Dy = Speed();
        }
    }

    private void removeOffscreen()
    {
        LinkedListNode<GameObj> temp;        // temporary pointer
        for (LinkedListNode<GameObj> s = marks.First; s != null; )   // first in list, first to be offscreen
        {
            if (s.Value.sprite.Offscreen() && s.Value.sprite.Y > 0)   // check if off screen, and is not above screen
            {
                temp = s.Next;      // get next node before removal
                marks.Remove(s);    // remove current node
                s = temp;           // set iterator to next node
            }
            else
                break;          // if not offscreen, further nodes should not be offscreen
        }
    }

    // ---------------------------------------------------------------------- accessors, setters

    public float Speed()
    {
        return values.ReadNumber("speed");
    }

    // changing speed, needs to update every sprite existing
    public void setSpeed(float newSpeed)
    {
        values.AddNumber("speed",newSpeed);    // change value
        updateSpeed();                  // update all existing sprites to new speed
    }

    public float X()
    {
        return values.ReadNumber("X");
    }

    public void setX(float newX)
    {
        values.AddNumber("X",newX);
    }

    public float Dx()
    {
        return values.ReadNumber("Dx");
    }

    // changing Dx needs to update rotation for new markings
    private void setDx(float newDx)
    {
        values.AddNumber("Dx",newDx);
        rot = -(float)SplashKit.VectorAngle(new Vector2D(){X=newDx,Y = Speed()}) + 90;
    }

    public float Dx_count()
    {
        return values.ReadNumber("Dx_count");
    }

    private void setDx_count(float newDx_count)
    {
        values.AddNumber("Dx_count",newDx_count);
    }

    private float Offset()
    {
        return values.ReadNumber("offset");
    }

    private void setOffset(float newOffset)
    {
        values.AddNumber("offset", newOffset);
    }

}
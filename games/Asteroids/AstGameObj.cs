using System;
using SplashKitSDK;

// Sprite handler object
public class AstGameObj
{
    public Sprite _sprite { get; }
    
    public float rotSpeed {get; set;}

    private int _health { get; set; }        // integer health, could be decimal if needed
    private int _maxHP { get; } 

    /*
    JSON VALUES
    "bmp_FP"  filepath of bitmap
    "name"  general name of object, store name in splashkit
    "cell_details" 5 index array
    "anim_fp"       string filepath of script file
    "anchor_pos"    2 index array, XY position of anchor position
    "mass"      mass value of sprite
    "pack"      sprite pack to be assigned          // NOT USED CURRENTLY, "default" is the initial pack assigned at start
    "hp_max"     number value for max health        // NOT USED
    */

    /*
    public AstGameObj(String bmpName, Point2D pos, Vector2D dir, double rot)
    {
        _selfSprite = SplashKit.CreateSprite(SplashKit.BitmapNamed(bmpName));
        // other stuff
    }
    */

    //
    public AstGameObj(Json jsonInfo)
    {   
        // make sprite
        _sprite = constructSprite(jsonInfo);

    }

    // check if bitmap of path is loaded already as name.
    // if name is loaded, just use it
    // ideally you just give the name, the path should be pre-loaded by handler, or game instance, not this
    private Sprite constructSprite(Json jsonInfo)
    {   
        String name = jsonInfo.ReadString("name");          // get name
        String bmpFP = jsonInfo.ReadString("bmp_fp");       // get filepath
        Bitmap bmp;
        Sprite spr;
        Point2D tempPos = new Point2D();
        List<double> tempList = new List<double>();          // 5 index

        if (SplashKit.HasBitmap(name))              // exist already? load existing bitmap into sprite
        {
            bmp = SplashKit.BitmapNamed(name);
        }
        else
        {
            bmp = SplashKit.LoadBitmap(name,bmpFP);          // initialise bitmap

            if (jsonInfo.HasKey("cell_details"))            // has cell frames, does not validate if animations are used
            {
                jsonInfo.ReadArray("cell_details", ref tempList);                 // put array into cd
                //List<int> cdINT = cd.Select(x => (int)x).ToList();          // turn cd into ints
                bmp.SetCellDetails((int)tempList[0],(int)tempList[1],(int)tempList[2],(int)tempList[3],(int)tempList[4]);       // pass ints 
            }
        }

        // seperated feom cell if you want a offcenter anchor
        if (jsonInfo.HasKey("anchor_pos"))
        {
            tempList = new List<double>();
            jsonInfo.ReadArray("anchor_pos",ref tempList);
            tempPos.X = tempList[0];
            tempPos.Y = tempList[1];
        }


        if (jsonInfo.HasKey("anim_fp"))             // json has animation script file path
        {
            AnimationScript scr = SplashKit.LoadAnimationScript(name,jsonInfo.ReadString("anim_fp"));

            spr = SplashKit.CreateSprite(name,bmp,scr);
        }
        else                                        // no animation script file path
        {
            spr = SplashKit.CreateSprite(name,bmp);
        }

        if (jsonInfo.HasKey("mass"))
            spr.Mass = jsonInfo.ReadNumber("mass");

        spr.AnchorPoint = tempPos;

        return spr;
    }

    // call if want to update rotation value and fix velocity to heading value
    // i hate this roundabout way to sidestep the rotation effect
    public void updateAngle()
    {
        if (rotSpeed != 0)
        {
            // get angle prior to change
            double currVectAngle = SplashKit.VectorAngle(_sprite.Velocity) + _sprite.Rotation;
            
            _sprite.Rotation += rotSpeed;           // ADD ROTATION CHANGE

            // translate velocity to new angle
            _sprite.Velocity = SplashKit.VectorFromAngle(currVectAngle - _sprite.Rotation,_sprite.Speed);
        }
        
    }

    // function to handle rotation agnostic vector addition
    // resultant vector will be window aligned
    public void addVector(Vector2D vector)
    {
        // translate out of rotation angle
        double currVectAngle = SplashKit.VectorAngle(_sprite.Velocity) + _sprite.Rotation;
        // window relative vector
        Vector2D currVect = SplashKit.VectorFromAngle(currVectAngle,_sprite.Speed);
        Vector2D newVect = SplashKit.VectorAdd(currVect,vector);

        setVelocity(newVect);
    }

    // window relative vector, with respect to current rotation
    public void setVelocity(Vector2D vector)
    {
        double newVectAngle = SplashKit.VectorAngle(vector);
        double newVectMag = SplashKit.VectorMagnitude(vector);

        _sprite.Velocity = SplashKit.VectorFromAngle(newVectAngle - _sprite.Rotation, newVectMag);
    }

}
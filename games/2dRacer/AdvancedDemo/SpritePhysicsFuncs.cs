using System;
using SplashKitSDK;

// functions for 1-1 sprite collision physics
// collision resolution depends on kind of collision desired
// objects need to have non-forced velocities to retain the effect

// functions need to be checked for collision externally, these only handle the impact physics over a frame
public static class SpritePhysics
{
    // external collision check, 
    // using sprite rectangles
    public static void CollisionElasticRect(Sprite A, Sprite B)
    {
        Vector2D AVel = A.Velocity, BVel = B.Velocity;
        // Velocity is relative overall vector between the two, it will be larger if closing towards, smaller if similar trajectory
        Vector2D AB = SplashKit.VectorOutOfRectFromRect(A.CollisionRectangle,B.CollisionRectangle,SplashKit.VectorSubtract(AVel,BVel));

        // assuming both sprites have mass values, if 0 then it won't work
        float MassTotal = A.Mass + B.Mass;
        float APerc = A.Mass / MassTotal, BPerc = B.Mass / MassTotal;  

        // vectors to push both sprite rectangles out of collision to each other, mass adjusted.
        Vector2D BA = SplashKit.VectorInvert(SplashKit.VectorMultiply(AB,BPerc));   // get sprite B fraction
        AB = SplashKit.VectorMultiply(AB, APerc);                          // get sprite A fraction

        // sprite collisions detect AABB too late, it seems to wait too long
        // use vectors to push back out of collision
        A.Move(AB);
        B.Move(BA);

        // get velocity magnitude in direction of collision
        double A_ABMag = SplashKit.DotProduct(AVel,AB) / SplashKit.VectorMagnitude(AB);
        double B_BAMag = SplashKit.DotProduct(BVel,BA) / SplashKit.VectorMagnitude(BA);

        // original vector to zero out
        Vector2D A_ABOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),A_ABMag);
        Vector2D B_BAOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(BA),B_BAMag);

        double A_AB_P = A_ABMag * A.Mass;   // A momentum
        double B_BA_P = B_BAMag * B.Mass;   // B momentum

        // perfectly elastic collision means momentum is perfectly exchanged between objects

        A_ABMag = - B_BA_P / A.Mass;  // now with B momentum
        B_BAMag = - A_AB_P / B.Mass;  // now with A momentum
       
        //Console.Out.WriteLine(A_ABMag + " " + B_BAMag);

        // get new vector based on new momentum
        Vector2D A_ABNew = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),A_ABMag); // magnitude is negative because opposite direction
        Vector2D B_BANew = SplashKit.VectorMultiply(SplashKit.UnitVector(BA),B_BAMag);

        //Console.Out.WriteLine(A_ABNew.X + " " + A_ABNew.Y);
        //Console.Out.WriteLine(B_BANew.X + " " + B_BANew.Y);

        // trade momentum vector aligned to collision. 
        // non-elastic collisions will change this section

        // remove original vector
        A.AddToVelocity(SplashKit.VectorInvert(A_ABOrig));
        B.AddToVelocity(SplashKit.VectorInvert(B_BAOrig));

        // add new vector
        A.AddToVelocity(A_ABNew);
        B.AddToVelocity(B_BANew);
    }

    public static void CollisionElasticRectMatrix(Sprite A, Sprite B)
    {
        Vector2D AVel = A.Velocity, BVel = B.Velocity;
        // Velocity is relative overall vector between the two, it will be larger if closing towards, smaller if similar trajectory
        Vector2D AB = SplashKit.VectorOutOfRectFromRect(A.CollisionRectangle,B.CollisionRectangle,SplashKit.VectorSubtract(AVel,BVel));

        // assuming both sprites have mass values, if 0 then it won't work
        float MassTotal = A.Mass + B.Mass;
        float APerc = A.Mass / MassTotal, BPerc = B.Mass / MassTotal;  

        // vectors to push both sprite rectangles out of collision to each other, mass adjusted.
        Vector2D BA = SplashKit.VectorInvert(SplashKit.VectorMultiply(AB,BPerc));   // get sprite B fraction
        AB = SplashKit.VectorMultiply(AB, APerc);                          // get sprite A fraction

        // sprite collisions detect AABB too late, it seems to wait too long
        // use vectors to push back out of collision
        A.Move(AB);
        B.Move(BA);

        // get velocity magnitude in direction of collision
        double A_ABMag = SplashKit.DotProduct(AVel,AB) / SplashKit.VectorMagnitude(AB);
        double B_BAMag = SplashKit.DotProduct(BVel,BA) / SplashKit.VectorMagnitude(BA);

        // original vector to zero out
        Vector2D A_ABOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),A_ABMag);
        Vector2D B_BAOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(BA),B_BAMag);

        double A_AB_P = A_ABMag * A.Mass;   // A momentum
        double B_BA_P = B_BAMag * B.Mass;   // B momentum

        // perfectly elastic collision means momentum is perfectly exchanged between objects

        A_ABMag = - B_BA_P / A.Mass;  // now with B momentum
        B_BAMag = - A_AB_P / B.Mass;  // now with A momentum
       
        //Console.Out.WriteLine(A_ABMag + " " + B_BAMag);

        // get new vector based on new momentum
        Vector2D A_ABNew = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),A_ABMag); // magnitude is negative because opposite direction
        Vector2D B_BANew = SplashKit.VectorMultiply(SplashKit.UnitVector(BA),B_BAMag);

        //Console.Out.WriteLine(A_ABNew.X + " " + A_ABNew.Y);
        //Console.Out.WriteLine(B_BANew.X + " " + B_BANew.Y);

        // trade momentum vector aligned to collision. 
        // non-elastic collisions will change this section

        // remove original vector
        A.AddToVelocity(SplashKit.VectorInvert(A_ABOrig));
        B.AddToVelocity(SplashKit.VectorInvert(B_BAOrig));

        // add new vector
        A.AddToVelocity(A_ABNew);
        B.AddToVelocity(B_BANew);
    }

    // objects do not trade momentum, they follow total momentum vector
    public static void CollisionInelasticRect(Sprite A, Sprite B)
    {
        Vector2D AVel = A.Velocity, BVel = B.Velocity;
        // Velocity is relative overall vector between the two, it will be larger if closing towards, smaller if similar trajectory
        Vector2D AB = SplashKit.VectorOutOfRectFromRect(A.CollisionRectangle,B.CollisionRectangle,SplashKit.VectorSubtract(AVel,BVel));

        // assuming both sprites have mass values, if 0 then it won't work
        float MassTotal = A.Mass + B.Mass;
        float APerc = A.Mass / MassTotal, BPerc = B.Mass / MassTotal;  

        // vectors to push both sprite rectangles out of collision to each other, mass adjusted.
        Vector2D BA = SplashKit.VectorInvert(SplashKit.VectorMultiply(AB,BPerc));   // get sprite B fraction
        AB = SplashKit.VectorMultiply(AB, APerc);                          // get sprite A fraction

        // sprite collisions detect AABB too late, it seems to wait too long
        // use vectors to push back out of collision
        A.Move(AB);
        B.Move(BA);

        // get velocity magnitude in direction of collision
        double A_ABMag = SplashKit.DotProduct(AVel,AB) / SplashKit.VectorMagnitude(AB);
        double B_BAMag = SplashKit.DotProduct(BVel,BA) / SplashKit.VectorMagnitude(BA);

        // original vector to zero out
        Vector2D A_ABOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),A_ABMag);
        Vector2D B_BAOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(BA),B_BAMag);

        double A_AB_P = A_ABMag * A.Mass;   // A momentum
        double B_BA_P = B_BAMag * B.Mass;   // B momentum

        // Inelastic section

        // perfectly inelastic means both objects will not rebound but take same vector from collision
        double TotalP = A_AB_P - B_BA_P;
        double newMag = TotalP / MassTotal;

        Vector2D newVect = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),newMag);

        // remove original vector
        A.AddToVelocity(SplashKit.VectorInvert(A_ABOrig));
        B.AddToVelocity(SplashKit.VectorInvert(B_BAOrig));

        // add new vector
        A.AddToVelocity(newVect);
        B.AddToVelocity(newVect);


    }

    // combination of elastic and inelastic, using a 0-1 coefficient to determine how close to one or the other method it is.
    // 1 means 100% of elastic vector (Rebound), 0% of inelastic vector (Merge)
    // does not validate if value is beyond 0-1 range
    // collision check should be handled externally
    // sprites should have a set positive mass value
    public static void CollisionRect(Sprite A, Sprite B, float Elasticity)
    {
        Vector2D AVel = A.Velocity, BVel = B.Velocity;
        // Velocity is relative overall vector between the two, it will be larger if closing towards, smaller if similar trajectory
        Vector2D AB = SplashKit.VectorOutOfRectFromRect(A.CollisionRectangle,B.CollisionRectangle,SplashKit.VectorSubtract(AVel,BVel));

        // assuming both sprites have mass values, if 0 then it won't work
        float MassTotal = A.Mass + B.Mass;
        float APerc = A.Mass / MassTotal, BPerc = B.Mass / MassTotal;  

        // vectors to push both sprite rectangles out of collision to each other, mass adjusted.
        Vector2D BA = SplashKit.VectorInvert(SplashKit.VectorMultiply(AB,BPerc));   // get sprite B fraction
        AB = SplashKit.VectorMultiply(AB, APerc);                          // get sprite A fraction

        // sprite collisions detect AABB too late, it seems to wait too long
        // use vectors to push back out of collision
        A.Move(AB);
        B.Move(BA);

        // get velocity magnitude in direction of collision
        double A_ABMag = SplashKit.DotProduct(AVel,AB) / SplashKit.VectorMagnitude(AB);
        double B_BAMag = SplashKit.DotProduct(BVel,BA) / SplashKit.VectorMagnitude(BA);

        // original vector to zero out
        Vector2D A_ABOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),A_ABMag);
        Vector2D B_BAOrig = SplashKit.VectorMultiply(SplashKit.UnitVector(BA),B_BAMag);

        double A_AB_P = A_ABMag * A.Mass;   // A momentum
        double B_BA_P = B_BAMag * B.Mass;   // B momentum

        // inelastic section
        double TotalP = A_AB_P - B_BA_P;
        double newMag = (TotalP / MassTotal) * (1 - Elasticity);
        Vector2D newVect = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),newMag);

        // elastic section
        A_ABMag = - (B_BA_P / A.Mass) * Elasticity;  // now with B momentum
        B_BAMag = - (A_AB_P / B.Mass) * Elasticity;  // now with A momentum
        Vector2D A_ABNew = SplashKit.VectorMultiply(SplashKit.UnitVector(AB),A_ABMag); // magnitude is negative because opposite direction
        Vector2D B_BANew = SplashKit.VectorMultiply(SplashKit.UnitVector(BA),B_BAMag);

        // vector addition section

        // remove original vector
        A.AddToVelocity(SplashKit.VectorInvert(A_ABOrig));
        B.AddToVelocity(SplashKit.VectorInvert(B_BAOrig));

        // add inelastic portion
        A.AddToVelocity(newVect);
        B.AddToVelocity(newVect);

        // add elastic portion
        A.AddToVelocity(A_ABNew);
        B.AddToVelocity(B_BANew);
    }


}
using SplashKitSDK;

namespace Sky_Surge
{
    public class Bullet
    {


        public double x, y;
        private double speed;
        public int damage;
        public Bitmap bulletBitmap;

        public Bullet(double initialX, double initialY, double bulletSpeed, int bulletDamage)
        {
            x = initialX;
            y = initialY;
            speed = bulletSpeed;
            damage = bulletDamage;
            bulletBitmap = SplashKit.LoadBitmap("bullet", "bullet.png");
        }

        public void Move()
        {
            y -= speed;

            bulletBitmap.Draw(x, y);

        }


        public bool CheckCollisionP(Enemy enemy, Bullet bullet)
        {

            bool collision = SplashKit.BitmapCollision(bullet.bulletBitmap, bullet.x, bullet.y, enemy.enemySprite, enemy.x, enemy.y);

            // Debug output
            Console.WriteLine($"Bullet: ({bullet.x}, {bullet.y}), Enemy: ({enemy.x}, {enemy.y}), Collision: {collision}");



            return collision;


        }
        public bool CheckCollisionE(Player player, Bullet bullet)
        {
            return SplashKit.BitmapCollision(bulletBitmap, bullet.x, bullet.y, player.playerSprite, player.X, player.Y);

        }

    }



}
using SplashKitSDK;

namespace Sky_Surge
{
    public class Bullet
    {
        public double x, y;
        private double _speed;
        public int damage;
        private Bitmap _bulletBitmap;

        public Bullet(double initialX, double initialY, double bulletSpeed, int bulletDamage)
        {
            x = initialX;
            y = initialY;
            _speed = bulletSpeed;
            damage = bulletDamage;
            _bulletBitmap = SplashKit.LoadBitmap("bullet", "bullet.png");
        }

        public void Move()
        {
            y -= _speed;
            _bulletBitmap.Draw(x, y);
        }

        public bool CheckCollisionP(Enemy enemy, Bullet bullet)
        {
            bool collision = SplashKit.BitmapCollision(bullet._bulletBitmap, bullet.x, bullet.y, enemy.enemySprite, enemy.x, enemy.y);
            return collision;
        }

        public bool CheckCollisionE(Player player, Bullet bullet)
        {
            bool collision = SplashKit.BitmapCollision(bullet._bulletBitmap, bullet.x, bullet.y, player.playerSprite, player.X, player.Y);
            return collision;
        }
    }
}
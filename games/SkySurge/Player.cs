using System;
using SplashKitSDK;

namespace Sky_Surge
{
    public class Player
    {
        private double _x, _y;
        public Bitmap playerSprite;
        private List<Bullet> _bullets;
        public double X => _x;
        public double Y => _y;
        public int _health;

        public Player(double initialX, double initialY, int startHealth)
        {
            _x = initialX;
            _y = initialY;
            playerSprite = SplashKit.LoadBitmap("plane", "Plane.png");
            _bullets = new List<Bullet>();
            _health = startHealth;
        }

        public void Draw()
        {
            playerSprite.Draw(_x, _y);

            for (int i = _bullets.Count - 1; i >= 0; i--)
            {
                _bullets[i].Move();
                if (_bullets[i].y < 0)
                {
                    _bullets.RemoveAt(i);
                }
            }
        }

        public void Update(List<Enemy> enemies)
        {
            List<Bullet> bulletsToRemove = new List<Bullet>();

            foreach (var bullet in _bullets)
            {
                bullet.Move();

                foreach (var enemy in enemies)
                {
                    if (bullet.CheckCollisionP(enemy, bullet))
                    {
                        enemy.TakeDamage(bullet.damage);
                        bulletsToRemove.Add(bullet);
                        break;
                    }
                }
            }

            foreach (var bulletToRemove in bulletsToRemove)
            {
                _bullets.Remove(bulletToRemove);
            }
        }

        public void Move()
        {
            const double speed = 0.15;

            if (SplashKit.KeyDown(KeyCode.LeftKey))
            {
                _x -= speed;
            }

            if (SplashKit.KeyDown(KeyCode.RightKey))
            {
                _x += speed;
            }

            if (SplashKit.KeyDown(KeyCode.UpKey))
            {
                _y -= speed;
            }

            if (SplashKit.KeyDown(KeyCode.DownKey))
            {
                _y += speed;
            }
        }

        public void Shoot()
        {

            if (SplashKit.KeyTyped(KeyCode.SpaceKey))
            {
                const double bulletSpeed = 0.4;
                const double bulletWidth = 10;
                const double bulletHeight = 20;
                double bulletStartX = _x + (playerSprite.Width - bulletWidth) / 2 - 2;
                double bulletStartY = _y + (playerSprite.Height - bulletHeight) / 2 - 35;
                Bullet newBullet = new Bullet(bulletStartX, bulletStartY, bulletSpeed, 3);
                _bullets.Add(newBullet);
            }
        }

        public void TakeDamage(int damage)
        {
            _health -= damage;
        }
    }
}
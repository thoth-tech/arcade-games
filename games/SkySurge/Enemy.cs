using System;
using SplashKitSDK;

namespace Sky_Surge
{
    public class Enemy
    {
        public double x, y;
        public int health;
        public Bitmap enemySprite;
        private double _moveInterval;
        private List<Bullet> _bullets;
        private double _shootInterval;
        private double _initialShootInterval;
        private static bool _timerStarted = false;

        public Enemy(double initialX, double initialY, int enemyHealth, double initialMoveInterval, double initialShootInterval)
        {
            x = initialX;
            y = initialY;
            health = enemyHealth;
            enemySprite = SplashKit.LoadBitmap("enemy", "Stealthbomber.png");
            _moveInterval = initialMoveInterval;
            _bullets = new List<Bullet>();
            _initialShootInterval = initialShootInterval;
            _shootInterval = initialShootInterval;
            _moveInterval = initialMoveInterval;
        }

        public void Draw()
        {
            if (IsOnScreen())
            {
                enemySprite.Draw(x, y);

                for (int i = _bullets.Count - 1; i >= 0; i--)
                {
                    _bullets[i].Move();
                    if (_bullets[i].y < 0)
                    {
                        _bullets.RemoveAt(i);
                    }
                }
            }
        }

        public void Move()
        {
            if (!_timerStarted)
            {
                SplashKit.CreateTimer("timer");
                SplashKit.StartTimer("timer");
                _timerStarted = true;
            }
            const double speed = 0.025;
            if (y >= SplashKit.ScreenHeight() - enemySprite.Height)
            {
                health = 0;
            }
            if (_moveInterval <= 0)
            {
                y += speed;
                _timerStarted = false;
            }
            else
            {
                double elapsedTime = SplashKit.TimerTicks("timer");
                _moveInterval -= elapsedTime / 1000000;
            }
        }

        public void Shoot()
        {
            if (IsOnScreen())
            {
                SplashKit.CreateTimer("timer_bullet");
                SplashKit.StartTimer("timer_bullet");
                if (_shootInterval <= 0)
                {
                    const double bulletSpeed = -0.1;
                    const double bulletWidth = 10;
                    const double bulletHeight = 20;
                    double bulletStartX = x + (enemySprite.Width - bulletWidth) / 2 - 2;
                    double bulletStartY = y + (enemySprite.Height - bulletHeight) / 2 + 35;
                    Bullet newBullet = new Bullet(bulletStartX, bulletStartY, bulletSpeed, 1);
                    _bullets.Add(newBullet);
                    _shootInterval = _initialShootInterval;
                }
                else
                {
                    double elapsedTimeShoot = SplashKit.TimerTicks("timer_bullet");
                    _shootInterval -= elapsedTimeShoot;
                }
            }
        }
        public void Update(Player player)
        {
            List<Bullet> bulletsToRemove = new List<Bullet>();

            foreach (var bullet in _bullets)
            {
                bullet.Move();

                if (bullet.CheckCollisionE(player, bullet))
                {
                    player.TakeDamage(bullet.damage);
                    bulletsToRemove.Add(bullet);
                    break;
                }
            }

            foreach (var bulletToRemove in bulletsToRemove)
            {
                _bullets.Remove(bulletToRemove);
            }
        }

        public void Follow(Player player)
        {
            if (player != null)
            {
                const double speed = 0.1;
                double directionX = player.X - x;
                double directionY = player.Y - y;
                double distance = Math.Sqrt(directionX * directionX + directionY * directionY);
                if (distance > 0)
                {
                    x += directionX * speed / distance;
                    y += directionY * speed / distance;
                }
            }
        }

        public void TakeDamage(int damage)
        {
            health -= damage;
        }
        private bool IsOnScreen()
        {
            return x >= 0 && x <= SplashKit.ScreenWidth() && y >= 0 && y <= SplashKit.ScreenHeight();
        }
    }
}

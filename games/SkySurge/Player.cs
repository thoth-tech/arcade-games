using System;
using SplashKitSDK;

namespace Sky_Surge
{
    public class Player
    {
        private double x, y;
        public Bitmap playerSprite;
        private List<Bullet> bullets;
        public double X => x;
        public double Y => y;

        public Player(double initialX, double initialY)
        {
            x = initialX;
            y = initialY;
            playerSprite = SplashKit.LoadBitmap("plane", "Plane.png");
            bullets = new List<Bullet>();
        }

        public void Draw()
        {
            playerSprite.Draw(x, y);

            for (int i = bullets.Count - 1; i >= 0; i--)
            {
                bullets[i].Move();

                // Check if the bullet is off-screen and remove it
                if (bullets[i].y < 0)
                {
                    bullets.RemoveAt(i);
                }
            }
        }

        public void Update(List<Enemy> enemies)
        {
            List<Bullet> bulletsToRemove = new List<Bullet>();

            foreach (var bullet in bullets)
            {
                bullet.Move();

                foreach (var enemy in enemies)
                {
                    if (bullet.CheckCollisionP(enemy, bullet))
                    {
                        Console.WriteLine("collision");
                        enemy.TakeDamage(bullet.damage);
                        bulletsToRemove.Add(bullet);
                        break;
                    }
                }
            }

            // Remove bullets after the loop
            foreach (var bulletToRemove in bulletsToRemove)
            {
                bullets.Remove(bulletToRemove);
            }
        }

        public void Move()
        {
            const double speed = 0.15;

            if (SplashKit.KeyDown(KeyCode.LeftKey))
            {
                x -= speed;
            }

            if (SplashKit.KeyDown(KeyCode.RightKey))
            {
                x += speed;
            }

            if (SplashKit.KeyDown(KeyCode.UpKey))
            {
                y -= speed;
            }

            if (SplashKit.KeyDown(KeyCode.DownKey))
            {
                y += speed;
            }
        }

        public void Shoot(List<Enemy> enemies)
        {

            if (SplashKit.KeyTyped(KeyCode.SpaceKey))
            {
                const double bulletSpeed = 0.4;
                const double bulletWidth = 10;
                const double bulletHeight = 20;

                double bulletStartX = x + (playerSprite.Width - bulletWidth) / 2 - 2;
                double bulletStartY = y + (playerSprite.Height - bulletHeight) / 2 - 35;

                Bullet newBullet = new Bullet(bulletStartX, bulletStartY, bulletSpeed, 3);
                bullets.Add(newBullet);


                foreach (var enemy in enemies)
                {

                    if (newBullet.CheckCollisionP(enemy, newBullet))
                    {
                        Console.WriteLine("collision");
                        enemy.TakeDamage(newBullet.damage);
                        break;
                    }
                }
            }
        }

    }
}
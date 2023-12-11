using System;
using SplashKitSDK;

namespace Sky_Surge
{
    public class Enemy
    {
        public double x, y;
        public int health;
        public Bitmap enemySprite;
        private Player? player;
        private double targetX, targetY;

        public Enemy(double initialX, double initialY, double targetX, double targetY, int enemyHealth)
        {
            x = initialX;
            y = initialY;
            health = enemyHealth;
            enemySprite = SplashKit.LoadBitmap("enemy", "Stealthbomber.png");
            
        }

        public void Draw()
        {
            enemySprite.Draw(x, y);
        }

        public void SetPlayer(Player player)
        {
            this.player = player;
        }


        public void Move()
        {
            const double speed = 0.1;

            double directionX = targetX - x;
            double directionY = targetY - y;

            double distance = Math.Sqrt(directionX * directionX + directionY * directionY);

            double normalX = directionX / distance;
            double normalY = directionY / distance;

            if (distance > speed)
            {
                x += normalX * speed;
                y += normalY * speed;
            }
            else
            {
                x = targetX;
                y = targetY;
            }
        Console.WriteLine($"Enemy at ({x}, {y})");
    
        }       
        public void Follow()
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
}


}

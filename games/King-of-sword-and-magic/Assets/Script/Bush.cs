using UnityEngine;

public class Bush : EnemyBase
{
    public override void Start()
    {
        
    }

    public override void Update()
    {

    }

    public override void TakeDamage(int damage, Transform owner, int level)
    {
        Instantiate(deadVFX, transform.position, transform.rotation);
        for (int i = 0; i < dropItem.Length; i++)
        {
            if (Random.Range(0, 100) < 80)
            {
                Instantiate(dropItem[i],
                transform.position + new Vector3(0.5f * i, 0.5f * i, 0), transform.rotation);
            }
        }
        Destroy(gameObject);
    }
}

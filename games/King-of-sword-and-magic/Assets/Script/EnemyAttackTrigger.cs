using UnityEngine;

public class EnemyAttackTrigger : MonoBehaviour
{
    public int damage;
    public Transform owner;
    public float destroyTime = 0.2f;
    public GameObject effect;

    private void Start()
    {
        if (effect != null)
        {
            Destroy(effect, destroyTime);
        }
        Destroy(gameObject, destroyTime);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Player"))
        {
            collision.gameObject.GetComponent<Player>().TakeDamage(damage, owner);
            if(effect != null)
            {
                Destroy(effect);
            }
            Destroy(gameObject);
        }
        if (collision.CompareTag("Wall"))
        {
            if (effect != null)
            {
                Destroy(effect);
            }
            Destroy(gameObject);
        }
    }
    public void SetDamage(int Num, Transform Owner)
    {
        damage = Num;
        owner = Owner;
    }
}

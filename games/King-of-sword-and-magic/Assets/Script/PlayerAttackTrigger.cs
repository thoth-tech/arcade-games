using UnityEngine;

public class PlayerAttackTrigger : MonoBehaviour
{
    public int damage = 10;
    public Transform owner;
    public float destroyTime = 0.2f;
    public int attackLevel = 1;

    private void Start()
    {
        Destroy(gameObject, destroyTime);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Enemy"))
        {
            collision.gameObject.GetComponent<EnemyBase>().
                TakeDamage(damage, owner , attackLevel);
        }
    }

    public void SetDamage(int Num, Transform Owner, int Level)
    {
        damage = Num;
        owner = Owner;
        attackLevel = Level;
    }



}

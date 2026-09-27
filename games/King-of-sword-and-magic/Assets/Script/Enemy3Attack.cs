using UnityEngine;

public class Enemy3Attack : MonoBehaviour
{
    public float attackDis = 10.0f;
    public GameObject bulletPrefab;
    private GameObject player;
    private bool canAttack = true;
    public Animator anim;
    public AudioClip attackSound;
    void Start()
    {
        player = GameObject.Find("Player");
    }

    void Update()
    {
        if (!player) { return; }
        if (Vector3.Distance(player.transform.position,
            transform.position) < attackDis)
        {
            Attack();
        }
    }

    public void Attack()
    {
        if (!canAttack) { return; }
        canAttack = false;
        anim.SetTrigger("Attack");
        Invoke(nameof(CreateBullet), 0.45f);
        Invoke("ResetCanAttack", 2.0f);
        SoundManger.Instance.PlaySFXSound(attackSound);
    }

    public void CreateBullet()
    {
        Instantiate(bulletPrefab, transform.position, transform.rotation);
    }

    public void ResetCanAttack()
    {
        canAttack = true;
    }
}

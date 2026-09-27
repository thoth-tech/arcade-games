using UnityEngine;

public class Enemy2Attack : MonoBehaviour
{
    public float attackDis = 10.0f;
    public GameObject bulletPrefab;
    private GameObject player;
    private bool canAttack = true;
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
        Instantiate(bulletPrefab, transform.position, transform.rotation);
        SoundManger.Instance.PlaySFXSound(attackSound);
        Invoke("ResetCanAttack", 1.0f); 
    }

    public void ResetCanAttack()
    {
        canAttack = true;
    }
}

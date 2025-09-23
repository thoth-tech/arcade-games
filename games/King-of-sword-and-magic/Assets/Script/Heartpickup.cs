using UnityEngine;

public class Heartpickup : MonoBehaviour
{
    public GameObject player;
    public bool canFly = false;
    public Rigidbody2D rb;
    public float flySpeed = 5f;
   
    void Update()
    {
        if (canFly)
        {
            FlyToPlayer();
        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Player"))
        {
            player = collision.gameObject;
            canFly = true;
        }
    }

    public void FlyToPlayer()
    {
        rb.linearVelocity = (player.transform.position -
            transform.position).normalized * flySpeed;
        if (Vector2.Distance(transform.position, player.transform.position) < 0.8f)
        {
            player.GetComponent<Player>().ADDHealth(10);
            Destroy(gameObject);
        }
    }
}

using UnityEngine;

public class Enemy3Bullet : MonoBehaviour
{
    public GameObject player;
    public Rigidbody2D rb;
    public float speed = 5f;
    private Vector3 tarDir;
    void Start()
    {
        player = GameObject.Find("Player");
        tarDir = (player.transform.position - transform.position).normalized;
    }

    
    void Update()
    {
        rb.linearVelocity = tarDir * speed;
    }
}

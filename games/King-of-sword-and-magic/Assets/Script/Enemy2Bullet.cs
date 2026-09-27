using UnityEngine;

public class Enemy2Bullet : MonoBehaviour
{
    public Rigidbody2D rb;
    public float speed = 10f;
    void Start()
    {
        
    }

    
    void Update()
    {
        rb.linearVelocity = transform.right * speed;
    }
}

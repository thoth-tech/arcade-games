using UnityEngine;

public class Arrow : MonoBehaviour
{
    public Rigidbody2D rb;
    public float speed = 30f;
    public GameObject sr;
    void Start()
    {
        Destroy(gameObject, 3f);
    }

    
    void Update()
    {
       rb.linearVelocity = transform.right * speed;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.CompareTag("Wall"))
        {
            sr.transform.SetParent(collision.transform);
            Destroy(gameObject);
        }
        else if(collision.CompareTag("Enemy"))
        {
            if(collision.gameObject)
            {
                sr.transform.SetParent(collision.transform);
                Destroy(gameObject);
            }
            else
            {
                Destroy(gameObject);
            }
        }
    }
}

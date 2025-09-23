using UnityEngine;

public class MagicLine : MonoBehaviour
{
    public SpriteRenderer sr;
    public CapsuleCollider2D col;
    void Start()
    {
        Destroy(gameObject, 0.3f);
        InvokeRepeating(nameof(SetLine), 0.01f, 0.01f);
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void SetLine()
    {
        sr.size = new Vector2(sr.size.x + 0.5f, sr.size.y);
        col.size = new Vector2(sr.size.x + 0.5f, 0.45f);
        col.offset = new Vector2(sr.size.x / 2, 0.02f);
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Wall"))
        {
            CancelInvoke(nameof(SetLine));
        }
    }
}
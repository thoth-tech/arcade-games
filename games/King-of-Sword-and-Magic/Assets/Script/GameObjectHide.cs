using UnityEngine;
using UnityEngine.Tilemaps;

public class GameObjectHide : MonoBehaviour
{
    public Tilemap map;
    public SpriteRenderer sr;
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.tag =="Player")
        {
            if (map != null)
            { map.color = new Color(1, 1, 1, 0.95f); } 
            if (sr != null)
            { sr.color = new Color(1, 1, 1, 0.7f); }
        }
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        if(collision.tag =="Player")
        {
            if (map != null)
            { map.color = new Color(1, 1, 1, 1); }
            if (sr != null)
            { sr.color = new Color(1, 1, 1, 1); }
        }
    }
}

using UnityEngine;

public class PosHide : MonoBehaviour
{
    public SpriteRenderer sr;

    void Awake()
    {
        sr.enabled = false;
    }


}

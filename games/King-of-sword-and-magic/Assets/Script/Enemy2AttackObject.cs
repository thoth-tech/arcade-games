using UnityEngine;

public class Enemy2AttackObject : MonoBehaviour
{
    public float destroyTime = 1f;
    
    void Start()
    {
        Destroy(gameObject, destroyTime);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

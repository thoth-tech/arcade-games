using UnityEngine;

public class WeaponPivot : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        // 使用方向键控制武器朝向（鼠标不再控制）
        Vector2 aim = Vector2.zero;
        if (Input.GetKey(KeyCode.UpArrow)) { aim.y += 1f; }
        if (Input.GetKey(KeyCode.DownArrow)) { aim.y -= 1f; }
        if (Input.GetKey(KeyCode.LeftArrow)) { aim.x -= 1f; }
        if (Input.GetKey(KeyCode.RightArrow)) { aim.x += 1f; }

        if (aim.sqrMagnitude > 0.01f)
        {
            float angle = Mathf.Atan2(aim.y, aim.x) * Mathf.Rad2Deg;
            transform.rotation = Quaternion.Euler(0, 0, angle);
        }
    }
}

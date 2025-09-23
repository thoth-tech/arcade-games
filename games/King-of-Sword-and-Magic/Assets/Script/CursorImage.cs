using UnityEngine;
using UnityEngine.UI;

public class CursorImage : MonoBehaviour
{
    public Image cursorImage;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        // 用方向键模拟鼠标移动
        Vector3 pos = transform.position;
        float moveSpeed = 600f * Time.deltaTime; // 屏幕空间速度
        if (Input.GetKey(KeyCode.UpArrow))
        {
            pos.y += moveSpeed;
        }
        if (Input.GetKey(KeyCode.DownArrow))
        {
            pos.y -= moveSpeed;
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            pos.x -= moveSpeed;
        }
        if (Input.GetKey(KeyCode.RightArrow))
        {
            pos.x += moveSpeed;
        }
        transform.position = pos;
        
        // 将所有鼠标右键等价为 U 键，这里保留左键视觉效果但响应 U
        if(Input.GetKeyDown(KeyCode.U))
        {
            cursorImage.color = Color.red;
            transform.localScale = new Vector3(0.8f, 0.8f, 0.8f);
        }
        else if(Input.GetKeyUp(KeyCode.U))
        {
            cursorImage.color = Color.white;
            transform.localScale = new Vector3(1f, 1f, 1f);
        }
    }
}

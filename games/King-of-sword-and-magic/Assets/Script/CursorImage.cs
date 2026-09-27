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
        // 鼠标点击不再触发攻击，这里仅跟随光标（如需隐藏可在 Inspector 关闭该对象）
        transform.position = Input.mousePosition;
    }
}

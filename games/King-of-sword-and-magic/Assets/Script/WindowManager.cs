using UnityEngine;
using UnityEngine.SceneManagement;

public class WindowManager : MonoBehaviour
{
    // 将此脚本挂到任意常驻对象（如 GameManager），或放到每个场景里
    private const int MinWidth = 640;
    private const int MinHeight = 480;
    private const int MaxWidth = 1600;
    private const int MaxHeight = 900;

    void Awake()
    {
        DontDestroyOnLoad(gameObject);
        ApplyWindowConstraints();
        // 编译前移除边框（运行时设置为无边框窗口）
        Screen.fullScreenMode = FullScreenMode.FullScreenWindow;
    }

    void OnApplicationFocus(bool hasFocus)
    {
        if (hasFocus)
        {
            ApplyWindowConstraints();
        }
    }

    void Update()
    {
        // 运行过程中避免超过范围
        ApplyWindowConstraints();
    }

    private void ApplyWindowConstraints()
    {
        int width = Mathf.Clamp(Screen.width, MinWidth, MaxWidth);
        int height = Mathf.Clamp(Screen.height, MinHeight, MaxHeight);
        if (width != Screen.width || height != Screen.height)
        {
            Screen.SetResolution(width, height, Screen.fullScreenMode);
        }
    }
}



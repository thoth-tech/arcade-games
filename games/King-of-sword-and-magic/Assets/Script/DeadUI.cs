using Unity.Cinemachine;
using UnityEngine;
using UnityEngine.SceneManagement;

public class DeadUI : MonoBehaviour
{
    private bool isActive;
    private void OnEnable()
    {
        isActive = true;
    }
    private void OnDisable()
    {
        isActive = false;
    }
    private void Update()
    {
        if (!isActive) { return; }
        // 死亡重生界面：Enter 重新开始，Escape 返回主菜单
        if (Input.GetKeyDown(KeyCode.Return))
        {
            RestGameButton();
        }
        else if (Input.GetKeyDown(KeyCode.Escape))
        {
            BackMainButton();
        }
    }
    public void RestGameButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        Invoke(nameof(JumpToScene1), 1.0f);
    }

    public void BackMainButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        Invoke(nameof(JumpToMainScene), 1.0f);
    }
    public void JumpToMainScene()
    {
        gameObject.SetActive(false);
        SceneManager.LoadScene(0);
    }
    public void JumpToScene1()
    {
        Player.Instance.PlayerResurrection();
        gameObject.SetActive(false);
        SceneManager.LoadScene(1);
    }
}

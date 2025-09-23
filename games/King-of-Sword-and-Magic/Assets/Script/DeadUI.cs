using Unity.Cinemachine;
using UnityEngine;
using UnityEngine.SceneManagement;

public class DeadUI : MonoBehaviour
{
    private bool hasScheduledAction = false;
    private void Update()
    {
        // 死亡界面：Enter 重新开始，ESC 返回主界面
        if (gameObject.activeInHierarchy && !hasScheduledAction)
        {
            if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.KeypadEnter))
            {
                RestGameButton();
                hasScheduledAction = true;
            }
            else if (Input.GetKeyDown(KeyCode.Escape))
            {
                BackMainButton();
                hasScheduledAction = true;
            }
        }
    }
    public void RestGameButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        Invoke(nameof(JumpToScene1), 1.0f);
        hasScheduledAction = true;
    }

    public void BackMainButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        Invoke(nameof(JumpToMainScene), 1.0f);
        hasScheduledAction = true;
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

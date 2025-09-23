using UnityEngine;
using UnityEngine.SceneManagement;
#if UNITY_EDITOR
using UnityEditor;
#endif

public class StartGameCanvas : MonoBehaviour
{
    public GameObject[] objs;
    private bool hasScheduledAction = false;
    private void Start()
    {
        if (!CanvasMain.Instance) { return; }
        CanvasMain.Instance.StartClip();
        objs[0] = Player.Instance.gameObject;
        objs[1] = CanvasMain.Instance.gameObject.transform.GetChild(0).gameObject;
        foreach (GameObject obj in objs)
        {
            obj.SetActive(false);
        }
    }
    private void Update()
    {
        // Enter 开始游戏, ESC 退出游戏
        if (!hasScheduledAction && gameObject.activeInHierarchy)
        {
            if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.KeypadEnter))
            {
                StartGameButton();
                hasScheduledAction = true;
            }
            else if (Input.GetKeyDown(KeyCode.Escape))
            {
                QuitGameButton();
                hasScheduledAction = true;
            }
        }
    }
    public void StartGameButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        gameObject.SetActive(false);
        Invoke("JumpToScene1", 1f);
        hasScheduledAction = true;
    }
    public void QuitGameButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        Invoke("EndGame", 1.0f);
        hasScheduledAction = true;
    }

    public void JumpToScene1()
    {
        foreach (GameObject obj in objs)
        {
            obj.SetActive(true);
        }
        Player.Instance.PlayerResurrection();
        SceneManager.LoadScene(1);
    }
    public void EndGame()
    {
#if UNITY_EDITOR
        EditorApplication.isPlaying = false;
#endif
        Application.Quit();
    }
}

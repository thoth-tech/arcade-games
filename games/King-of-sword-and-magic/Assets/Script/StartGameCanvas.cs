using UnityEngine;
using UnityEngine.SceneManagement;
#if UNITY_EDITOR
using UnityEditor;
#endif

public class StartGameCanvas : MonoBehaviour
{
    public GameObject[] objs;
    private bool hasInitialized = false;
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
        hasInitialized = true;
    }
    private void Update()
    {
        if (!hasInitialized) { return; }
        // 开始界面：Enter 开始游戏，Escape/Exit 退出
        if (Input.GetKeyDown(KeyCode.Return))
        {
            StartGameButton();
        }
        else if (Input.GetKeyDown(KeyCode.Escape))
        {
            QuitGameButton();
        }
    }
    public void StartGameButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        gameObject.SetActive(false);
        Invoke("JumpToScene1", 1f);
    }
    public void QuitGameButton()
    {
        if (CanvasMain.Instance)
        { CanvasMain.Instance.EndClip(); }
        Invoke("EndGame", 1.0f);
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

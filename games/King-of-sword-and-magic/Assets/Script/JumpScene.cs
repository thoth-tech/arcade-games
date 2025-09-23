using UnityEngine;
using UnityEngine.SceneManagement;

public class JumpScene : MonoBehaviour
{
    public int sceneIndex = 0;
    public Vector3 playerNewPos;
    private GameObject player;

    private void Start()
    {
        if(CanvasMain.Instance)
        {
            CanvasMain.Instance.StartClip();
            player = Player.Instance.gameObject;
        }
    }

    public void JumpToScene()
    {
        SceneManager.LoadScene(sceneIndex);
        player.transform.position = playerNewPos;
    }
    public void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            CanvasMain.Instance.EndClip();
            Invoke("JumpToScene", 1f);
        }
    }
}

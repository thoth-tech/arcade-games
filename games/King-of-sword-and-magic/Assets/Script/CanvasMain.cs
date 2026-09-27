using UnityEngine;

public class CanvasMain : MonoBehaviour
{
    public static CanvasMain Instance;
    public Animator Anim;
    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    public void StartClip()
    {
        Anim.SetTrigger("StartClip");
    }
    public void EndClip()
    {
        Anim.SetTrigger("EndClip");
    }
}

using UnityEngine;
using UnityEngine.SceneManagement;

public class SoundManger : MonoBehaviour
{
    public static SoundManger Instance;
    public AudioSource BGM;
    public AudioSource SFX;
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
    public void PlayBGMSound(AudioClip clip)
    {
        if(BGM.clip != clip)
        {
            BGM.clip = clip;
            BGM.Play();
        }
    }
    public void PlaySFXSound(AudioClip clip)
    {
        SFX.PlayOneShot(clip);
    }
}

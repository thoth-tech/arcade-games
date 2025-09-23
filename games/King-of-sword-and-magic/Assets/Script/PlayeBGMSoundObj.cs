using UnityEngine;

public class PlayeBGMSoundObj : MonoBehaviour
{
    public AudioClip BGM;
    private void Awake()
    {
        if(SoundManger.Instance)
        {
            SoundManger.Instance.PlayBGMSound(BGM);
        }
    }
}

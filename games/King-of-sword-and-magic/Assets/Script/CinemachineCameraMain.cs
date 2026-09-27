using UnityEngine;

public class CinemachineCameraMain : MonoBehaviour
{
    public static CinemachineCameraMain Instance;
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
}

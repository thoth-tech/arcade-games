using UnityEngine;
using UnityEngine.UI;

public class NPC : MonoBehaviour
{
    public Text npcText;
    public Text npcText2;
    public string npcTextString;
    public string npcTextString2;
    public GameObject npcPanel;
    public GameObject npcPanel2;
    public GameObject npcPanel3;
    public bool isPlayerInRange;

    void Start()
    {
        npcText.text = npcTextString;
        npcText2.text = npcTextString2;
        Invoke("OpenPanel", Random.Range(3f, 5f));
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.F) && isPlayerInRange)
        {
            npcPanel2.SetActive(false);
            npcPanel3.SetActive(true);
        }
    }
    public void OpenPanel()
    {
        if (!isPlayerInRange)
        { npcPanel.SetActive(true); }
        Invoke("ClosePanel", Random.Range(3f, 5.0f)); 
    }
    public void ClosePanel()
    {
        if(!isPlayerInRange)
        { npcPanel.SetActive(false); }
        Invoke("OpenPanel", Random.Range(3f, 5.0f));
    }
    public void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            isPlayerInRange = true;
            npcPanel2.SetActive(true);
            npcPanel.SetActive(false);
        }
    }
    public void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            isPlayerInRange = false;
            npcPanel2.SetActive(false);
            npcPanel3.SetActive(false);
        }
    }
}

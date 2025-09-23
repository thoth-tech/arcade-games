using UnityEngine;
using UnityEngine.UI;

public enum WeaponType { Sword, Bow, Staff }

public  class Player : MonoBehaviour
{
    public static Player Instance;
    [Header("基础属性")]
    public float speed = 5f;
    public int HP = 100;
    public int HPNow = 100;
    public int ATK = 10;    
    private float h, v;
    [Header("组件")]
    public Rigidbody2D rb;
    public Animator animPlayer;
    public Animator animSword;
    public Animator animSlash;
    public SpriteRenderer sr;
    public GameObject swordTri;
    public Transform swordTriPos;
    public Slider hpBar;
    [Header("玩家死亡")]
    public GameObject deadVFX;
    public bool isDead = false;
    public GameObject deadUI;
    public bool isGetHit = false;
    [Header("攻击")]
    public GameObject[] weapons;
    public GameObject[] weaponsUI;
    public GameObject arrowTri;
    public Transform arrowTriPos;
    public GameObject staffTri;
    public Transform staffTriPos;
    public AudioClip attackSound;
    public AudioClip attack2Sound;
    public AudioClip attack3Sound;
    public WeaponType currentWeapon = WeaponType.Sword;
    private bool canAttack = true;
    [Header("金币")]
    public int coin = 0;
    public Text coinText;

    private void Awake()
    {
        //单例制作
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

    void Start()
    {
        HPNow = HP;
        ClampWindowSize();
    }


    void Update()
    {
        // 全局 ESC 退出
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            QuitGameImmediately();
            return;
        }
        if(!isDead)
        {
            Move();
            SwitchWeapon();
            Attack();
        }
        else
        {
            rb.linearVelocity = Vector2.zero;
        }
        ClampWindowSize();
    }

    private void QuitGameImmediately()
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#endif
        Application.Quit();
    }

    private void ClampWindowSize()
    {
        if (Screen.fullScreen) { return; }
        int minW = 640;
        int minH = 480;
        int maxW = 1600;
        int maxH = 900;
        int w = Screen.width;
        int h = Screen.height;
        int clampedW = Mathf.Clamp(w, minW, maxW);
        int clampedH = Mathf.Clamp(h, minH, maxH);
        if (clampedW != w || clampedH != h)
        {
            Screen.SetResolution(clampedW, clampedH, FullScreenMode.Windowed);
        }
    }

    public void Move()
    {
        // 仅使用 WASD 控制角色移动，避免方向键影响移动
        h = 0f;
        v = 0f;
        if (Input.GetKey(KeyCode.A)) { h -= 1f; }
        if (Input.GetKey(KeyCode.D)) { h += 1f; }
        if (Input.GetKey(KeyCode.S)) { v -= 1f; }
        if (Input.GetKey(KeyCode.W)) { v += 1f; }
        if (!isGetHit)//如果玩家受伤
        { rb.linearVelocity = new Vector2(h * speed, v * speed); }
        //动画
        if(h!=0 || v !=0)
        {
            animPlayer.SetBool("IsRun", true);
        }
        else
        {
            animPlayer.SetBool("IsRun", false);
        }
        //翻转角色
        if (h > 0) { sr.flipX = false; }
        else if( h < 0) { sr.flipX = true; }
    }

    public void Attack()
    {
        if(Input.GetKeyDown(KeyCode.U))
        {
            if(!canAttack){ return; }
            canAttack = false;
            Invoke(nameof(ResetcanAttack), 0.5f);
            if(currentWeapon == WeaponType.Sword)//剑
            {
                //动画
                animSword.SetTrigger("Attack1");
                animSlash.SetTrigger("Attack1");
                //生成攻击盒子以及伤害设置
                GameObject go = Instantiate(swordTri, swordTriPos.position,
                    swordTriPos.rotation, swordTriPos);
                go.GetComponent<PlayerAttackTrigger>().
                    SetDamage(ATK + Random.Range(0, 5), transform, 13);
                //声音
                SoundManger.Instance.PlaySFXSound(attackSound);
            }
            else if(currentWeapon == WeaponType.Bow)//弓
            {
                GameObject go = Instantiate(arrowTri, arrowTriPos.position,
                    arrowTriPos.rotation);
                go.GetComponent<PlayerAttackTrigger>().
                    SetDamage(ATK - Random.Range(3, 5), transform, 3);
                SoundManger.Instance.PlaySFXSound(attack2Sound);
            }
            else if(currentWeapon == WeaponType.Staff)//杖
            {
                GameObject go = Instantiate(staffTri, staffTriPos.position,
                   staffTriPos.rotation);
                go.GetComponent<PlayerAttackTrigger>().
                    SetDamage(ATK - Random.Range(0, 3), transform, 6);
                SoundManger.Instance.PlaySFXSound(attack3Sound);
            } 
        }
    }

    public void ResetcanAttack()
    {
        canAttack = true;
    }

    public void SwitchWeapon()
    {
      if(Input.GetKeyDown(KeyCode.T))
        {
            HideWeapons();//隐藏所有武器
            currentWeapon = WeaponType.Sword;
            //显示当前武器
            weapons[0].SetActive(true);
            weaponsUI[0].SetActive(true);
        }
      else if(Input.GetKeyDown(KeyCode.G))
        {
            HideWeapons();
            currentWeapon = WeaponType.Bow;
            weapons[1].SetActive(true);
            weaponsUI[1].SetActive(true);
        }
      else if(Input.GetKeyDown(KeyCode.Y))
        {
            HideWeapons();
            currentWeapon = WeaponType.Staff;
            weapons[2].SetActive(true);
            weaponsUI[2].SetActive(true);
        }
    }

    public void HideWeapons()
    {
        for(int i = 0; i < weapons.Length; i++)
        {
            weapons[i].SetActive(false);
            weaponsUI[i].SetActive(false);
        }
    }

    public void TakeDamage(int damage,Transform owner)
    {
        //扣血
        CancelInvoke(nameof(GetHitAnimEnd));
        animPlayer.SetBool("IsGetHit", true);
        isGetHit = true;
        //血量UI显示
        HPNow = HPNow - damage;
        hpBar.value = (float)HPNow / (float)HP;
        Invoke(nameof(GetHitAnimEnd), 0.4f);
        //击退效果
        Vector2 dir = (transform.position - owner.position).normalized;
        rb.linearVelocity = dir * 10;
        //死亡
        if (HPNow <= 0)
        {
            Instantiate(deadVFX, transform.position, transform.rotation);
            gameObject.SetActive(false);
            isDead = true;
            deadUI.SetActive(true);
        }
    }

    public void GetHitAnimEnd()
    {
        animPlayer.SetBool("IsGetHit", false);
        isGetHit = false;
    }

    public void ADDCoin(int num)
    {
        coin = coin + num;
        coinText.text = coin.ToString();
    }

    public void ADDHealth(int num)
    {
        HPNow = HPNow + num;
        if(HPNow > HP){HPNow = HP;}
        hpBar.value = (float)HPNow / (float)HP;
    }
    //复活
    public void PlayerResurrection()
    {
        gameObject.SetActive(true);
        isDead = false;
        deadUI.SetActive(false);
        HPNow = HP;
        hpBar.value = (float)HPNow / (float)HP;
    }
}

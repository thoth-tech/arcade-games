using UnityEngine;
using UnityEngine.UI;

public class EnemyBase : MonoBehaviour
{
    [Header("组件")]
    public Animator anim;
    public GameObject attackTrigger1;
    public Transform attackPos1;
    public Rigidbody2D rb;
    public Player player;
    public SpriteRenderer sr;
    [Header("Bool变量")]
    public bool playerInRange = false;
    public bool canAttack1 = true;
    [Header("基础属性")]
    public int ATK = 5;
    public int HP = 100;    
    private int HPNow = 100;
    [Header("血量相关变量")]
    public Slider hpBar;
    public GameObject deadVFX;
    [Header("怪物移动")]
    public Transform[] movePos;
    public float moveSpeed = 2.0f;
    private Transform tarPos;
    private bool waitIdle = false;
    public bool trace = false;  
    private bool isGetHit = false;
    [Header("怪物掉落")]
    public GameObject[] dropItem;

    public virtual void Start()
    {
        //移动点判定
        if(movePos.Length > 1){tarPos = movePos[Random.Range(0, movePos.Length)];}
        else{Debug.LogError("移动点数量不足");}
        //玩家判定
        if (Player.Instance)
        { player = Player.Instance; }
    }

    public  virtual void Update()
    {
        CreatAttackTrigger1();
        if(!isGetHit)
        {
            if(!trace)
            {
                Move();
            }
            else
            {
                Trace();
            }
        }
    }

    public virtual void TakeDamage(int damage,Transform owner,int level)
    {
        //追踪玩家
        trace = true;
        //动画与击退效果
        CancelInvoke(nameof(GetHitAnimEnd));
        anim.SetBool("IsGetHit", true);
        isGetHit = true;
        Vector2 dir = (transform.position - owner.position).normalized;
        rb.linearVelocity = dir *  level;
        Invoke(nameof(GetHitAnimEnd), 0.25f);
        //血量减少与血条显示
        CancelInvoke(nameof(HPBarHide));
        HPNow -= damage;
        hpBar.gameObject.SetActive(true);
        hpBar.value = (float)HPNow / (float)HP;
        Invoke(nameof(HPBarHide), 2.0f);
        //死亡判定
        if(HPNow <= 0)
        {
            Instantiate(deadVFX, transform.position, transform.rotation);
            for(int i = 0; i < dropItem.Length; i++)
            {
                if (Random.Range(0, 100) < 50)
                { Instantiate(dropItem[i], 
                    transform.position + new Vector3(0.5f*i,0.5f*i,0), transform.rotation); }
            }
            Destroy(gameObject);
        }
    }

    public void GetHitAnimEnd()
    {
        anim.SetBool("IsGetHit", false);
        isGetHit = false;
    }

    public void HPBarHide()
    {
        hpBar.gameObject.SetActive(false);
    }
    //追踪玩家
    public void Trace()
    {
        Vector2 dir = (player.gameObject.transform.position - transform.position).normalized;
        rb.linearVelocity = dir * moveSpeed;
        anim.SetBool("IsMove", true);
        sr.flipX = (rb.linearVelocity.x < 0);//动画翻转
    }

    public void CreatAttackTrigger1()
    {
        //玩家是否存在以及是否在攻击范围内
        if (!player) { return;  }
        if(Vector2.Distance(transform.position,player.transform.position)<= 5f)
        {
            playerInRange = true;
        }
        else
        {
            playerInRange = false;
        }
        //攻击判定
        if(playerInRange)
        {
            if(canAttack1)
            {
                //创建攻击触发器
                CancelInvoke(nameof(Attack1End));
                 GameObject go = Instantiate(attackTrigger1, attackPos1.position,
                    attackPos1.rotation,attackPos1);
                go.GetComponent<EnemyAttackTrigger>().
                    SetDamage(ATK + Random.Range(0, 5),transform);
                canAttack1 = false;
                Invoke(nameof(Attack1End), 0.4f);
            }

        }
    }

    public void Attack1End()
    {
        canAttack1 = true;
    }

    public void Move()
    {
        if(waitIdle)//静止
        { 
            rb.linearVelocity = Vector2.zero ; 
            anim.SetBool("IsMove", false);
            return; 
        }
        //路径巡逻
        Vector3 dir = (tarPos.position - transform.position).normalized;
        if(Vector3.Distance(transform.position, tarPos.position) > 0.5f)
        {
            rb.linearVelocity = dir * moveSpeed;
            anim.SetBool("IsMove", true);
            sr.flipX = (rb.linearVelocity.x < 0);
        }
        else
        {
            rb.linearVelocity = Vector2.zero;
            tarPos = GetNewTarPos();
            waitIdle = true;
            Invoke(nameof(RestWaitIdle), 2.0f);
        }
    }

    public void RestWaitIdle()
    {
        waitIdle = false;
    }
    //获取移动点
    public Transform GetNewTarPos()
    {
        if(movePos.Length < 2)
        { return transform; }

        Transform tar;
        tar = movePos[Random.Range(0, movePos.Length)];
        if(tar != tarPos){return tar;}
        else{return GetNewTarPos();}
    }

}

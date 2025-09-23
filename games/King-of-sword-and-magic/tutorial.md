# Unity 6 2D RPG Quickstart & Project-Based Tutorial

## Table of Contents
- 1. Project Structure & Core Systems: overview of scripts/scenes/prefabs
- 2. Environment & Build Settings: Unity version, template, build order, input handling
- 3. Player Setup: components & Inspector bindings, move/attack/damage/death
- 4. Enemies (EnemyBase) & AI/Attacks: patrol/chase/attack spawning/damage & drops
- 5. Attack Triggers & Pickups: hit detection, coin/heart magnet and settlement
- 6. UI & Fades: canvas fade in/out, main menu buttons, death UI behavior
- 7. Scenes & Transitions: build list, trigger-based scene jump (JumpScene)
- 8. 10-Minute Rebuild Checklist: shortest path to a runnable loop
- 9. Unity 6 Beginner Notes: physics/anim/collision/singletons
- 10. Deploying on Raspberry Pi 5 (Linux): WebGL/Nginx deployment, kiosk, checks

## 1. Project Structure & Core Systems
- Player: `Assets/Script/Player.cs`
- Enemy base & AI: `Assets/Script/EnemyBase.cs`
- Attack triggers: `Assets/Script/PlayerAttackTrigger.cs`, `Assets/Script/EnemyAttackTrigger.cs`
- Pickups: `Assets/Script/Coin.cs`, `Assets/Script/Heartpickup.cs`
- UI/Fades: `Assets/Script/CanvasMain.cs`, `Assets/Script/StartGameCanvas.cs`, `Assets/Script/DeadUI.cs`
- Scene switching: `Assets/Script/JumpScene.cs`
- Key scenes: `Assets/Scenes/StartScene.unity`, `Assets/Scenes/Game1Scene1.unity`
- Prefabs: `Assets/Prefab/` (enemy bundles, attack bodies, pickups, etc.)

## 2. Environment & Build Settings
1) Install Unity 6 via Unity Hub (2D URP template recommended).
2) Open `File > Build Settings` and add:
   - 0: `Assets/Scenes/StartScene.unity`
   - 1: `Assets/Scenes/Game1Scene1.unity`
   - 2: `Assets/Scenes/Game1Scene2.unity`
   - 3: `Assets/Scenes/Game1Scene3.unity`
   - 4: `Assets/Scenes/Game1Scene4.unity`
3) Set `Project Settings > Player > Active Input Handling` to Both (Old + New).

## 3. Player Setup
Script: `Assets/Script/Player.cs`
- Components: `Rigidbody2D` (Dynamic), `BoxCollider2D`, `Animator`, `SpriteRenderer`
- Inspector bindings:
  - `rb/animPlayer/sr`
  - `hpBar` (Slider), `coinText` (Text)
  - Three weapon prefabs and spawn points: `swordTri + swordTriPos`, `arrowTri + arrowTriPos`, `staffTri + staffTriPos`
  - Attack SFX: `attackSound/attack2Sound/attack3Sound`
  - Death UI: `deadUI`, death VFX: `deadVFX`

Movement:
```startLine:endLine:filepath
79:98:Assets/Script/Player.cs
    public void Move()
    {
        h = Input.GetAxisRaw("Horizontal");
        v = Input.GetAxisRaw("Vertical");
        if (!isGetHit)
        { rb.linearVelocity = new Vector2(h * speed, v * speed); }
        animPlayer.SetBool("IsRun", h!=0 || v!=0);
        if (h > 0) { sr.flipX = false; }
        else if( h < 0) { sr.flipX = true; }
    }
```

Attacks & weapons:
```startLine:endLine:filepath
100:127:Assets/Script/Player.cs
    public void Attack()
    {
        if(Input.GetMouseButtonDown(0))
        {
            if(!canAttack){ return; }
            canAttack = false;
            Invoke(nameof(ResetcanAttack), 0.5f);
            if(currentWeapon == WeaponType.Sword)
            {
                animSword.SetTrigger("Attack1");
                animSlash.SetTrigger("Attack1");
                var go = Instantiate(swordTri, swordTriPos.position, swordTriPos.rotation, swordTriPos);
                go.GetComponent<PlayerAttackTrigger>().SetDamage(ATK + Random.Range(0, 5), transform, 13);
            }
            else if(currentWeapon == WeaponType.Bow)
            {
                var go = Instantiate(arrowTri, arrowTriPos.position, arrowTriPos.rotation);
                go.GetComponent<PlayerAttackTrigger>().SetDamage(ATK - Random.Range(3, 5), transform, 3);
            }
```

Damage, hit-stun, death & revive:
```startLine:endLine:filepath
179:199:Assets/Script/Player.cs
    public void TakeDamage(int damage,Transform owner)
    {
        animPlayer.SetBool("IsGetHit", true);
        isGetHit = true;
        HPNow -= damage;
        hpBar.value = (float)HPNow / (float)HP;
        Invoke(nameof(GetHitAnimEnd), 0.4f);
        Vector2 dir = (transform.position - owner.position).normalized;
        rb.linearVelocity = dir * 10;
        if (HPNow <= 0)
        {
            Instantiate(deadVFX, transform.position, transform.rotation);
            gameObject.SetActive(false);
            isDead = true;
            deadUI.SetActive(true);
        }
    }
```

## 4. Enemies (EnemyBase) & AI/Attacks
Script: `Assets/Script/EnemyBase.cs`
- Patrol points: place several empty child objects under the enemy and assign them to `movePos[]`.
- Attack spawning: set `attackTrigger1` to the enemy attack trigger prefab and `attackPos1` to the spawn transform.
- World-space health bar: bind `Slider` to `hpBar`.

Creating attack trigger:
```startLine:endLine:filepath
121:136:Assets/Script/EnemyBase.cs
        if(playerInRange)
        {
            if(canAttack1)
            {
                CancelInvoke(nameof(Attack1End));
                var go = Instantiate(attackTrigger1, attackPos1.position, attackPos1.rotation, attackPos1);
                go.GetComponent<EnemyAttackTrigger>().SetDamage(ATK + Random.Range(0, 5),transform);
                canAttack1 = false;
                Invoke(nameof(Attack1End), 0.4f);
            }
        }
```

Taking damage & drops:
```startLine:endLine:filepath
59:88:Assets/Script/EnemyBase.cs
    public virtual void TakeDamage(int damage,Transform owner,int level)
    {
        trace = true;
        anim.SetBool("IsGetHit", true);
        isGetHit = true;
        Vector2 dir = (transform.position - owner.position).normalized;
        rb.linearVelocity = dir *  level;
        Invoke(nameof(GetHitAnimEnd), 0.25f);
        HPNow -= damage;
        hpBar.gameObject.SetActive(true);
        hpBar.value = (float)HPNow / (float)HP;
        Invoke(nameof(HPBarHide), 2.0f);
        if(HPNow <= 0)
        {
            Instantiate(deadVFX, transform.position, transform.rotation);
            for(int i = 0; i < dropItem.Length; i++)
            {
                if (Random.Range(0, 100) < 50)
                { Instantiate(dropItem[i], transform.position + new Vector3(0.5f*i,0.5f*i,0), transform.rotation); }
            }
            Destroy(gameObject);
        }
    }
```

## 5. Attack Triggers & Pickups
Player hits enemy:
```startLine:endLine:filepath
15:29:Assets/Script/PlayerAttackTrigger.cs
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Enemy"))
        {
            collision.gameObject.GetComponent<EnemyBase>().TakeDamage(damage, owner , attackLevel);
        }
    }
```

Enemy hits player:
```startLine:endLine:filepath
19:29:Assets/Script/EnemyAttackTrigger.cs
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Player"))
        {
            collision.gameObject.GetComponent<Player>().TakeDamage(damage, owner);
            Destroy(gameObject);
        }
```

Coin/Heart magnet and settlement:
```startLine:endLine:filepath
32:41:Assets/Script/Coin.cs
    public void FlyToPlayer()
    {
        rb.linearVelocity = (player.transform.position - transform.position).normalized * flySpeed;
        if (Vector2.Distance(transform.position, player.transform.position) < 0.8f)
        {
            player.GetComponent<Player>().ADDCoin(1);
            Destroy(gameObject);
        }
    }
```

## 6. UI & Fades
Cross-scene fades:
```startLine:endLine:filepath
20:27:Assets/Script/CanvasMain.cs
    public void StartClip()
    {
        Anim.SetTrigger("StartClip");
    }
    public void EndClip()
    {
        Anim.SetTrigger("EndClip");
    }
```

Main menu start button (onClick):
```startLine:endLine:filepath
21:43:Assets/Script/StartGameCanvas.cs
public void StartGameButton()
{
    if (CanvasMain.Instance)
    { CanvasMain.Instance.EndClip(); }
    gameObject.SetActive(false);
    Invoke("JumpToScene1", 1f);
}
```

Death UI restart/back to main:
```startLine:endLine:filepath
7:29:Assets/Script/DeadUI.cs
public void RestGameButton()
{
    if (CanvasMain.Instance)
    { CanvasMain.Instance.EndClip(); }
    Invoke(nameof(JumpToScene1), 1.0f);
}
```

## 7. Scenes & Transitions
Build order:
```startLine:endLine:filepath
1:23:ProjectSettings/EditorBuildSettings.asset
  - enabled: 1
    path: Assets/Scenes/StartScene.unity
  - enabled: 1
    path: Assets/Scenes/Game1Scene1.unity
  - enabled: 1
    path: Assets/Scenes/Game1Scene2.unity
  - enabled: 1
    path: Assets/Scenes/Game1Scene3.unity
  - enabled: 1
    path: Assets/Scenes/Game1Scene4.unity
```

Trigger-based scene jump:
```startLine:endLine:filepath
19:31:Assets/Script/JumpScene.cs
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
```

## 8. 10-Minute Rebuild Checklist
- StartScene: place `Player`, `CanvasMain`, `StartGameCanvas`, `DeadUI`; bind all fields as above.
- Game1Scene1: place `EnemyAndPos` (or enemy + patrol points), Tilemap (separate solid vs trigger).
- Run: main menu → fade out → level movement/combat/death/revive full loop.

## 9. Unity 6 Beginner Notes
- Physics-based motion: prefer `Rigidbody2D.linearVelocity`, avoid directly setting `transform.position` for movement.
- Animations: `IsRun/IsGetHit` Bools and `Attack1` Trigger must match script names and state transitions.
- Collisions: attack bodies use `Is Trigger`; at least one side has `Rigidbody2D`; ensure Layer collision matrix allows interaction.
- Singletons across scenes: `Player/CanvasMain` use `DontDestroyOnLoad`; avoid duplicates across scenes.

## 10. Deploying on Raspberry Pi 5 (Linux)
Raspberry Pi 5 is ARM64 (aarch64). Unity doesn’t provide an official Linux ARM64 Player. The recommended approach is deploying a WebGL build on the Pi (via Nginx/static hosting). Three paths:

- Preferred (recommended): WebGL build + Nginx (run locally in the Pi’s browser or via LAN)
- Alternative (unstable): run x86_64 Linux/Windows desktop builds via Box64/Wine/FEX (compatibility not guaranteed)
- Server (non-client): Headless Server (server logic only, no rendering)

### 10.1 WebGL on Raspberry Pi (Recommended)
1) Unity build settings (in your project):
   - `File > Build Settings > WebGL`, Add Open Scenes, Build.
   - Player Settings > Publishing Settings:
     - Compression Format: `Gzip` or `Brotli`.
     - Enable `Decompression Fallback` (works even without server config).
   - Player Settings > Other Settings:
     - Graphics API: WebGL 2.0.
     - Multithreading (if enabled): requires COOP/COEP headers (see Nginx below). If inconvenient, disable multithreading and rebuild.

2) Install Nginx on Raspberry Pi OS (64-bit):
```bash
sudo apt update && sudo apt install -y nginx
```

3) Deploy the build output:
```bash
sudo mkdir -p /var/www/html/game
sudo rsync -avz /path/to/YourWebGLBuild/ /var/www/html/game/
```

4) Nginx config (WASM/MIME/compression/multithread headers):
```bash
sudo tee /etc/nginx/sites-available/unity_webgl <<'EOF'
server {
    listen 80;
    server_name _;
    root /var/www/html/game;
    index index.html;

    types {
        application/wasm wasm;
        application/octet-stream data mem
        application/javascript js
    }

    gzip on;
    gzip_static on;
    gzip_types application/javascript application/wasm text/plain text/css application/json;

    location / {
        try_files $uri $uri/ /index.html;
        add_header Cross-Origin-Opener-Policy same-origin always;
        add_header Cross-Origin-Embedder-Policy require-corp always;
    }

    location ~* \.(data|mem|wasm|js|json)$ {
        add_header Access-Control-Allow-Origin *;
    }
}
EOF
sudo ln -sf /etc/nginx/sites-available/unity_webgl /etc/nginx/sites-enabled/unity_webgl
sudo nginx -t && sudo systemctl reload nginx
```

5) Access on the Pi:
- Open `http://localhost/` or `http://<Pi_IP>/` in the browser.
- Kiosk mode (autostart fullscreen Chromium):
```bash
sudo apt install -y chromium-browser
mkdir -p ~/.config/autostart
cat > ~/.config/autostart/kiosk.desktop <<'EOF'
[Desktop Entry]
Type=Application
Exec=chromium-browser --kiosk --app=http://localhost/
X-GNOME-Autostart-enabled=true
Name=Unity WebGL Kiosk
EOF
```

6) Performance & quality tips (Pi 5):
- Lower initial resolution (template/CSS) or use canvas scaling.
- Disable shadows/reduce particles; lower URP quality/post-processing.
- In code: `Application.targetFrameRate = 30;`.

7) Troubleshooting:
- White screen / stall: verify Nginx serves 200 OK (not 404); check browser console.
- `wasm` MIME error: ensure `application/wasm` type is configured.
- Multithreading errors: need COOP/COEP headers; otherwise disable multithreading and rebuild.
- Compression issues: if server headers aren’t configured, enable Unity `Decompression Fallback` and rebuild.

### 10.2 Box64/Wine/FEX for Desktop Builds (Not Guaranteed)
- No official Linux ARM64 Player. Box64 (x86_64 translation) or Wine for x86_64 builds may work, but results vary (graphics/input/audio). Use only if WebGL doesn’t meet your needs and you accept the risks.

### 10.3 Headless Server (Server Logic Only)
- If you need a non-rendered server process on Pi, build Linux Headless.
- Clients should still be WebGL (browser) or another native platform.

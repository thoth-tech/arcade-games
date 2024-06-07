#include "splashkit.h"

void update_camera(const sprite &target)
{

    center_camera_on(target, 10, -150);

    // Ensure camera stays within the bounds of the game world
    if (camera_x() < 0)
    {
        set_camera_x(0);
    }
    if (camera_y() < 0)
    {
        set_camera_y(0);
    }
}

void draw_portal(sprite spr, double x, double y)
{
    sprite_set_x(spr, x);
    sprite_set_y(spr, y);
    draw_sprite(spr);
}

void draw_ground(sprite spr, double startx, double starty, double cameraX, double cameraY)
{
    double x = 0; 
    while (x - cameraX < screen_width() + 2000)
    {
        sprite_set_x(spr, x - cameraX);
        sprite_set_y(spr,starty -cameraY);
        draw_sprite(spr);
        x = x + sprite_width(spr);
    }
}

int main()
{
    // screen data
    window game = open_window("Mario", 1000, 600);
    bitmap bg = load_bitmap("bG", "background.jpg");
    sprite bg_sprite = create_sprite(bg);

    // gound data 
    bitmap ground_bitmap = load_bitmap("ground", "ground4.png");
    sprite ground_sprite = create_sprite(ground_bitmap);
    sprite_set_x(ground_sprite, 0);
    double ground_y =  window_height(game) - bitmap_height(ground_bitmap);
    sprite_set_y(ground_sprite, ground_y);

    // character data
    double char_ground = ground_y -32;

    // air ground data
    bitmap air_ground_bitmap = load_bitmap("air_ground", "groundAir.png");
    sprite air_ground = create_sprite(air_ground_bitmap);
    sprite_set_x(air_ground, 600);
    double air_ground_y =  char_ground - 50;
    sprite_set_y(air_ground, air_ground_y);

    // player data
    bitmap player = load_bitmap("playerBmp", "protSpriteSheet.png");
    bitmap_set_cell_details(player, 31, 32, 4, 3, 12);
    animation_script player_anim_script = load_animation_script("player_run_r_anim", "playerRunR.txt");
    animation_script player_idle_anim_script = load_animation_script("playIdle", "playerIdle.txt");
    animation player_run_r_anim = create_animation(player_anim_script, "runR");
    animation player_run_l_anim = create_animation(player_anim_script, "runL");
    animation playIdle = create_animation(player_idle_anim_script, "idle");
    sprite playerIdle = create_sprite(player, player_idle_anim_script);
    sprite playerRunR = create_sprite(player, player_anim_script);
    sprite_start_animation(playerRunR, "runr");
    sprite_set_x(playerIdle, 100);
    sprite_set_y(playerIdle, char_ground);
    sprite_set_x(playerRunR, 100);
    sprite_set_y(playerRunR, char_ground);

    // zombie data
    bitmap zomb = load_bitmap("zombieBmp", "zombieSpriteSheet.png");
    bitmap_set_cell_details(zomb, 31, 32, 4, 3, 12);
    animation_script zombAnimS = load_animation_script("zombRun", "playerRunR.txt");
    animation zombAnim = create_animation(zombAnimS, "runL");
    sprite zombRun = create_sprite(zomb, zombAnimS);
    sprite_set_x(zombRun, 500);
    sprite_set_y(zombRun, char_ground);
    sprite_start_animation(zombRun, "runl");

    // coin data
    bitmap coin1 = load_bitmap("coins", "coinspritesheet.png");
    bitmap_set_cell_details(coin1, 64, 63, 8, 1, 8);
    animation_script coinScript = load_animation_script("coinAnimationScript", "coinAnimationScript.txt");
    animation coinAnim = create_animation(coinScript, "coin");
    sprite coinSprite = create_sprite(coin1, coinScript);
    sprite_start_animation(coinSprite, "coin");
    sprite_set_x(coinSprite, 500);
    sprite_set_y(coinSprite, char_ground - 20);
    sound_effect coinSound = load_sound_effect("coinCollected", "coinCollected.wav");

    // portal data
    bitmap port = load_bitmap("port", "portals.png");
    bitmap_set_cell_details(port, 32,32,4,5,17);
    animation_script portScript = load_animation_script("portalAnimationScript", "portalScript.txt");
    animation portalAnim = create_animation(portScript, "portal");
    sprite portal = create_sprite(port, portScript);
    sprite_start_animation(portal,"portal");
    sprite_set_x(portal, 800);
    sprite_set_y(portal, char_ground);

    // endGate Data
    bitmap end = load_bitmap("end", "endGate.png");
    bitmap_set_cell_details(end, 32,31,5,1,5);
    animation_script endScript = load_animation_script("endGateAnimationScript", "endGateScript.txt");
    animation endAnim = create_animation(endScript, "end");
    sprite endGate = create_sprite(end, endScript);
    sprite_start_animation(endGate,"end");
    sprite_set_x(endGate, 1050);
    sprite_set_y(endGate, char_ground); 

    
    bool isMoving = false;
    bool isJumping = false;
    bool collected = false;

    // jumping velocity
    vector_2d vec = vector_to(1, -1.25);
    // gravity velocity
    vector_2d gravity = vector_to(0, 0.02);
    // zombie velocity
    vector_2d zombVel = vector_to(-2, 0);
    // normal velocity
    vector_2d groundVel = vector_to(0, 0);

    // score
    int score = 0;
    string scoreString;

    while (!quit_requested())
    {
        clear_screen(COLOR_BLACK);
        update_camera(playerRunR);
        draw_ground(bg_sprite,0,0, camera_x(), camera_y());
        draw_sprite(air_ground, -camera_x(), -camera_y());
        draw_portal(portal, 600 -camera_x(), char_ground);
        draw_sprite(endGate, -camera_x(), -camera_y());
        draw_sprite(playerRunR);
        
        if (sprite_offscreen(zombRun))
        {
            sprite_set_x(zombRun, screen_width() - 10);
            sprite_set_y(zombRun, char_ground);
        }
        else
        {
            draw_sprite(zombRun);
        }
        
        if (!collected)
        {
            draw_sprite(coinSprite);
        }
        draw_ground(ground_sprite,0,ground_y, camera_x(), camera_y());
        
        string scoreString = "Score: " + std::to_string(score);
        draw_text(scoreString, COLOR_BLACK, 500 - 25, 10);
        refresh_screen(60);

        if (!collected)
        {
            update_sprite_animation(coinSprite);
            update_sprite(coinSprite);
        }

        update_sprite_animation(portal);
        update_sprite(portal);
        update_sprite_animation(endGate);
        update_sprite(endGate);
        sprite_set_velocity(zombRun, zombVel);
        update_sprite_animation(playerRunR);
        update_sprite(playerRunR);
        update_sprite_animation(zombRun);
        update_sprite(zombRun);
        update_sprite_animation(playerIdle);
        update_sprite(playerIdle);
        process_events();

        if (key_down(RIGHT_KEY))
        {
            if (sprite_animation_name(playerRunR) != "runr")
            {
                sprite_start_animation(playerRunR, "runr");
            }

            sprite_set_x(playerRunR, sprite_position(playerRunR).x + 1.5);
            sprite_set_x(playerIdle, sprite_position(playerIdle).x + 1.5);
            isMoving = true;
        }
        else if (key_down(LEFT_KEY))
        {
            if (sprite_animation_name(playerRunR) != "runl")
            {
                sprite_start_animation(playerRunR, "runl");
            }

            sprite_set_x(playerRunR, sprite_position(playerRunR).x - 1.5);
            sprite_set_x(playerIdle, sprite_position(playerIdle).x - 1.5);
            isMoving = true;
        }
        else if (key_typed(UP_KEY))
        {
            sprite_set_velocity(playerRunR, vec);
            isJumping = true;

        }
        else if (!isMoving)
        {
            sprite_start_animation(playerIdle, "idle");
        }

        if (isJumping)
        {

            // add the gravity to the player's velocity
            sprite_set_velocity(playerRunR, vector_add(sprite_velocity(playerRunR), gravity));

            if (sprite_position(playerRunR).y > char_ground)
            {
                sprite_set_velocity(playerRunR, groundVel);
                sprite_set_y(playerRunR, char_ground);
                isJumping = false;
            }
        }

        if (bitmap_collision(player, sprite_position(playerRunR), zomb, sprite_position(zombRun)))
        {
            clear_screen(COLOR_BLACK);
            draw_text("GAME OVER", COLOR_WHITE, 300, 300);
            refresh_screen();
            delay(5000);
            close_window(game);
        }

        if (sprite_collision(playerRunR,coinSprite))
        {
            free_sprite(coinSprite);
            play_sound_effect("coinCollected");
            score++;
            collected = true;
            
        }

        bool onAir = false;
        if(sprite_collision(playerRunR, portal))
        {
            int charAirGround = air_ground_y - 31;
            move_sprite_to(playerRunR, sprite_position(playerRunR).x, charAirGround);
            onAir = true;
        }

        if(sprite_position(playerRunR).x > 600 && onAir)
        {
            move_sprite_to(playerRunR, sprite_position(playerRunR).x, char_ground);
            onAir = false;
        }
    }

    return 0;
}



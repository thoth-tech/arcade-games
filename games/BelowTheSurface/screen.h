#include "splashkit.h"
#include "level.h"
#include "cellsheet.h"
#include "get_level.h"
#include "texteffect.h"
#include "button.h"
#include "password.h"
#include <memory>
#include <vector>

class Screen;

class ScreenState
{
    protected:
        Screen *screen;
        string screen_state;

    public:
        virtual ~ScreenState()
        {};

        void set_state(Screen *screen, string screen_state)
        {
            this->screen = screen;
            this->screen_state = screen_state;
        };

        string get_type()
        {
            return this->screen_state;
        };

        virtual void update() = 0;
};

class Screen
{
    private:
        ScreenState *state;
        int tile_size;
        int players = 1;
        vector<CellSheet> cell_sheets;
        vector<string> files;

    public:
        key_code input_key = F_KEY;
        key_code pause_key = H_KEY;
        int level_number = 1;
        int max_levels = 10;
        shared_ptr<Level> current_level;

        Screen(ScreenState *state, int tile_size, vector<CellSheet> cell_sheets, vector<string> files) : state(nullptr)
        {
            this->cell_sheets = cell_sheets;
            this->tile_size = tile_size;
            this->files = files;
            this->change_state(state, "Initial");
        };

        ~Screen()
        {
            delete state;
        };

        void change_state(ScreenState *new_state, string type)
        {
            if (this->state != nullptr)
                delete this->state;
            this->state = new_state;
            this->state->set_state(this, type);
        };

        void update()
        {
            this->state->update();
        };

        int get_tile_size()
        {
            return this->tile_size;
        };

        int get_players()
        {
            return this->players;
        };

        void set_players(int num)
        {
            this->players = num;
        };

        vector<CellSheet> get_cell_sheets()
        {
            return this->cell_sheets;
        };

        vector<string> get_files()
        {
            return this->files;
        };
};

class CompanyIntroScreen : public ScreenState
{
    private:
        bool run_once = false;
        double alpha = 1.0;
        int screen_time = 5;

    public:
        CompanyIntroScreen(){};

        ~CompanyIntroScreen(){};

        void update() override;
};

class TeamIntroScreen : public ScreenState
{
    private:
        bool run_once = false;
        double alpha = 1.0;
        int screen_time = 7;

    public:
        TeamIntroScreen(){};

        ~TeamIntroScreen(){};

        void update() override;
};

class MenuScreen : public ScreenState
{
    private:
        bool run_once = false;
        vector<shared_ptr<Button>> menu_buttons;
        int offset = -80;
        int num_buttons = 5;
        int selection = 0;

    public:
        MenuScreen(){};

        ~MenuScreen(){};

        void update() override;
};

class ExtraScreen : public ScreenState
{
    private:
        bool run_once = false;
        vector<shared_ptr<Button>> menu_buttons;
        int offset = -40;
        int num_buttons = 3;
        int selection = 0;

    public:
        ExtraScreen(){};

        ~ExtraScreen(){};

        void update() override;
};

class PreLevelScreen : public ScreenState
{
    private:
        bool run_once = false;
        shared_ptr<TextEffect> text_effect;
        bitmap image;

    public:
        PreLevelScreen(){};

        ~PreLevelScreen(){};

        void update() override;
};

class PasswordScreen : public ScreenState
{
    private:
        bool run_once = false;
        bool stage_1 = true;
        int num_buttons = 3;
        int offset = -100;
        vector<shared_ptr<Button>> menu_buttons;
        int selection = 0;
        shared_ptr<Password> password_screen;

    public:
        PasswordScreen(){};

        ~PasswordScreen(){};

        void update() override;
};

class LevelScreen : public ScreenState
{
    private:
        bool run_once = false;
        bool pause = false;
        bool pause_run_once;
        int num_buttons = 2;
        int offset = 0;
        vector<shared_ptr<Button>> menu_buttons;
        int selection = 0;

    public:
        LevelScreen(){};

        ~LevelScreen(){};

        void update() override;

        // Inputs for testing functions
        void testing_input()
        {
            if (!pause)
            {
                if (key_typed(NUM_1_KEY))
                {
                    if (this->screen->level_number < this->screen->max_levels)
                    {
                        this->screen->level_number += 1;
                        this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
                        this->screen->change_state(new PreLevelScreen, "Pre Level");
                    }
                }

                if (key_typed(NUM_2_KEY))
                {
                    if (this->screen->level_number > 1)
                    {
                        this->screen->level_number -= 1;
                        this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
                        this->screen->change_state(new PreLevelScreen, "Pre Level");
                    }
                }
            }
        };
};

class GameOverScreen : public ScreenState
{
    private:
        bool run_once = false;
        int num_buttons = 2;
        int offset = 300;
        vector<shared_ptr<Button>> menu_buttons;
        int selection = 0;

    public:
        GameOverScreen(){};

        ~GameOverScreen(){};

        void update() override;
};

class WinScreen : public ScreenState
{
    private:
        bool run_once = false;
        int num_buttons = 2;
        int offset = 320;
        vector<shared_ptr<Button>> menu_buttons;
        int selection = 0;

    public:
        WinScreen(){};

        ~WinScreen(){};

        void update() override;
};

class CreditsScreen : public ScreenState
{
    private:
        bool run_once = false;

    public:
        CreditsScreen(){};

        ~CreditsScreen(){};

        void update() override;
};

class BackstoryScreen : public ScreenState
{
    private:
        bool run_once = false;
        int current = 0;
        int max_screens = 5;

    public:
        BackstoryScreen(){};

        ~BackstoryScreen(){};

        void update() override;
        string name()
        {
            if (current == 0)
            {
                return "MenubgDark";
            }
            return "backstory" + std::to_string(current) + ".png";
        };
};

double fade_in(double alpha, double fade_length)
{ 
    if(alpha > 0)
        alpha -= (fade_length/180);
    
    fill_rectangle(rgba_color(0.0,0.0,0.0,alpha),0,0,1600,896,option_to_screen());

    return alpha;
}

double fade_out(double alpha, double fade_length)
{ 
    if(alpha < 1)
        alpha += (fade_length/150);
    
    fill_rectangle(rgba_color(0.0,0.0,0.0,alpha),0,0,1600,896,option_to_screen());

    return alpha;
}

double screen_effect(double alpha, int time_length, string timer_name, double fade_length)
{
    int time = timer_ticks(timer_name) / 1000;

    if(time < fade_length)
        alpha = fade_in(alpha, fade_length);
    else if(time > (time_length - fade_length))
        alpha = fade_out(alpha, fade_length);

    return alpha;
}

bool screen_timer(int time_length, string timer_name)
{
    if(!timer_started(timer_named(timer_name)))
        start_timer(timer_name);
    
    int time = timer_ticks(timer_name) / 1000;

    if(time > time_length)
    {
        stop_timer(timer_name);
        reset_timer(timer_name);
        return true;
    }
    else
        return false;
}

void draw_buttons(vector<shared_ptr<Button>> menu_buttons, int selection)
{
    for(int i = 0; i < menu_buttons.size(); i++)
    {
        if(menu_buttons[i]->get_id() == selection)
            menu_buttons[i]->set_selected(true); 
        else
            menu_buttons[i]->set_selected(false); 

        menu_buttons[i]->draw();
    }
}

int button_selection(int selection, int num_buttons)
{
    if(key_typed(UP_KEY) || key_typed(W_KEY))
    {
        selection -= 1;

        if(selection < 0)
            selection = 0;
    }
    if(key_typed(DOWN_KEY) || key_typed(S_KEY))
    {
        selection += 1;

        if(selection > num_buttons - 1)
            selection = num_buttons - 1;
    }

    return selection;
}

void CompanyIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    
    bitmap title = bitmap_named("Company1");
    bitmap title2 = bitmap_named("Company2");
    font screen_font = font_named("DefaultFont");
    int font_size = 80;
    color font_color = COLOR_WHITE;
    string text = "Thoth Tech presents";

    draw_bitmap(title2, pt.x - bitmap_width(title2)/2 + 5, pt.y - bitmap_height(title2)/2 - 5, option_to_screen());
    draw_bitmap(title, pt.x - bitmap_width(title)/2, pt.y - bitmap_height(title)/2, option_to_screen());

    draw_text(text, COLOR_BROWN, screen_font, font_size, pt.x- text_width(text, screen_font, font_size)/2 + 5, (pt.y - text_height(text, screen_font, font_size)/2) + 200 - 5, option_to_screen());
    draw_text(text, font_color, screen_font, font_size, pt.x- text_width(text, screen_font, font_size)/2, (pt.y - text_height(text, screen_font, font_size)/2) + 200, option_to_screen());

    bool time_up = screen_timer(screen_time, "ScreenTimer");

    alpha = screen_effect(alpha, screen_time, "ScreenTimer", 2);

    if(time_up)
        this->screen->change_state(new TeamIntroScreen, "TeamIntro");

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        stop_timer("ScreenTimer");
        reset_timer("ScreenTimer");
        this->screen->change_state(new TeamIntroScreen, "TeamIntro");
    }
}

void TeamIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);

    bitmap logo = bitmap_named("TeamLogo");
    font screen_font = font_named("DefaultFont");
    int font_size = 30;
    color font_color = COLOR_WHITE;
    string text = "Morgaine Barter";
    string text2 = "Daniel Agbay, Lily Lan, Robert Osborne";
    string text3 = "Jiahao Zheng, Roy Chen";
    string text4 = "And";
    string text5 = "Lachlan Morgan";
    string text6 = "Present";

    draw_bitmap(logo, pt.x - bitmap_width(logo)/2, pt.y - bitmap_height(logo)/2 - 150, option_to_screen());

    draw_text(text, font_color, screen_font, font_size, pt.x- text_width(text, screen_font, font_size)/2, (pt.y - text_height(text, screen_font, font_size)/2) + 150, option_to_screen());
    draw_text(text2, font_color, screen_font, font_size, pt.x- text_width(text2, screen_font, font_size)/2, (pt.y - text_height(text2, screen_font, font_size)/2) + 150 + text_height(text2, screen_font, font_size) * 1, option_to_screen());
    draw_text(text3, font_color, screen_font, font_size, pt.x- text_width(text3, screen_font, font_size)/2, (pt.y - text_height(text3, screen_font, font_size)/2) + 150 + text_height(text3, screen_font, font_size) * 2, option_to_screen());
    draw_text(text4, font_color, screen_font, font_size, pt.x- text_width(text4, screen_font, font_size)/2, (pt.y - text_height(text4, screen_font, font_size)/2) + 150 + text_height(text4, screen_font, font_size) * 3, option_to_screen());
    draw_text(text5, font_color, screen_font, font_size, pt.x- text_width(text5, screen_font, font_size)/2, (pt.y - text_height(text5, screen_font, font_size)/2) + 150 + text_height(text5, screen_font, font_size) * 4, option_to_screen());
    draw_text(text6, font_color, screen_font, font_size, pt.x- text_width(text6, screen_font, font_size)/2, (pt.y - text_height(text6, screen_font, font_size)/2) + 150 + text_height(text6, screen_font, font_size) * 6, option_to_screen());

    bool time_up = screen_timer(screen_time, "ScreenTimer");

    alpha = screen_effect(alpha, screen_time, "ScreenTimer", 2);

    if(time_up)
        this->screen->change_state(new MenuScreen, "Menu");

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        stop_timer("ScreenTimer");
        reset_timer("ScreenTimer");
        this->screen->change_state(new MenuScreen, "Menu");
    }
}

string get_button_text(int id)
{
    string text = "";

    switch (id)
    {
        case 1:
            text = "1 PLAYER";
            break;
        case 2:
            text = "2 PLAYER";
            break;
        case 3:
            text = "PASSWORD";
            break;
        case 4:
            text = "EXTRAS";
            break;
        case 5:
            text = "EXIT";
            break;
        default:
            break;
    }

    return text;
}

string get_extras_button_text(int id)
{
    string text = "";

    switch (id)
    {
        case 1:
            text = "BACKSTORY";
            break;
        case 2:
            text = "CREDITS";
            break;
        case 3:
            text = "BACK";
            break;
        default:
            break;
    }

    return text;
}

void MenuScreen::update()
{
    set_camera_x(0);
    set_camera_y(0);

    if(!run_once)
    {
        //stop_music();
        for(int i = 0; i < num_buttons; i++)
        {
            string text = get_button_text(i + 1);
            shared_ptr<Button> test(new Button(bitmap_named("Button"), offset, i, text));
            offset += 100;
            menu_buttons.push_back(test);
        }
        run_once = true;
    }

    if (!music_playing())
    {
        play_music("MenuMusic.mp3"); 
        set_music_volume(0.2f);
    }

    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_bitmap("MenuBg", 0, 0, option_to_screen());

    bitmap title = bitmap_named("Title");
    drawing_options scale = option_scale_bmp(2, 2);
    draw_bitmap(title, pt.x - bitmap_width(title)/2, 100, scale);

    draw_buttons(menu_buttons, selection);
    selection = button_selection(selection, num_buttons);

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        switch(selection)
        {
            case 0:
                {
                    play_sound_effect("Select");
                    this->screen->set_players(1);
                    stop_music();
                    this->screen->change_state(new PreLevelScreen, "Pre Level");
                }
                break;
            case 1:
                {
                    play_sound_effect("Select");
                    this->screen->set_players(2);
                    stop_music();
                    this->screen->change_state(new PreLevelScreen, "Pre Level");
                }
                break;
            case 2:
                {
                    play_sound_effect("Select");
                    this->screen->change_state(new PasswordScreen, "Password");
                }
            break;
            case 3:
                {
                    play_sound_effect("Select");
                    this->screen->change_state(new ExtraScreen, "Extras");
                }
            break;
                case 4:
                {
                    exit(0);
                }
                break;
            default:
                break;
        }
    }
}

void ExtraScreen::update()
{
    set_camera_x(0);
    set_camera_y(0);

    if(!run_once)
    {
        for(int i = 0; i < num_buttons; i++)
        {
            string text = get_extras_button_text(i + 1);
            shared_ptr<Button> test(new Button(bitmap_named("Button"), offset, i, text));
            offset += 100;
            menu_buttons.push_back(test);
        }
        run_once = true;
    }

    if (!music_playing())
    {
        play_music("MenuMusic.mp3"); 
        set_music_volume(0.2f);
    }

    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_bitmap("MenuBg", 0, 0, option_to_screen());

    bitmap title = bitmap_named("Title");
    drawing_options scale = option_scale_bmp(2, 2);
    draw_bitmap(title, pt.x - bitmap_width(title)/2, 100, scale);
    draw_buttons(menu_buttons, selection);
    selection = button_selection(selection, num_buttons);

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        switch(selection)
        {
            case 0:
                {
                    play_sound_effect("Select");
                    this->screen->change_state(new BackstoryScreen, "Backstory");
                }
                break;
            case 1:
                {
                    play_sound_effect("Select");
                    this->screen->change_state(new CreditsScreen, "Credits");
                }
                break;

            case 2:
                {
                    play_sound_effect("Select");
                    this->screen->change_state(new MenuScreen, "Main Menu");
                }
                break;
            default:
                break;
        }
    }
}

void PreLevelScreen::update()
{
    font screen_font = font_named("DefaultFont");
    point_2d pt = screen_center();
    int font_size = 40;
    int font_size_chapter = 30;
    int font_size_password = 15;
    int font_size_side_text = 20;
    string chapter_text = "";
    if(this->screen->level_number < 40)
        chapter_text = "Chapter " + std::to_string(this->screen->level_number);
    else
        chapter_text = "Chapter ???";
    
    if(!run_once)
    {
        stop_music();
        if(this->screen->get_files().size() != 0)
        {
            shared_ptr<Level> custom_level(new BlankLevel(this->screen->get_cell_sheets(),this->screen->get_tile_size(),this->screen->get_players(),this->screen->get_files().size(),this->screen->get_files()));
            this->screen->current_level = custom_level;
            this->screen->max_levels = 1;
        }
        else
        {
            this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
        }

        image = this->screen->current_level->get_pre_level_image();

        vector<string> side_text = this->screen->current_level->get_pre_level_text();

        shared_ptr<TextEffect> temp(new TextEffect(side_text, screen_center().x - bitmap_width(image)/2, 740, screen_font, font_size_side_text));
        text_effect = temp;

        run_once = true;
    }

    string level_text = "- " + this->screen->current_level->get_level_name() + " -";
    string password = "Password: " + this->screen->current_level->get_level_password();

    clear_screen(COLOR_BLACK);
    draw_bitmap("MenubgDark", 0, 0, option_to_screen());
    draw_text(chapter_text, COLOR_WHITE, screen_font, font_size_chapter, pt.x - text_width(chapter_text, screen_font, font_size)/2, 20);
    draw_text(level_text, COLOR_WHITE, screen_font, font_size, pt.x - text_width(level_text, screen_font, font_size)/2, 100);
    draw_text(password, COLOR_WHITE, screen_font, font_size_password, pt.x - text_width(password, screen_font, font_size_password)/2, screen_height() - 30);

    draw_bitmap(image, pt.x - bitmap_width(image)/2, pt.y - bitmap_height(image)/2 + 10, option_to_screen());

    text_effect->update();

    bool time_up = screen_timer(5, "ScreenTimer");

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        stop_timer("ScreenTimer");
        reset_timer("ScreenTimer");
        this->screen->change_state(new LevelScreen, "Level");
    }

    if(time_up)
        this->screen->change_state(new LevelScreen, "Level");
}

string get_pause_text(int id)
{
    string text = "";

    switch (id)
    {
        case 1:
            text = "CONTINUE";
            break;
        case 2:
            text = "MENU";
            break;
        default:
            break;
    }

    return text;
}

void LevelScreen::update()
{
    if(!run_once)
    {
        for(int i = 0; i < num_buttons; i++)
        {
            string text = get_pause_text(i + 1);
            shared_ptr<Button> test(new SmallButton(bitmap_named("ButtonSmall"), offset, i, text, 20, COLOR_BLACK, COLOR_RED));
            offset += 80;
            menu_buttons.push_back(test);
        }

        run_once = true;
    }
    if(!pause)
    {
        this->screen->current_level->update();

        if(this->screen->current_level->player1_complete && this->screen->current_level->player2_complete)
        {
            if(!timer_started("DanceTime"))
                start_timer("DanceTime");
            int time = timer_ticks("DanceTime")/1000; // Changed to int, was u_int
            if(time > 2)
            {
                stop_timer("DanceTime");
                if(this->screen->level_number < this->screen->max_levels)
                {
                    stop_music();
                    this->screen->level_number += 1;
                    this->screen->change_state(new PreLevelScreen, "Pre Level");
                }
                else
                {
                    stop_music();
                    this->screen->change_state(new WinScreen, "Win");
                }
            }
        }
        else
        {
            if(this->screen->current_level->is_player1_out_of_lives || this->screen->current_level->is_player2_out_of_lives)
            {
                stop_music();
                this->screen->level_number = 1;
                this->screen->current_level = get_next_level(this->screen->level_number,this->screen->get_cell_sheets(),this->screen->get_tile_size(),this->screen->get_players());
                this->screen->change_state(new GameOverScreen, "GameOver");
            }
        }

        if(key_typed(RETURN_KEY) || key_typed(screen->pause_key))
        {
            pause_run_once = false;
            pause = true;
            pause_music();
        }
    }
    else
    {
        point_2d pt = to_screen(screen_center());
        string text = "-Game Paused-";
        color font_color = COLOR_WHITE;
        font screen_font = font_named("DefaultFont");
        int font_size = 40;
        if(!pause_run_once)
        {
            fill_rectangle(rgba_color(0,0,0,50), 0, 0, screen_width(), screen_height(), option_to_screen());
            pause_run_once = true;
        }
        draw_text(text, font_color, screen_font, font_size, pt.x- text_width(text, screen_font, font_size)/2, 300, option_to_screen());
        draw_buttons(menu_buttons, selection);
        selection = button_selection(selection, num_buttons);
        
        if(key_typed(RETURN_KEY) || key_typed(screen->pause_key))
        {
            switch(selection)
            {
                case 0:
                    {
                        pause = false;
                        resume_music();
                        break;
                    }
                case 1:
                    {
                        stop_music();
                        this->screen->level_number = 1;
                        this->screen->current_level = get_next_level(this->screen->level_number,this->screen->get_cell_sheets(),this->screen->get_tile_size(),this->screen->get_players());
                        this->screen->change_state(new MenuScreen, "Menu");
                        break;
                    }
                default:
                    break; 
            }
        }
        

    }

    testing_input();
}

string get_gameover_text(int id)
{
    string text = "";

    switch (id)
    {
        case 1:
            text = "RETRY";
            break;
        case 2:
            text = "MENU";
            break;
        default:
            break;
    }

    return text;
}

void GameOverScreen::update()
{
    set_camera_x(0);
    set_camera_y(0);
    clear_screen(COLOR_BLACK);
    draw_bitmap("MenubgDark", 0, 0, option_to_screen());
    
    if (!run_once)
    {
        stop_music();
        for(int i = 0; i < num_buttons; i++)
        {
            string text = get_gameover_text(i + 1);
            shared_ptr<Button> test(new SmallButton(bitmap_named("ButtonDark"), offset, i, text, 20, COLOR_WHITE, COLOR_RED));
            offset += 60;
            menu_buttons.push_back(test);
        }
        if (!sound_effect_playing("GameOver"))
            play_sound_effect("GameOver");
        
        run_once = true;
    }

    if(!sound_effect_playing("GameOver"))
    {
        if(!sound_effect_playing("Rain"))
            play_sound_effect("Rain");
    }
        
    point_2d pt = screen_center();
    string game_over_text = "Game Over";
    font screen_font = font_named("DefaultFont");
    int font_size = 80;
    color font_color = COLOR_WHITE_SMOKE;

    draw_text(game_over_text, font_color, screen_font, font_size, pt.x - text_width(game_over_text, screen_font, font_size)/2, (pt.y - text_height(game_over_text, screen_font, font_size)/2) - 300, option_to_screen());

    bitmap game_over = bitmap_named("GameOver");
    //fill_rectangle(COLOR_WHITE_SMOKE, pt.x - bitmap_width(game_over)/2 - 10, pt.y - bitmap_height(game_over)/2 - 10, bitmap_width(game_over) + 20, bitmap_height(game_over) + 20);
    draw_bitmap(game_over, pt.x - bitmap_width(game_over)/2, pt.y - bitmap_height(game_over)/2, option_to_screen());

    draw_buttons(menu_buttons, selection);
    selection = button_selection(selection, num_buttons);

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        if (sound_effect_playing("GameOver"))
            stop_sound_effect("GameOver");
        if (sound_effect_playing("Rain"))
            stop_sound_effect("Rain");
        stop_music();
        switch(selection)
        {
            case 0:
                {
                    this->screen->change_state(new PreLevelScreen, "PreLevel");
                    break;
                }
            case 1:
                {
                    this->screen->change_state(new MenuScreen, "Menu");
                    break;
                }
            default:
                break; 
        }
    }
}

string get_win_text(int id)
{
    string text = "";

    switch (id)
    {
        case 1:
            text = "PLAY AGAIN";
            break;
        case 2:
            text = "MENU";
            break;
        default:
            break;
    }

    return text;
}

void WinScreen::update()
{
    set_camera_x(0);
    set_camera_y(0);
    if (!run_once)
    {
        stop_music();
        if (!sound_effect_playing("GameWin"))
            play_sound_effect("GameWin");
        for(int i = 0; i < num_buttons; i++)
        {
            string text = get_win_text(i + 1);
            shared_ptr<Button> test(new SmallButton(bitmap_named("ButtonSmall"), offset, i, text, 20, COLOR_BLACK, COLOR_RED));
            offset += 60;
            menu_buttons.push_back(test);
        }
        run_once = true;
    }

    point_2d pt = screen_center();
    string win_screen_text1 = "YOU WON";
    string win_screen_text2 = "GOOD JOB";
    font screen_font = font_named("DefaultFont");
    int font_size = 80;
    bitmap win_bitmap = bitmap_named("WinScreen");
    color font_color = COLOR_WHITE_SMOKE;

    clear_screen(COLOR_BLACK);
    draw_bitmap("MenubgDark", 0, 0, option_to_screen());

    draw_text(win_screen_text1, font_color, screen_font, font_size, pt.x - text_width(win_screen_text1, screen_font, font_size)/2, (pt.y - text_height(win_screen_text1, screen_font, font_size)/2) - 350, option_to_screen());
    draw_bitmap(win_bitmap, pt.x - bitmap_width(win_bitmap)/2, 130, option_to_screen());
    draw_text(win_screen_text2, font_color, screen_font, font_size - 20, pt.x - text_width(win_screen_text2, screen_font, font_size - 20)/2, (pt.y - text_height(win_screen_text2, screen_font, font_size - 20)/2) + 180, option_to_screen());

    draw_buttons(menu_buttons, selection);
    selection = button_selection(selection, num_buttons);

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        this->screen->level_number = 1;
        this->screen->current_level = get_next_level(this->screen->level_number,this->screen->get_cell_sheets(),this->screen->get_tile_size(),this->screen->get_players());
        if (!sound_effect_playing("GameWin"))
            stop_sound_effect("GameWin");
        stop_music();
        switch(selection)
        {
            case 0:
                {
                    this->screen->change_state(new PreLevelScreen, "PreLevel");
                    break;
                }
            case 1:
                {
                    this->screen->change_state(new MenuScreen, "Menu");
                    break;
                }
            default:
                break; 
        }
    }
}

void CreditsScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_bitmap("MenubgDark", 0, 0, option_to_screen());

    bitmap logo = bitmap_named("TeamLogo");
    font screen_font = font_named("DefaultFont");
    color font_color = COLOR_WHITE;
    int font_size = 25;
    vector<string> credits;

    if (!music_playing())
    {
        play_music("MenuMusic.mp3"); 
        set_music_volume(0.2f);
    }

    credits.push_back("Technical Lead: Lachlan Morgan");
    credits.push_back("Delivery Lead: Morgaine Barter");
    credits.push_back("Game Design: Daniel Agbay, Morgaine Barter");
    credits.push_back("Lead Developer: Lachlan Morgan");
    credits.push_back("Developers: Lily Lan, Daniel Agbay, Morgaine Barter");
    credits.push_back("            Robert Osborne, Jiahao Zheng, Roy Chen");
    credits.push_back("Game Artwork: Lily Lan, Morgaine Barter, Daniel Agbay");
    credits.push_back("Game Music: Robert Osborne");
    credits.push_back("Sound Design: Lily Lan");
    credits.push_back("Additional Artwork: Lachlan Morgan");

    draw_bitmap(logo, pt.x - bitmap_width(logo)/2, pt.y - bitmap_height(logo)/2 - 200, option_to_screen());
    draw_text("CREDITS", font_color, screen_font, 80, pt.x- text_width("CREDITS", screen_font, 80)/2, (pt.y - text_height("CREDITS", screen_font, 80)/2) - 350, option_to_screen());

    int height = (text_height(credits[0], screen_font, font_size) * 2) + 13;

    for(int i = 0; i < credits.size(); i++)
        draw_text(credits[i], font_color, screen_font, font_size, 150, (pt.y + (i * height/2)) + 30, option_to_screen());

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
        this->screen->change_state(new MenuScreen, "Menu");
}

void BackstoryScreen::update()
{
    font screen_font = font_named("DefaultFont");
    int font_size = 80;
    color font_color = COLOR_WHITE_SMOKE;

    if (!music_playing())
    {
        play_music("MenuMusic.mp3"); 
        set_music_volume(0.2f);
    }

    if (screen_timer(8, "BackstoryTimer"))
    {
        clear_screen(COLOR_BLACK);
        current++;
        //current %= 5;
    }
    
    if(current == 0)
    {
        point_2d pt = screen_center();
        draw_bitmap(this->name(), 0, 0, option_to_screen());
        draw_text("Backstory", font_color, screen_font, font_size, pt.x- text_width("Backstory", screen_font, font_size)/2, (pt.y - text_height("Backstory", screen_font, font_size)/2) - 200, option_to_screen());
    }
    else if (current > max_screens - 1)
        this->screen->change_state(new MenuScreen, "Menu");
    else
        draw_bitmap(this->name(), 0, 0, option_to_screen());

    if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
    {
        current++;
        if(current > max_screens - 1)
            this->screen->change_state(new MenuScreen, "Menu");
    }
}

void play_sounds()
{
    if (!sound_effect_playing("Select"))
        play_sound_effect("Select");
    stop_music();
}

void enter_level(int level_number, Screen* screen)
{
    screen->level_number = level_number;
    play_sounds();
    screen->change_state(new PreLevelScreen, "Pre Level");
}

string get_password_text(int id)
{
    string text = "";

    switch (id)
    {
        case 1:
            text = "1 PLAYER";
            break;
        case 2:
            text = "2 PLAYER";
            break;
        case 3:
            text = "MENU";
            break;
        default:
            break;
    }

    return text;
}

void PasswordScreen::update()
{
    if(!run_once)
    {
        shared_ptr<Password> pass(new Password);
        password_screen = pass;
        for(int i = 0; i < num_buttons; i++)
        {
            string text = get_password_text(i + 1);
            shared_ptr<Button> test(new SmallButton(bitmap_named("ButtonDark"), offset, i, text, 20, COLOR_BLACK, COLOR_RED));
            offset += 80;
            menu_buttons.push_back(test);
        }
        run_once = true;
    }

    if (!music_playing())
    {
        play_music("MenuMusic.mp3"); 
        set_music_volume(0.2f);
    }

    if(stage_1)
    {
        clear_screen(COLOR_BLACK);
        draw_bitmap("MenubgDark", 0, 0, option_to_screen());
        draw_buttons(menu_buttons, selection);
        selection = button_selection(selection, num_buttons);

        if(key_typed(RETURN_KEY) || key_typed(screen->input_key))
        {
            play_sound_effect("Select");
            switch(selection)
            {
                case 0:
                    {
                        this->screen->set_players(1);
                        stage_1 = false;
                        break;
                    }
                case 1:
                    {
                        this->screen->set_players(2);
                        stage_1 = false;
                        break;
                    }
                case 2:
                    {
                        this->screen->change_state(new MenuScreen, "Menu");
                        break;
                    }
                default:
                    break; 
            }
        }

    }
    else
    {
        string password = password_screen->update();

        if(password == "EXITEXITEXIT")
        {
            play_sound_effect("Select");
            this->screen->change_state(new MenuScreen, "Menu");
        }
        else if(password == "START")
        {
            enter_level(1, screen);
        }
        else if(password == "MULTI")
        {
            enter_level(2, screen);
        }
        else if(password == "EZPZ_")
        {
            enter_level(3, screen);
        }
        else if(password == "CLIMB")
        {
            enter_level(4, screen);
        }
        else if(password == "JUMPW")
        {
            enter_level(5, screen);
        }
        else if(password == "TEMPL")
        {
            enter_level(6, screen);
        }
        else if(password == "MOUSE")
        {
            enter_level(7, screen);
        }
        else if(password == "SURFN")
        {
            enter_level(8, screen);
        }
        else if(password == "FIGHT")
        {
            enter_level(9, screen);
        }
        else if(password == "BOSS_")
        {
            enter_level(10, screen);
        }     
        else if(password == "ROACH")
        {
            enter_level(40, screen);
        }
        else if(password == "MARIO")
        {
            enter_level(50, screen);
        }
    }
}

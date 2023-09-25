#include "splashkit.h"
#include "game.h"
#include "MainMenu.h"
#include <iostream>

using namespace std;

void load_resources()
{
    load_resource_bundle("game_bundle", "icons.txt");
    load_music("game", "game.wav");
    load_music("intro", "intro.mp3");
    open_audio();
}

int main()
{
    open_window("VentureAdventure", 672, SCREEN_HEIGHT);
    window_toggle_border("VentureAdventure");
    load_resources();

    game_data game;
    game = new_game();

    MainMenu mainMenu;

    bool quitGame = false;

    while (!quitGame) {
        system("clear");

        mainMenu.Render();
        cout << "Select an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Starting the game...\n";
            while (true) {
                if (GetUserInput() == 'esc') {
                    break;
                }
            }
            break;

        case 2:
            cout << "Entering level select...\n";
            break;

        case 3:
            mainMenu.ShowHowToPlay();
            break;

        case 4:
            quitGame = true;
            cout << "Quitting the game...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }

        if (choice != 1 && choice != 2 && choice != 3 && choice != 4) {
            delay(1000);
    }

    while (!quit_requested() && !key_down(ESCAPE_KEY))
    {
        play_music("intro");
        set_music_volume(0.04);

        start_screen();

        fade_music_out(1000);

        play_music("game", 100);
        set_music_volume(0.025);

        while (!quit_requested() && !key_down(ESCAPE_KEY))
        {
            process_events();

            clear_screen();

            draw_game(game);

            update_game(game);

            if (game.player.attacked == true)
                draw_text("Game Over", COLOR_BLANCHED_ALMOND, "font.ttf", 70, SCREEN_WIDTH / 2 - 138, SCREEN_HEIGHT / 2 - 48, option_to_screen());

            start_debug(game);

            refresh_screen(60);
        }
    }

    return 0;
}

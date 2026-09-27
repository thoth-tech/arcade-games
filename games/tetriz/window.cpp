#include "define.h"
#include "window.h"
#include "draw.h"
#include "game.h"
#include "terminal.h"
#include "utils.h"
#include "control.h"

namespace ui
{

    void window_init()
    {
#ifdef _WIN32
        system("chcp 65001");
#endif

        tc::hide_cursor();
        tc::clean_screen();

        auto [r, c] = tc::get_size();
        tc::cols = c;
        tc::rows = r;
        tc::top = (tc::rows - 24) / 2;
        tc::left = (tc::cols - 29 * 2) / 2;
    }

    void window_exit()
    {
        tc::show_cursor();
        tc::reset_color();
    }

    void show_windows()
    {
        dw::window(1, 1, 9, 6, "Hold");
        dw::window(1, 10, 12, 24, "Tetriz");
        dw::window(7, 1, 9, 16, "Status");
        dw::window(19, 22, 8, 4, "Info");
        dw::window(1, 22, 8, 18, "Next");
    }
    void show_info()
    {
        std::ostringstream oss;
        tc::reset_color(oss);

        tc::move_to(10, 4, oss);
        oss << "Level:" <<std::setw(5)<< gm::level;
        tc::move_to(11, 4, oss);
        oss << "Score:" <<std::setw(5)<< gm::score;
        tc::move_to(12, 4, oss);
        oss << "Lines:" <<std::setw(5)<< gm::lines;

        tc::move_to(14, 4, oss);
        oss << "FPS:" << ut::fps();
        tc::move_to(15, 4, oss);

        if (gm::ending)
        {
            dw::window(9, 12, 8, 3, "", 2, (int)Color::Red, oss);
            tc::move_to(10, ut::b2c(13), oss);
            tc::set_fore_color((int)Color::Red, oss);
            oss << " Game Over!";
        }

        if (gm::pausing)
        {
            dw::window(9, 12, 8, 3, "", 2, (int)Color::White, oss);
            tc::move_to(10, ut::b2c(13), oss);
            oss << "   PAUSE!";
        }
        if (gm::helping)
        {
            tc::set_back_color((int)Color::White, oss);
            tc::set_fore_color((int)Color::Black, oss);

            tc::move_to(5, ut::b2c(12), oss);
            oss << "   Help Info   ";
            tc::move_to(6, ut::b2c(12), oss);
            oss << "                ";
            tc::move_to(7, ut::b2c(12), oss);
            oss << " Pause        [w] ";
            tc::move_to(8, ut::b2c(12), oss);
            oss << " Rotate-L     [t] ";
            tc::move_to(9, ut::b2c(12), oss);
            oss << " Rotate-2     [g] ";
            tc::move_to(10, ut::b2c(12), oss);
            oss << " Left         [a] ";
            tc::move_to(11, ut::b2c(12), oss);
            oss << " Right        [d] ";
            tc::move_to(12, ut::b2c(12), oss);
            oss << " Down         [s] ";
            tc::move_to(13, ut::b2c(12), oss);
            oss << " Drop         [f] ";
            tc::move_to(14, ut::b2c(12), oss);
            oss << " Hold         [y] ";
            tc::move_to(15, ut::b2c(12), oss);
            oss << " Reset        [r] ";
            tc::move_to(16, ut::b2c(12), oss);
            oss << " Help         [h] ";
            tc::move_to(17, ut::b2c(12), oss);
            oss << " Quit      [ESC] ";
        }
        std::cout << oss.str();
    }
    void show_game()
    {
        dw::frame(gm::frame, 2, 11);
        dw::next(gm::next, 2, 23);
        dw::hold(gm::hold_piece, 2, 2);
        std::cout << std::flush;
    }
    void show_exit()
    {
        tc::clean_screen();
        dw::window(1, 1, 18, 3, "", 2, (int)Color::Red);
        tc::move_to(2, ut::b2c(2));
        tc::set_fore_color((int)Color::Blue);
        std::cout << "  Thank you for playing ";
        tc::set_fore_color((int)Color::Purple);
        tc::set_blod();
        std::cout << "Tetriz";
        tc::reset_color();
        tc::set_fore_color((int)Color::Blue);
        std::cout << "!" << std::endl<<std::endl;
        
    }
    void show_help()
    {
        tc::reset_color();
        tc::move_to(21, ut::b2c(24));
        std::cout << "Help [h]";
    }
    void window_resize()
    {
        auto [r, c] = tc::get_size();
        if (tc::rows == r && tc::cols == c)
            return;

        tc::cols = c;
        tc::rows = r;
        tc::top = (tc::rows - 24) / 2;
        tc::left = (tc::cols - 29 * 2) / 2;

        tc::clean_screen();
        ui::show_windows();
        ui::show_help();
        gm::reseting = true;
    }
} // namespace ui
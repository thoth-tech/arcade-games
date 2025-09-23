#include "control.h"
#include "define.h"
#include "game.h"
namespace gm
{
    char command;

    std::map<char, std::function<void()>> comm_func{
        {KEY_ESCAPE, command_quit},
        {KEY_W, command_pause},
        {KEY_S, command_down},
        {KEY_A, command_left},
        {KEY_D, command_right},
        {KEY_R, command_reset},
        {KEY_F, command_drop},
        {KEY_T, command_rotate_L},
        {KEY_G, command_rotate_2},
        {KEY_Y, command_hold},
        {KEY_H, command_help},
    };
#ifdef __linux__
    char getch()
    {
        char c;
        struct termios old, cur;
        tcgetattr(0, &cur);
        old = cur;
        cfmakeraw(&cur);
        tcsetattr(0, 0, &cur);
        c = getchar();
        tcsetattr(0, 0, &old);
        return c;
    }
#endif

    void key_event()
    {
        while (running)
        {
            command = getch();
            if (comm_func.find(command) != comm_func.end())
                comm_func[command]();
        }
    }

    void start_listener()
    {
        static std::jthread t(key_event);
    }

    void command_quit()
    {
        quit();
    }

    void command_rotate_R()
    {
        rotate(1);
    }
    void command_rotate_L()
    {
        rotate(3);
    }
    void command_rotate_2()
    {
        rotate(2);
    }

    void command_left()
    {
        left();
    }

    void command_right()
    {
        right();
    }

    void command_down()
    {
        down();
    }

    void command_drop()
    {
        drop();
    }

    void command_hold()
    {
        hold();
    }

    void command_reset()
    {
        reset();
    }

    void command_help()
    {
        help();
    }

    void command_pause()
    {
        pause();
    }

} // namespace gm
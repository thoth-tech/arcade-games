
#include "control.h"
#include "define.h"
#include "game.h"
namespace gm
{
    char command;

    std::map<char, std::function<void()>> comm_func{
        {KEY_Q, command_quit},
        {KEY_ESCAPE, command_quit},
        {KEY_W, command_rotate_R},
        {KEY_UP, command_rotate_R},
        {KEY_Z, command_rotate_L},
        {KEY_X, command_rotate_2},
        {KEY_S, command_down},
        {KEY_DOWN, command_down},
        {KEY_A, command_left},
        {KEY_LEFT, command_left},
        {KEY_D, command_right},
        {KEY_RIGHT, command_right},
        {KEY_C, command_hold},
        {KEY_R, command_reset},
        {KEY_H, command_help},
        {KEY_SPACE, command_drop},
        {KEY_P, command_pause},

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
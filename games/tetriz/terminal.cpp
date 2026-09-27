#include "terminal.h"
#include "define.h"

#define CSI "\033["

namespace tc
{

    int cols, rows;
    int top, left;

    void move_to(int row, int col, std::ostream &os)
    {
        os << CSI << top + row << ';' << left + col << 'H';
    }

    void set_fore_color(int id, std::ostream &os)
    {
        os << CSI << "38;5;" << id << 'm';
    }

    void set_back_color(int id, std::ostream &os)
    {
        os << CSI << "48;5;" << id << 'm';
    }

    void clean_screen(std::ostream &os)
    {
        os << CSI << "2J";
    }

    void reset_color(std::ostream &os)
    {
        os << CSI << "0m";
    }

    void hide_cursor(std::ostream &os)
    {
        os << CSI << "?25l";
    }

    void show_cursor(std::ostream &os)
    {
        os << CSI << "?25h";
    }

    void set_blod(std::ostream &os)
    {
        os << CSI << "1m";
    }

    void set_underline(std::ostream &os)
    {
        os << CSI << "4m";
    }

    std::pair<int, int> get_size()
    {
        int columns = 0, rows = 0;
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#elif __linux__
        struct winsize w;
        ioctl(fileno(stdout), TIOCGWINSZ, &w);
        columns = (int)(w.ws_col);
        rows = (int)(w.ws_row);
#endif

        return {rows, columns};
    }

} // namespace tc
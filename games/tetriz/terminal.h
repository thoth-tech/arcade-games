#pragma once
#include "define.h"

namespace tc
{ // terminal control

    //====================================
    // 屏幕长宽rows,cols
    extern int cols, rows;
    // 起点坐标top,left
    extern int top, left;
    //====================================
    void move_to(int row, int col, std::ostream &os = std::cout);
    void set_fore_color(int id, std::ostream &os = std::cout);
    void set_back_color(int id, std::ostream &os = std::cout);
    void clean_screen(std::ostream &os = std::cout);
    void reset_color(std::ostream &os = std::cout);
    void hide_cursor(std::ostream &os = std::cout);
    void show_cursor(std::ostream &os = std::cout);
    void set_blod(std::ostream &os = std::cout);
    void set_underline(std::ostream &os = std::cout);
    std::pair<int,int> get_size();
}
#pragma once
#include "define.h"
#include "tetromino.h"

namespace dw
{
    void window(int top, int left, int width, int height, std::string title="",int style=0,int color=(int)Color::White,std::ostream& os=std::cout);
    void frame(Matrix& frame,int top,int left);
    void next(std::queue<Tetromino> next5,int top,int left);
    void hold(Tetromino &h, int top, int left);
    //-----------------------------------------------------------
    /**
     * style: 风格标签：
     * 0. 默认风格，无风格
     * 1. 白点打底
     * 2. 下划线
     * 
    */
    void matrix(Matrix& m,int top,int left,Matrix* buffer=nullptr,int style=0);

} // namespace dw

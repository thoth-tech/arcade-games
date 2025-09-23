#pragma once
#include "tetromino.h"
#include "define.h"

namespace gm
{
    class Piece
    {
    public:
        Piece(Tetromino &t, int x0, int y0, int i);
        Piece() = default;
        bool down();
        bool left();
        bool right();
        bool rotate(int i);//1:R  2:180  3:L
        std::pair<int, int> get_mino(int i) const;
        std::pair<int, int> get_xy() const;
        int get_color() const;
        bool test(int x, int y) const;
        void set_ghost();
        void set_disable();
        Tetromino get_tetromino() const;
    private:
        bool move(int dx,int dy);
        char get_type() const;

        Tetromino tetro_set;
        int index; // [0 R 2 L]
        int x, y;  // 正交直角坐标系
        std::shared_ptr<Matrix> sp_playfield;
        int status; //0：阴影， 1：正常  2：不可用
        Offset offset;//踢墙表
    };
} // namespace gm

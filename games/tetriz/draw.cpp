
#include "draw.h"
#include "terminal.h"
#include <iostream>
#include "utils.h"
#include "piece.h"
#include "game.h"
/**
 *  	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
U+250x	─	━	│	┃	┄	┅	┆	┇	┈	┉	┊	┋	┌	┍	┎	┏
U+251x	┐	┑	┒	┓	└	┕	┖	┗	┘	┙	┚	┛	├	┝	┞	┟
U+252x	┠	┡	┢	┣	┤	┥	┦	┧	┨	┩	┪	┫	┬	┭	┮	┯
U+253x	┰	┱	┲	┳	┴	┵	┶	┷	┸	┹	┺	┻	┼	┽	┾	┿
U+254x	╀	╁	╂	╃	╄	╅	╆	╇	╈	╉	╊	╋	╌	╍	╎	╏
U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟
U+256x	╠	╡	╢	╣	╤	╥	╦	╧	╨	╩	╪	╫	╬	╭	╮	╯
U+257x	╰	╱	╲	╳	╴	╵	╶	╷	╸	╹	╺	╻	╼	╽	╾	╿
 *
*/

namespace dw
{
    //                              0123456
    const std::u32string style1 = U" ┌┐└┘│─";
    const std::u32string style2 = U" ╔╗╚╝║═";
    const std::u32string style3 = U" ┏┓┗┛┃━";
    const std::u32string style4 = U" ╭╮╰╯│─";
    std::u32string cur_style = style4;

    std::map<int, std::u32string> map_style{
        {1, style1},
        {2, style2},
        {3, style3},
        {4, style4},
    };

    //          1,10,12,22
    void window(int top, int left, int width, int height, std::string title, int style, int color, std::ostream &os)
    {
        if (style)
            cur_style = map_style[style];

        tc::set_fore_color(color, os);
        for (int r = 0; r < height; ++r)
        {
            tc::move_to(top + r, ut::b2c(left), os);
            for (int c = 0; c < width; ++c)
            {
                if (r == 0) // 第一行
                {
                    if (c == 0) // 第一列
                    {
                        os << ut::utf32_to_utf8({cur_style[0], cur_style[1]});
                    }
                    else if (c == width - 1) // 最后一列
                    {
                        os << ut::utf32_to_utf8({cur_style[2]});
                    }
                    else
                    {
                        os << ut::utf32_to_utf8({cur_style[6], cur_style[6]}); // 2个—
                    }
                }
                else if (r == height - 1) // 最后一行
                {
                    if (c == 0) // 第一列
                    {
                        os << ut::utf32_to_utf8({cur_style[0], cur_style[3]});
                    }
                    else if (c == width - 1) // 最后一列
                    {
                        os << ut::utf32_to_utf8({cur_style[4]});
                    }
                    else
                    {
                        os << ut::utf32_to_utf8({cur_style[6], cur_style[6]}); // 2个—
                    }
                }
                else
                {
                    if (c == 0) // 第一列
                    {
                        os << ut::utf32_to_utf8({cur_style[0], cur_style[5]});
                    }
                    else if (c == width - 1) // 最后一列
                    {
                        os << ut::utf32_to_utf8({cur_style[5]});
                    }
                    else
                    {
                        os << "  ";
                    }
                }
            }
        }

        // title
        tc::move_to(top, ut::b2c(left) + (width * 2 - title.length()) / 2, os);
        os << title;
    }

    void frame(Matrix &frame, int top, int left)
    {
        static Matrix buffer(frame.size(), std::vector<int>(frame[0].size(), -1));
        static Matrix buffer_top2(2, std::vector<int>(frame[0].size(), -1));

        if (gm::reseting)
        {
            buffer = Matrix(frame.size(), std::vector<int>(frame[0].size(), -1));
            buffer_top2 = Matrix(2, std::vector<int>(frame[0].size(), -1));
        }

        Matrix f(frame.begin(), frame.begin() + 20);
        matrix(f, top + 2, left, &buffer, 1);

        Matrix v(frame.begin() + 20, frame.end());
        matrix(v, top, left, &buffer_top2, 2);
    }

    void next(std::queue<Tetromino> next5, int top, int left)
    {
        static Matrix buffer(15, std::vector<int>(6, -1));
        if (gm::reseting)
        {
            buffer = Matrix(15, std::vector<int>(6, -1));
        }
        Matrix next_field(15, std::vector<int>(6, 0));
        for (int y = 12; next5.size() > 0; y -= 3)
        {
            gm::Piece p(next5.front(), 2, y, 0);
            gm::merge(next_field, p);
            next5.pop();
        }

        matrix(next_field, top, left, &buffer);
    }

    void hold(Tetromino &h, int top, int left)
    {
        static Matrix buffer(4, std::vector<int>(7, -1));
        if (gm::reseting)
        {
            buffer = Matrix(4, std::vector<int>(7, -1));
        }
        Matrix hold_field(4, std::vector<int>(7, 0));
        if (!h.empty())
        {
            gm::Piece p(h, 3, 1, 0);
            if (gm::holding)
            {
                p.set_disable();
            }
            gm::merge(hold_field, p);
            matrix(hold_field, top, left, &buffer);
        }
    }

    void matrix(Matrix &m, int top, int left, Matrix *buffer, int style)
    {
        std::string blank = "  ";
        if (style == 1)
            blank = "\u30fb";
        std::ostringstream oss;
        // frame xy ----> row/col
        int row, col;
        for (int y = 0; y < m.size(); ++y)
        {
            for (int x = 0; x < m[0].size(); ++x)
            {
                if (buffer != nullptr)
                {
                    if ((*buffer)[y][x] == m[y][x])
                        continue;
                    (*buffer)[y][x] = m[y][x];
                }

                row = top + m.size() - y - 1;
                col = left + x;
                tc::move_to(row, ut::b2c(col), oss);
                if (m[y][x] > 0) // 正常块
                {
                    tc::reset_color(oss);
                    tc::set_back_color(m[y][x], oss);
                    oss << "  ";
                }
                else if (m[y][x] < 0) // 阴影块
                {
                    tc::reset_color(oss);
                    tc::set_fore_color(0 - m[y][x], oss);
                    oss << "\u25e3\u25e5";
                }
                else // 空白区
                {
                    tc::reset_color(oss);
                    if (style == 2 && y == 0)
                        tc::set_underline(oss);
                    oss << blank; // "\u30FB";
                }
            }
        }
        std::cout << oss.str();
    }

} // namespace dw
#include "game.h"
#include "tetromino.h"
#include "utils.h"
#include "control.h"
namespace gm
{
    //---------------变量定义区---------------
    bool locking;
    bool running;
    bool holding;
    bool ending;
    bool reseting;
    bool helping;
    bool pausing;
    Piece one_piece;
    Matrix playfield;
    std::chrono::microseconds duration;
    Matrix frame;
    std::queue<Tetromino> next;
    std::vector<Tetromino> bag;
    Tetromino hold_piece;
    int score, level, lines;
    std::chrono::microseconds time;
    //-----------------------------------
    void init()
    {
        srand(std::time(0));
        locking = false;
        running = true;
        holding = false;
        ending = false;
        reseting = false;
        helping = false;
        pausing = false;
        score = lines = 0;
        levelup();
        playfield = Matrix(22, std::vector<int>(10, 0));
        load();
        bag.clear();
        preview();
        one_piece = pick();
        frame = playfield;
        hold_piece.clear();
        time = 0ms;
    }

    void process()
    {
        if (ending || pausing)
            return;

        render();
        if (ut::timer(duration))
        {
            // 如果无法继续下落，就锁定在游戏区
            if (one_piece.down())
                return;

            if (locking)
            {
                // 锁定
                lock();
                // 消行
                clear();
                levelup();
                one_piece = pick();
                locking = false;
                holding = false;
                reseting = false;
            }
            else
            {
                locking = true;
            }
        }
    }

    void render()
    {
        frame = playfield;
        // 正常块
        merge(frame, one_piece);
        // 阴影块
        Piece ghost = one_piece;
        ghost.set_ghost();
        while (ghost.down())
            ;

        merge(frame, ghost);
    }
    Piece pick()
    {
        assert(next.size() > 0);
        Piece p(next.front(), 4, 20, 0);
        next.pop();

        if (!p.test(4, 20))
        {
            // game over!
            ending = true;
        }

        preview();

        return std::move(p);
    }
    void lock()
    {
        merge(playfield, one_piece);
    }
    void clear()
    {
        int count = 0;
        for (auto it = playfield.begin(); it != playfield.end();)
        {
            bool full = true;
            for (auto cell : *it)
            {
                if (cell == 0)
                {
                    full = false;
                    break;
                }
            }
            if (full)
            {
                // 消行 [1,2,3,4] -->[1,3,4]
                it = playfield.erase(it);
                playfield.push_back(std::vector<int>(it->size(), 0));
                count++;
            }
            else
                ++it;
        }
        /**
        Single	100 x level	100 x level
        Double	300 x level	300 x level
        Triple	500 x level	500 x level
        Tetris	800 x level	800 x level
        Soft drop	1 point per cell	1 point per cell
        Hard drop	2 points per cell	2 point per cell
        */
        if (count == 1)
            score += 100 * level;
        if (count == 2)
            score += 300 * level;
        if (count == 3)
            score += 500 * level;
        if (count == 4)
            score += 800 * level;
        lines += count;
    }
    void quit()
    {
        running = false;
    }
    void rotate(int i)
    {
        if (pausing)
            return;
        one_piece.rotate(i);
    }

    void left()
    {
        if (pausing)
            return;
        one_piece.left();
    }

    void right()
    {
        if (pausing)
            return;
        one_piece.right();
    }

    void down()
    {
        if (pausing)
            return;
        if (one_piece.down())
            score += 1;
    }

    void drop()
    {
        if (pausing)
            return;
        while (one_piece.down())
            score += 2;
        locking = true;
        // 直落后马上生效
        duration = 0s;
    }
 
    void preview()
    {
        int index;
        while (next.size() < 5)
        {
            if (bag.size() == 0)
                bag = {I, J, L, O, S, T, Z};
            index = rand() % bag.size();
            next.push(bag[index]);
            bag.erase(bag.begin() + index);
        }
    }

    void load()
    {
        std::ifstream fs("tetriz.map");
        assert(fs.is_open());
        std::string line;
        // 22*10
        for (auto &row : playfield | std::ranges::views::take(20) | std::ranges::views::reverse)
        {
            getline(fs, line);
            for (auto i : iota(0, 10))
            {
                if (line[i] == '1')
                {
                    row[i] = (int)Color::Gray;
                }
            }
        }
        fs.close();
    }

    void hold()
    {
        if (pausing)
            return;
        if (holding)
            return;

        if (hold_piece.empty())
        {
            hold_piece = one_piece.get_tetromino();
            one_piece = pick();
        }
        else
        {
            auto tmp = hold_piece;
            hold_piece = one_piece.get_tetromino();
            one_piece = Piece(tmp, 4, 20, 0);
        }

        holding = true;
    }

    void levelup()
    {
        // 每消除10行 升1级
        level = lines / 10 + 1;
        // Time = (0.8-((Level-1)*0.007))^(Level-1)
        duration = std::chrono::milliseconds(int(pow((0.8 - ((level - 1) * 0.007)), level - 1) * 1000));
    }

    void reset()
    {
        init();
        reseting = true;
    }

    void help()
    {
        helping = !helping;
        reseting = !helping;
        pausing = helping;
    }

    void pause()
    {
        pausing = !pausing;
        reseting = !pausing;
    }

    void merge(Matrix &m, const Piece &p)
    {
        auto [x, y] = p.get_xy();
        for (auto i : iota(0, 4))
        {
            auto [dx, dy] = p.get_mino(i);
            if (m[y + dy][x + dx] == 0)
                m[y + dy][x + dx] = p.get_color();
        }
    }

} // namespace gm
#pragma once
#include "tetromino.h"
#include "piece.h"
namespace gm
{

    //=======================================
    // 游戏变量定义
    //=======================================
    // 游戏运行状态
    extern bool running;
    // 锁定标志
    extern bool locking;
    // 暂存标志
    extern bool holding;
    // 重置标志
    extern bool reseting;
    // 终结标志
    extern bool ending;
    // 帮助标志
    extern bool helping;
    // 暂停标志
    extern bool pausing;
    // 当前掉落的块
    extern Piece one_piece;
    // 游戏场地
    extern Matrix playfield;
    // 每帧时间间隔
    extern std::chrono::microseconds duration;
    // 当前渲染帧
    extern Matrix frame;
    // 5格预览队列
    extern std::queue<Tetromino> next;
    // 方块口袋
    extern std::vector<Tetromino> bag;
    // 暂存块
    extern Tetromino hold_piece;
    // 计分、等级、消行
    extern int score, level, lines;
    // 总耗时
    extern std::chrono::microseconds time;
    //=======================================
    // 游戏逻辑
    //=======================================
    // 游戏主逻辑
    void process();
    // 游戏初始化
    void init();
    // 渲染当前帧
    void render();
    // 获取一个块
    Piece pick();
    // 锁定
    void lock();
    // 消行
    void clear();
    // 退出
    void quit();
    void rotate(int i);
    void left();
    void right();
    void down();
    // 直落
    void drop();
    // 生成预览队列
    void preview();
    // 载入预设地图
    void load();
    // 暂存
    void hold();
    // 升级
    void levelup();
    // 重置游戏
    void reset();
    // 帮助
    void help();
    // 暂停
    void pause();
    // 回显命令
    std::string echo();
    //-----------------------
    void merge(Matrix &m, const Piece &p);

} // namespace gm

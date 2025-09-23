
#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <map>
#include <functional>
#include <locale>
#include <codecvt>
#include <vector>
#include <memory>
#include <cassert>
#include <ranges>
#include <sstream>
#include <ostream>
#include <queue>
#include <fstream>
#include <cmath>
#include <iomanip>

#ifdef __linux__
#include <termios.h>
#include <sys/ioctl.h>
#elif _WIN32
#include <conio.h>
#include <windows.h>
#endif

using namespace std::chrono_literals;

#define KEY_Q 'q'
#define KEY_W 'w'
#define KEY_S 's'
#define KEY_A 'a'
#define KEY_D 'd'
#define KEY_Z 'z'
#define KEY_X 'x'
#define KEY_C 'c'
#define KEY_R 'r'
#define KEY_H 'h'
#define KEY_P 'p'


#ifdef __linux__
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67
#elif _WIN32
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#endif

#define KEY_ENTER 13
#define KEY_BACKSPACE 127
#define KEY_ESCAPE 27
#define KEY_SPACE 32

using Matrix = std::vector<std::vector<int>>;
using Tetromino = std::vector<std::vector<std::pair<int, int>>>;
using Offset = std::vector<std::vector<std::pair<int, int>>>;
using std::ranges::views::iota;

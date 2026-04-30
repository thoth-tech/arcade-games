#ifndef _TILE_H_
#define _TILE_H_

#include <vector>

#define SIZE_TILE 48//定义瓦片大小
struct Tile
{
	enum Direction//前进方向
	{
		None = 0,
		Up,
		Down,
		Left,
		Right
	};
	
	
	
	int terrian = 0;//选择瓦片样式
	int decoration = -1;//选择装饰物
	int special_flag = -1;//特殊标志，出生点，守护点
	bool has_tower = false;//标志有无防御塔
	Direction direction = Direction::None;
};

typedef std::vector<std::vector<Tile>> TileMap;//构建可拓展的二维数组




#endif // !_TILE_H_



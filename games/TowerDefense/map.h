#ifndef _Map_H_
#define _Map_H_

#include "tile.h"
#include"route.h"

#include <SDL.h>
#include<string>
#include<sstream>
#include<fstream>
#include<unordered_map>

class Map
{
public: typedef std::unordered_map<int, Route> SpawnerRoutePool;

public:
	Map() = default;

	~Map() = default;

	bool load(const std::string& path)//定义加载文件函数
	{

		std::ifstream file(path);//读取文件
		if (!file.good()) return false;//如果文件打不开，直接返回终止

		TileMap tile_map_tep;//定义二维数组tep版

		int idx_x = -1, idy_y = -1;//定义并初始化二维数组的索引

		std::string str_line;//定义读取csv文件行承载对象
		while (std::getline(file, str_line))
		{
			str_line = trim_str(str_line);//标准化行对象，使得没有制表符和空格
			if (str_line.empty())
				continue;

			idx_x = -1, idy_y++;//都从第一列开始读起，行数根据循环数自动累加,实现一行一行往下读的效果
			tile_map_tep.emplace_back();//往二维数组里面追加新一行

			std::string str_tile;//定义单元格承载
			std::stringstream str_stream(str_line);//字符串化行承载
			while (std::getline(str_stream, str_tile, ','))//进行getline操作，分隔符是，
			{
				idx_x++;//随着循环增加读取列数
				tile_map_tep[idy_y].emplace_back();//追加单元格承载
				Tile& tile = tile_map_tep[idy_y].back();//获得单元格承载
				load_tile_from_string(tile, str_tile);
			}
		}
		file.close();

		if (tile_map_tep.empty() || tile_map_tep[0].empty())
			return false;

		tilemap = tile_map_tep;//地图文件没有问题，将tep输入给正式版

		generate_map_cache();
		
		return true;
	}

	size_t get_width() const//获取地图数据宽度
	{
		if (tilemap.empty())
			return false;

		return tilemap[0].size();
	}

	size_t get_height() const//获取地图数据高度
	{
		return tilemap.size();
	}

	const TileMap& get_tile_map() const//对外暴露接口
	{
		return tilemap;
	}

	const SDL_Point& get_idx_home() const//对外暴露接口
	{
		return idx_home;
	}

	const SpawnerRoutePool& get_idx_spawner_pool() const//对外暴露接口
	{
		return spawner_route_pool;
	}

	void place_tower(const SDL_Point& idx_title)//对外暴露接口
	{
		tilemap[idx_title.y][idx_title.x].has_tower = true;
	}
private:
	TileMap tilemap;
	SDL_Point idx_home = { 0 };
	SpawnerRoutePool spawner_route_pool;
private:
	std::string trim_str(const std::string& str)//标准格式化字符串的方法
	{
		size_t begin_idx = str.find_first_not_of(" \t");
		if (begin_idx == std::string::npos)
			return "";
		size_t end_idx = str.find_last_not_of(" \t");
		size_t idx_range = end_idx - begin_idx + 1;
		return str.substr(begin_idx, idx_range);
	}

	void load_tile_from_string(Tile& tile, const std::string& str)
	{
		std::string str_tidy = trim_str(str);
		std::string str_value;
		std::vector<int> values;
		std::stringstream stringstream(str_tidy);
		while (std::getline(stringstream, str_value, '\\'))
		{
			int value;
			try {
				value = std::stoi(str_value);
			}
			catch (const std::invalid_argument)
			{
				value = -1;
			}
			values.push_back(value);
		}//value接收整数化后的字符串values，接收完一个以后往values推，直到推完。
		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
		tile.decoration = (values.size() < 2) ? -1 : values[1];
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
		tile.special_flag = (values.size() <= 3) ? -1 : values[3];
		//对输入值的异常处理
	}

	void generate_map_cache()
	{
		for (int y = 0; y < get_height(); y++) 
		{
		
			for(int x = 0;x < get_width();x++)
			{
				const Tile& tile = tilemap[y][x];
				if (tile.special_flag < 0)
					continue;
			//非特殊地点继续
				if (tile.special_flag == 0)
				{
					idx_home.x = x;
					idx_home.y = y;
				}
			//房屋守卫点记录
				else
				{
					spawner_route_pool[tile.special_flag] = Route(tilemap,{  x,y  });
				}
			}
				
		
		}
	
	}

};

	



#endif // !_Map_H_


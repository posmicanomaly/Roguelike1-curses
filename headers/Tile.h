#ifndef TILE_H
#define TILE_H

#include <vector>
#include <string>
using namespace std;

class Tile
{
public:
	int id;
	char base_sym;
	char sym;
	int y;
	int x;
	string item;
	string type;
	bool passable;
	bool check(int y, int x);
	bool HasNPC;
	bool HasPlayer;
	bool Visible;
	bool Explored;
	bool IsWall;
	Tile(int ID, int y, int x, string TILETYPE, string MESSAGE, string FLAG, string FLAG2);
	Tile(int ID, char sym, int y, int x, string type, bool passable, bool Bound);
	//new shit 2.0
	string TileType;
	string Message;
	string Flag;
	string Flag2;

	void SetSymFromTileType(string TileType);
};

#endif
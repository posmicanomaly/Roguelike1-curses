#ifndef MAP_2_H
#define MAP_2_H

#include <fstream>
#include <string>
#include <curses.h>
#include "..\headers\Tile.h"

class GameMap{
public:
	//Map();
	int LastTileScanned;
	string MapName;
	GameMap();
	GameMap(string FileName, ifstream &file_in);
	void DrawMap();
	int ScanTileByCoord(int y, int x);
	bool CheckCollision(int DestinationTile);
	bool CheckCollisionWithWall(int Dest);
	bool CheckCollisionWithNPC(int DestinationTile);
	bool CheckCollisionWithPlayer(int DestinationTile);
	
	bool Bresenham(int y1, int x1, int const y2, int const x2);

	bool BoundsRegistered;
	vector<Tile> TileMap;
	vector<int> Bounds;

	int StoredPlayerX;
	int StoredPlayerY;

	void StorePlayerPos(int &y, int &x);
	void LoadPlayerPos(int &y, int &x);

private:
	vector<string> RawMap;
};

#endif
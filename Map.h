#ifndef MAP_H
#define MAP_H

#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <curses.h>
#include "Tile.h"

class GameMap{
public:
	//Map();
	int LastTileScanned;
	string MapName;
	string MapStyle;
	GameMap();
	GameMap(string FileName, ifstream &file_in);
	void DrawMap();
	int ScanTileByCoord(int y, int x);
	bool CheckCollision(int DestinationTile);
	bool CheckCollisionWithWall(int Dest);
	bool CheckCollisionWithNPC(int DestinationTile);
	bool CheckCollisionWithPlayer(int DestinationTile);
	
	bool Bresenham(int y1, int x1, int const y2, int const x2);
	vector<int> BresenhamNPC(int y1, int x1, int const y2, int const x2);

	bool BoundsRegistered;
	vector<Tile> TileMap;
	vector<int> Bounds;

	int StoredPlayerX;
	int StoredPlayerY;

	void StorePlayerPos(int &y, int &x);
	void LoadPlayerPos(int &y, int &x);
	//need way to store mobs on a level by level basis.
	

private:
	vector<string> RawMap;
};

#endif
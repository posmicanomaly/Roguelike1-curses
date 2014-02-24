//#include "entity.h"
#include "..\headers\entity.h"

//Entity::Entity(int yc, int xc, GameMap &CurrentMap)
//{
//	y = yc;
//	x = xc;
//	CurrentTile = CurrentMap.ScanTileByCoord(y, x);
//	PrevTile = CurrentTile;
//	CurrentMap.TileMap[CurrentTile].HasNPC = true;
//
//	Alive = true;
//};

bool Entity::Move(string direction, GameMap &CurrentMap)
{
	if(direction == "up")
	{
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y - 1, x)))
		{
		case true://Collision returned true
			return false;//Deny any move
			break;
		case false:
			//remove NPC flag from previous tile and update flag for current tile
			int Temp_new = CurrentMap.LastTileScanned;
			int Temp_old = CurrentMap.ScanTileByCoord(y, x);
			CurrentMap.TileMap[Temp_old].HasNPC = false;
			CurrentMap.TileMap[Temp_new].HasNPC = true;
			y--;
			
			PrevTile = CurrentTile;
			CurrentTile = CurrentMap.LastTileScanned;
			HasMoved = true;
			return true;
			break;
		}
	}
	else if(direction == "down")
	{
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y + 1, x)))
		{
		case true:
			return false;
			break;
		case false:
			//remove NPC flag from previous tile and update flag for current tile
			int Temp_new = CurrentMap.LastTileScanned;
			int Temp_old = CurrentMap.ScanTileByCoord(y, x);
			CurrentMap.TileMap[Temp_old].HasNPC = false;
			CurrentMap.TileMap[Temp_new].HasNPC = true;
			y++;
			PrevTile = CurrentTile;
			CurrentTile = CurrentMap.LastTileScanned;
			HasMoved = true;
			return true;
			break;
		}
	}
	else if(direction == "left")
	{
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y, x - 1)))
		{
		case true:
			return false;
			break;
		case false:
			//remove NPC flag from previous tile and update flag for current tile
			int Temp_new = CurrentMap.LastTileScanned;
			int Temp_old = CurrentMap.ScanTileByCoord(y, x);
			CurrentMap.TileMap[Temp_old].HasNPC = false;
			CurrentMap.TileMap[Temp_new].HasNPC = true;
			x--;
			PrevTile = CurrentTile;
			CurrentTile = CurrentMap.LastTileScanned;
			HasMoved = true;
			return true;
			break;
		}
	}
	else if(direction == "right")
	{
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y, x + 1)))
		{
		case true:
			return false;
			break;
		case false:
			//remove NPC flag from previous tile and update flag for current tile
			int Temp_new = CurrentMap.LastTileScanned;
			int Temp_old = CurrentMap.ScanTileByCoord(y, x);
			CurrentMap.TileMap[Temp_old].HasNPC = false;
			CurrentMap.TileMap[Temp_new].HasNPC = true;
			x++;
			PrevTile = CurrentTile;
			CurrentTile = CurrentMap.LastTileScanned;
			HasMoved = true;
			return true;
			break;
		}
	}
}

void Entity::GetFOV(GameMap &CurrentMap)
{
	vector<Tile>::iterator it;
	//reset all to invis
	for(it = CurrentMap.TileMap.begin(); it != CurrentMap.TileMap.end(); it++)
	{
		(*it).Visible = false;
	}
	Circle.clear();
	GetCircle(CurrentMap);
	vector<int>::iterator nit;
	for(nit = Circle.begin(); nit != Circle.end(); nit++)
	{
		CurrentMap.Bresenham(y, x, CurrentMap.TileMap[(*nit)].y, CurrentMap.TileMap[(*nit)].x);
	}
}
		
void Entity::GetCircle(GameMap &CurrentMap)
{
	int radius = 5;
	int xPos = 0;
	int yPos = 0;

	for(int i = 0; i < 130; i++)
	{
		xPos = x + radius * cos((3/10.f)*i);
		yPos = y + radius * sin((3/10.f)*i);
		if(xPos < 12)
			xPos = 12;
		if(xPos > 77)
			xPos = 77;
		if(yPos < 0)
			yPos = 0;
		if(yPos >21)
			yPos = 21;

		Circle.push_back(CurrentMap.ScanTileByCoord(yPos, xPos));
	}
}
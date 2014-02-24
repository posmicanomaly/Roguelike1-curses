#include "..\headers\Tile.h"

Tile::Tile(int ID, char syms, int ys, int xs, string types, bool passables, bool Bound)
{
	id = ID;
	sym = syms;
	base_sym = syms;
	y = ys ;	// 2 is a shim??? for Y
	x = xs + 12; // x shim
	type = types;
	passable = passables;
	HasNPC = false;
	HasPlayer = false;
	Visible = false;
	Explored = false;
	IsWall = Bound;
	item = "";
}
Tile::Tile(int ID, int Y, int X, string TILETYPE, string MESSAGE, string FLAG, string FLAG2)
{
	id = ID;
	TileType = TILETYPE;
	Message = MESSAGE;
	Flag = FLAG;
	Flag2 = FLAG2;
	y = Y;
	x = X + 12;
	HasNPC = false;
	HasPlayer = false;
	Visible = false;
	Explored = false;
	SetSymFromTileType(TileType);
}

void Tile::SetSymFromTileType(string TileType)
{
		 if(TileType == "#")
	{
		sym = '#';
		base_sym = '#';
		passable = false;
		IsWall = true;
		type = "Wall";
	}
	else if(TileType == ".")
	{
		sym = '.';
		base_sym = '.';
		passable = true;
		IsWall = false;
		type = "Floor";
	}
	else if(TileType == "O")
	{
		sym = 'O';
		base_sym = 'O';
		passable = true;
		IsWall = false;
		type = "Town";
	}
	else if(TileType == "C")
	{
		sym = 'C';
		base_sym = 'C';
		passable = true;
		IsWall = false;
		type = "Cave";
	}
	else if(TileType == ">")
	{
		sym = '>';
		base_sym = '>';
		passable = true;
		IsWall = false;
		type = "Up Stairs";
	}
	else if(TileType == "<")
	{
		sym = '<';
		base_sym = '<';
		passable = true;
		IsWall = false;
		type = "Down Stairs";
	}
	else if(TileType == "M")
	{
		sym = '^';
		base_sym = '^';
		passable = false;
		IsWall = true;
		type = "Mountains";
	}
}

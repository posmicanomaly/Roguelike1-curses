#include "..\headers\Player.h"
//#include "Message.h"

void Player::Draw()
{
	init_pair(15, COLOR_MAGENTA, COLOR_BLACK);
	move(y, x);
	addch('@' | A_BOLD | COLOR_PAIR(15));
	move(y, x);
}

Player::Player(int yc, int xc, GameMap &CurrentGameMap)
{
	y = yc;
	x = xc;
	fovradius = 0;
	CurrentTile = CurrentGameMap.ScanTileByCoord(y, x);
	PrevTile = CurrentTile;
	CurrentGameMap.TileMap[CurrentTile].HasPlayer = true;
	CurrentMap = CurrentGameMap.MapName;
	Alive = true;

	Turns = 0;

	Experience = 0;
	Level = 1;

	//stats
	Strength = Level * 1.2 - 2;
	Dexterity = Level * 0.9 - 0;
	Agility = Level * 0.9 + 2;
	Stamina = Level * 1.0 + 2;
	Intelligence = Level * 0.7 - 0;
	Wisdom = Level * 0.7 - 0;
	Charisma = Level * 1.0 - 2;
	//
	Regen = 1;
	//
	HP_Max = (Level * Stamina/2) * 1.3;
	HP_Current = HP_Max;

	CombatTile = -1;
};

bool Player::Move(string direction, GameMap &CurrentMap)
{
	
	if(direction == "up")
	{
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y - 1, x)) ||
				CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
		{
		case true:
			if(CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
			{
				//CurrentMap.TileMap[CurrentMap.LastTileScanned].HasNPC = false;
				CombatTile = CurrentMap.LastTileScanned;
				HasMoved = true;
			}
			
			return false;//Deny any move
			break;
		case false:
			//remove Player flag from previous tile and update flag for current tile
			CurrentMap.TileMap[CurrentTile].HasPlayer = false;
			CurrentMap.TileMap[CurrentMap.LastTileScanned].HasPlayer = true;
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
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y + 1, x)) ||
				CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
		{
		case true:
			if(CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
			{
				//CurrentMap.TileMap[CurrentMap.LastTileScanned].HasNPC = false;
				CombatTile = CurrentMap.LastTileScanned;
				HasMoved = true;
			}
			
			return false;
			break;
		case false:
			//remove Player flag from previous tile and update flag for current tile
			CurrentMap.TileMap[CurrentTile].HasPlayer = false;
			CurrentMap.TileMap[CurrentMap.LastTileScanned].HasPlayer = true;
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
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y, x - 1)) ||
				CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
		{
		case true:
			if(CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
			{
				//CurrentMap.TileMap[CurrentMap.LastTileScanned].HasNPC = false;
				CombatTile = CurrentMap.LastTileScanned;
				HasMoved = true;
			}
			
			return false;
			break;
		case false:
			//remove Player flag from previous tile and update flag for current tile
			CurrentMap.TileMap[CurrentTile].HasPlayer = false;
			CurrentMap.TileMap[CurrentMap.LastTileScanned].HasPlayer = true;
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
		switch(CurrentMap.CheckCollision(CurrentMap.ScanTileByCoord(y, x + 1)) ||
				CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
		{
		case true:
			if(CurrentMap.CheckCollisionWithNPC(CurrentMap.LastTileScanned))
			{
				//CurrentMap.TileMap[CurrentMap.LastTileScanned].HasNPC = false;
				CombatTile = CurrentMap.LastTileScanned;
				HasMoved = true;
			}
			
			return false;
			break;
		case false:
			//remove Player flag from previous tile and update flag for current tile
			CurrentMap.TileMap[CurrentTile].HasPlayer = false;
			CurrentMap.TileMap[CurrentMap.LastTileScanned].HasPlayer = true;
			x++;
			PrevTile = CurrentTile;
			CurrentTile = CurrentMap.LastTileScanned;
			HasMoved = true;
			return true;
			break;
		}
	}
}

void Player::GetFOV(GameMap &CurrentMap)
{
	vector<Tile>::iterator it;
	//reset all to invis
	for(it = CurrentMap.TileMap.begin(); it != CurrentMap.TileMap.end(); it++)
	{
		(*it).Visible = false;
	}
	Circle.clear();
	VisTiles.clear();
	GetCircle(CurrentMap);
	vector<int>::iterator nit;
	for(nit = Circle.begin(); nit != Circle.end(); nit++)
	{
		CurrentMap.Bresenham(y, x, CurrentMap.TileMap[(*nit)].y, CurrentMap.TileMap[(*nit)].x);
		VisTiles.push_back(*nit);
	}
}
		
void Player::GetCircle(GameMap &CurrentMap)
{
	//int radius = 8;
	//int iterations = 364;
	int radius = 0;
	int iterations = 0;
	int xPos = 0;
	int yPos = 0;
	switch(fovradius)
	{
	case 2:
		radius = 2;
		iterations = 100;
		break;
	case 4:
		radius = 4;
		iterations = 220;
		break;
	case 6:
		radius = 6;
		iterations = 364;
		break;
	}
	
	for(int i = 0; i < iterations; i++)
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

void Player::UpdateStats()
{
	if(Experience > 30)
		Level = 2;
	if(Experience > 75)
		Level = 3;
	if(Experience > 150)
		Level = 4;
	if(Experience > 320)
		Level = 5;
	if(Experience > 700)
		Level = 6;
	if(Experience > 1550)
		Level = 7;
	if(Experience > 3100)
		Level = 8;
	if(Experience > 6400)
		Level = 9;
	Strength = Level * 0.7 + (Level / 0.66) + (2.0 * Level);
	Strength = Level * 0.7 + (Level / 0.66) + 1.5;
	Dexterity = Level * 0.9 + (Level / 0.74) + 1.5;
	Agility = Level * 0.7 + (Level / 0.70) + 1.5;
	Stamina = Level * 0.9 + (Level / 0.85) + 2.5;
	Intelligence = Level * 0.7 - 0;
	Wisdom = Level * 0.7 - 0;
	Charisma = Level * 1.0;
	HP_Max = (Level * Stamina/2) * 1.7;

	ATKDamage = Strength;

	if(Turns%5 == 0 && HP_Current < HP_Max)
	{
		HP_Current += Regen;
		if(HP_Current > HP_Max)
			HP_Current = HP_Max;
	}
}
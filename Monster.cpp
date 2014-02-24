#include "Entity.h"

Monster::Monster(int y_loc, int x_loc, string type, int lvl, int flatmod, GameMap *CurrentMap)
{
	MarkedForDeath = false;
	Map = CurrentMap;
	Level = lvl;
	FlatModifier = flatmod;
	y = y_loc;
	x = x_loc;
	AggroPC = false;
	fovradius = 2;

	CurrentMap->ScanTileByCoord(y, x);
	CurrentMap->TileMap[CurrentMap->LastTileScanned].HasNPC = true;
	CurrentTile = CurrentMap->LastTileScanned;
	
	Stamina = Level * 0.9 + (Level / 0.85);

	if(type == "Gnoll")
	{
		HP_Max = (Level * Stamina/2) * 1.7 + FlatModifier;
		HP_Current = HP_Max;
		Symbol = 'G';

		Strength = Level * 2 + Level / 2;

		XPReward = 8 * Level;
	}
	else if(type == "Orc")
	{
		HP_Max = (Level * Stamina/2) * 1.7 + FlatModifier;
		HP_Current = HP_Max;
		Symbol = 'O';

		Strength = Level * 3 + Level / 3;
		XPReward = 10 * Level;
	}
	else if(type == "Rat")
	{
		HP_Max = (Level * Stamina/2) * 1.7 + FlatModifier;
		HP_Current = HP_Max;
		Symbol = 'r';

		Strength = Level * 1;
		XPReward = 1;
	}

	ATKDamage = Strength / 2;
	if(ATKDamage < 1)
		ATKDamage = 1;
}

void Monster::Draw()
{
	init_pair(2, COLOR_RED, COLOR_BLACK);

	move(y, x);
	if(AggroPC)
	{
		attron(COLOR_PAIR(2));
		addch(Symbol | A_BOLD);
		attroff(COLOR_PAIR(2));
	}
	else
		addch(Symbol | A_BOLD);
	move(y, x);
}

void Monster::Die(Entity &killedby)
{
	killedby.Experience += XPReward;
}

void Monster::GetFOVNPC(GameMap &CurrentMap)
{
	vector<int> ReturnedTiles;
	vector<Tile>::iterator it;
	Circle.clear();
	VisTiles.clear();
	GetCircle(CurrentMap);
	vector<int>::iterator nit;
	for(nit = Circle.begin(); nit != Circle.end(); nit++)
	{
		ReturnedTiles.clear();
		ReturnedTiles = CurrentMap.BresenhamNPC(y, x, CurrentMap.TileMap[(*nit)].y, CurrentMap.TileMap[(*nit)].x);
		for(int i = 0; i < ReturnedTiles.size(); i++)
			VisTiles.push_back(ReturnedTiles[i]);
	}
	
}
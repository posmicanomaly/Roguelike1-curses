#ifndef ENTITY_H
#define ENTITY_H
#include "Map.h"

class Entity{
public:
	//Entity(int yc, int xc, GameMap &CurrentMap);
	void GetFOV(GameMap &CurrentMap);
	int fovradius;
	void GetCircle(GameMap &CurrentMap);
	vector<int> Circle;
	int y;
	int x;
	
	void *vEngine;

	int CurrentTile;
	int PrevTile;

	bool Move(string direction, GameMap &CurrentMap);
	bool Alive;
	bool HasMoved;
	bool MarkedForDeath;
	int CombatTile;
	//general movable object stuff
	int HP_Current;
	int HP_Max;
	int Level;
	int Experience;
	int Strength;
	int Agility;
	int Dexterity;
	int Stamina;
	int Intelligence;
	int Wisdom;
	int Charisma;

	int ATKDamage;
	GameMap *Map;

	vector<int> VisTiles;
};



class Monster : public Entity{
public:
	Monster(int y_loc, int x_loc, string Type, int Level, int FlatModifier, GameMap *CurrentMap);
	void GetFOVNPC(GameMap &CurrentMap);
	string Type;
	int FlatModifier;
	int XPReward;
	char Symbol;
	void Draw();
	void Die(Entity &killedby);

	bool AggroPC;

};
#endif
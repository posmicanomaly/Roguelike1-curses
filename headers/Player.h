#ifndef PLAYER_H
#define PLAYER_H
#include "..\headers\Entity.h"

class Player : public Entity{
public:
	Player(int yc, int xc, GameMap &CurrentMap);
	void GetFOV(GameMap &CurrentMap);
	void Draw();
	void GetCircle(GameMap &CurrentMap);
	bool Move(string direction, GameMap &CurrentMap);
	vector<int> Circle;

	//player specific shit
	int Strength;
	int Dexterity;
	int Agility;
	int Stamina;
	int Intelligence;
	int Wisdom;
	int Charisma;

	int Turns;
	int Regen;

	string PlayerName;

	void UpdateStats();

	string CurrentMap;
};
#endif
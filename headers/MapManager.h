#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include "Map.h"
#include "..\headers\Entity.h"
#include "..\headers\Player.h"

class MapManager
{
public:
	vector<GameMap> MapVec;
	list<string> MapHistory;
	void LoadAllMaps();
	void ChangeMap(Player &player, string MapToChangeTo);
	void PrevMap(Player &player);

	GameMap *CurrentMap;

	string GetPreviousMap(){list<string>::reverse_iterator rit = MapHistory.rbegin(); return (*rit);}
	string RemoveAndGetPreviousMap(string &CurrentMap){MapHistory.remove(CurrentMap); list<string>::reverse_iterator rit = MapHistory.rbegin(); return (*rit);}

	string PreviousMap;
private:
	ifstream file_in;
};

#endif
#ifndef NPCMANAGER_H
#define NPCMANAGER_H
#include <sstream>
#include "Entity.h"
#include "Player.h"
#include "MapManager.h"
#include "Map.h"
#include <list>
using namespace std;

class NPCManager{
public:
	NPCManager();

	void SpawnRandomMonster(GameMap *CurrentMap);
	void SpawnMonster(string Type, GameMap *CurrentMap);
	void DrawAll(GameMap *CurrentMap);
	void DrawVisibleNPCs(GameMap *CurrentMap);
	void CheckNPCAggro(GameMap *CurrentMap, Player &player);
	void KillNPC(GameMap *CurrentMap, Player &player);
	void CombatPC(GameMap *CurrentMap, Player &player);
	list<Monster> MonsterList;

	vector<string> CombatBuffer;

	int PlayerTurns;
	int MobLevel;
};

#endif
#include "NPCManager.h"

NPCManager::NPCManager()
{
	MobLevel = 1;
}

void NPCManager::SpawnRandomMonster(GameMap *CurrentMap)
{
	bool Spawned = false;
	while(!Spawned)
	{
		int Y = rand() % 22;
		int X = 0;
		while(X < 12)
			X = rand() % 66;

		CurrentMap->ScanTileByCoord(Y, X);
		bool p_HasNPC = CurrentMap->TileMap[CurrentMap->LastTileScanned].HasNPC;
		bool p_HasPlayer = CurrentMap->TileMap[CurrentMap->LastTileScanned].HasPlayer;
		bool p_Passable = CurrentMap->TileMap[CurrentMap->LastTileScanned].passable;
		if(p_Passable && !(p_HasNPC || p_HasPlayer))
		{
			MonsterList.push_back(Monster(Y, X, "Gnoll", 1, 0, CurrentMap));
			Spawned = true;
		}
		else
			Spawned = false;
	}
}

void NPCManager::SpawnMonster(string Type, GameMap *CurrentMap)
{
	bool Spawned = false;
	if(PlayerTurns%50 == 0)
		MobLevel++;

	while(!Spawned)
	{
		int Y = rand() % 22;
		int X = 0;
		while(X < 12)
			X = rand() % 66;

		CurrentMap->ScanTileByCoord(Y, X);
		bool p_HasNPC = CurrentMap->TileMap[CurrentMap->LastTileScanned].HasNPC;
		bool p_HasPlayer = CurrentMap->TileMap[CurrentMap->LastTileScanned].HasPlayer;
		bool p_Passable = CurrentMap->TileMap[CurrentMap->LastTileScanned].passable;
		if(p_Passable && !(p_HasNPC || p_HasPlayer))
		{
			MonsterList.push_back(Monster(Y, X, Type, MobLevel, 0, CurrentMap));
			Spawned = true;
		}
		else
			Spawned = false;
	}
}

void NPCManager::DrawAll(GameMap *CurrentMap)
{
	for(list<Monster>::iterator it = MonsterList.begin(); it != MonsterList.end(); it++)
	{
		if((*it).Map == CurrentMap)
		{
			(*it).Draw();
			(*it).GetFOV(*CurrentMap);
		}
	}
}

void NPCManager::CheckNPCAggro(GameMap *CurrentMap, Player &player)
{
	for(list<Monster>::iterator it = MonsterList.begin(); it != MonsterList.end(); it++)
	{
		(*it).GetFOVNPC(*CurrentMap);
		for(int i = 0; i < (*it).VisTiles.size(); i++)
		{
			if((*it).VisTiles[i] == player.CurrentTile)
			{
				(*it).AggroPC = true;
				break;
			}
			else
				(*it).AggroPC = false;
		}
	}
}

void NPCManager::DrawVisibleNPCs(GameMap *CurrentMap)
{
	vector<int> NPCTileList;
	vector<int> PCVisTiles;
	vector<int> NPCtoDraw;
	for(list<Monster>::iterator it = MonsterList.begin(); it != MonsterList.end(); it++)
	{
		NPCTileList.push_back((*it).CurrentTile);
	}

	for(int i = 0; i < CurrentMap->TileMap.size(); i++)
	{
		if((*CurrentMap).TileMap[i].Visible)
			PCVisTiles.push_back((*CurrentMap).TileMap[i].id);
	}

	for(int i = 0; i < PCVisTiles.size(); i++)
		for(int j = 0; j < NPCTileList.size(); j++)
			if( PCVisTiles[i] == NPCTileList[j])
				NPCtoDraw.push_back(NPCTileList[j]);

	for(list<Monster>::iterator it = MonsterList.begin(); it != MonsterList.end(); it++)
	{
		for(int i = 0; i < NPCtoDraw.size(); i++)
			if((*it).CurrentTile == NPCtoDraw[i])
				if((*it).Map == CurrentMap)
				{
					(*it).Draw();
				}
	}
}

void NPCManager::KillNPC(GameMap *CurrentMap, Player &player)
{
	if(player.CombatTile != -1)
	{
		for(list<Monster>::iterator it = MonsterList.begin(); it != MonsterList.end(); it++)
		{
			if(player.CombatTile == (*it).CurrentTile)
			{
				// working on this
				(*it).Die(player);
				CurrentMap->TileMap[(*it).CurrentTile].HasNPC = false;
				MonsterList.erase(it);
				player.CombatTile = -1;
				break;
			}			
		}
	}
}

void NPCManager::CombatPC(GameMap *CurrentMap, Player &player)
{
	stringstream ss;
	for(list<Monster>::iterator it = MonsterList.begin(); it != MonsterList.end(); it++)
	{
		if(player.CombatTile == (*it).CurrentTile)
		{
			int PCMiss = rand() % 100;
			int NPCMiss = rand() % 100;
			ss.str("");
			if(PCMiss < 20)
				ss << "You MISSED! ";
			else
			{
				(*it).HP_Current -= player.ATKDamage;
				ss << "You Hit " << (*it).Symbol << "(" << (*it).Level << ") for " << player.ATKDamage << "; ";
			}

			if(NPCMiss < 40)
				ss << (*it).Symbol << " MISSED you! ";
			else
			{
				player.HP_Current -= (*it).ATKDamage;
				ss << (*it).Symbol << " Hit You For " << (*it).ATKDamage;
			}
			
			if(player.HP_Current <= 0)
			{
				player.Alive = false;
				break;
			}

			if((*it).HP_Current <= 0)
			{
				ss << " --You Killed " << (*it).Symbol;
				CombatBuffer.push_back(ss.str());
				KillNPC(CurrentMap, player);
				break;
			}

			else
			{
				player.CombatTile = -1;
				CombatBuffer.push_back(ss.str());
				break;
			}
		}
	}
}
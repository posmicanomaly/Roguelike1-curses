#include "MapManager.h"

void MapManager::LoadAllMaps()
{
	file_in.open("maplist.txt");
	string line;
	vector<string> FilesToLoad;
	while(!file_in.eof())
	{
		getline(file_in, line);
		FilesToLoad.push_back(line);
	}
	file_in.close();
	vector<string>::iterator itr;
	move(0,0);
	//for(itr = FilesToLoad.begin(); itr != FilesToLoad.end(); itr++)
	for(int i = 0; i != FilesToLoad.size(); i++)
	{
		//MapVec.push_back(GameMap(*itr, file_in));
		MapVec.push_back(GameMap(FilesToLoad[i], file_in));
	}
}

void MapManager::ChangeMap(Player &player, string MapToChangeTo)
{
	for(int i = 0; i != MapVec.size(); i++)
	{
		if(player.CurrentMap == MapVec[i].MapName)
		{
			MapVec[i].StorePlayerPos(player.y, player.x);
		}
	}

	if(MapToChangeTo == PreviousMap)
	{
		player.CurrentMap = MapToChangeTo;
	}
	else
	{
		player.CurrentMap = MapToChangeTo;
		MapHistory.push_back(MapToChangeTo);
	}
	if(player.CurrentMap == "overworld")
		player.fovradius = 2;
	else if(player.CurrentMap == "dungeon")
		player.fovradius = 4;
	else if(player.CurrentMap == "town")
		player.fovradius = 6;
	else if(player.CurrentMap == "Hamlet")
		player.fovradius = 6;

}

void MapManager::PrevMap(Player &player)
{
	string temp;
	player.CurrentMap = temp;
	player.CurrentMap = PreviousMap;
	PreviousMap = temp;
}
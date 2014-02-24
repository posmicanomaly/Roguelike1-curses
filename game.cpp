//alpha 8 code cleanup
//todo:
//
//
//
//

#include "Default.h"
#include "NPCManager.h"

using namespace std;

int main()
{
	srand(time(NULL));
	//Init Engine, creates window on declaration
	Engine GameEngine;
	GameEngine.WelcomeScreen();

	//Init MapManager
	MapManager MapManager;
	MapManager.LoadAllMaps();
	//Hack to set Currentmap and Previous map
	//Map transition is currently dependant on a "history" so previous in history is previous map
	//TODO make this not like that
	MapManager.CurrentMap = &MapManager.MapVec[0];
	MapManager.PreviousMap = "TestDungeon1";
	//Start the maphistory with our current map as place #1, previousmap needs to be there for some dumb reason i forget
	MapManager.MapHistory.push_back(MapManager.CurrentMap->MapName);

	
	
	//TODO player should know where to be inserted by a special flag on a tile on a per level basis
	Player player(10, 22, *MapManager.CurrentMap);
	//Hack fovradius, this is usually set by the "style" or the current map/level
	player.fovradius = 6;

	player.PlayerName = GameEngine.PlayerName;
	
	//DOES THIS NEED TO BE HERE???
	player.Draw();
	
	
	GameEngine.DrawUI(player, *MapManager.CurrentMap);

	GameMap *ActiveMap = MapManager.CurrentMap;
	//CurrentMap->LoadPlayerPos(player.y, player.x);
	while(player.Alive)
	{
		//Redundancy for the number of Turns that have been made
		GameEngine.NPCManager.PlayerTurns = player.Turns;
		//TEMPORARY
		//Hack for monster random spawn crap/////////////////////////////////
		if(player.Turns%10 == 0)
		{
			int Spawner = rand()%10;
			{
				if(Spawner < 5)
					GameEngine.NPCManager.SpawnMonster("Rat", ActiveMap);
				else if(Spawner >= 5 && Spawner < 7)
					GameEngine.NPCManager.SpawnMonster("Gnoll", ActiveMap);
				else if(Spawner >=7 && Spawner < 9)
					GameEngine.NPCManager.SpawnMonster("Orc", ActiveMap);
			}

		}
		/////////////////////////////////////////////////////////////////////
		
		//Hack ?
		//if the activemap(checked by a string name) is not the currentmap(usually set by a level change)
		//make it so
		if(ActiveMap->MapName != player.CurrentMap)
		{
			for(int i = 0; i != MapManager.MapVec.size(); i++)
			{
				if(MapManager.MapVec[i].MapName == player.CurrentMap)
				{
					ActiveMap = &MapManager.MapVec[i];
					ActiveMap->LoadPlayerPos(player.y, player.x);
					MapManager.CurrentMap = ActiveMap;
				}
			}
		}

		//This is some broken combat for sure, I've learned a lot since this, so I will go back and fix this if i update this project.
		if(player.CombatTile != -1)
		{
			GameEngine.NPCManager.CombatPC(ActiveMap, player);
			vector<string> FromCombat = GameEngine.NPCManager.CombatBuffer;
			for(int i = 0; i < FromCombat.size(); i++)
			{
				GameEngine.UIMessageBuffer.push_back(FromCombat[i]);
			}
		}
		////////////////////////////////////////////////////////

		player.UpdateStats();
		GameEngine.NPCManager.CheckNPCAggro(ActiveMap, player);
			
		if(player.HasMoved || GameEngine.NeedFullRedraw == true)
		{
			++player.Turns;
			player.HasMoved = false;
			GameEngine.NeedFullRedraw = false;
			clear();
			player.GetFOV(*ActiveMap);
			ActiveMap->DrawMap();
			GameEngine.DrawUI(player, *ActiveMap);
			player.Draw();
			//GameEngine.NPCManager.DrawAll(ActiveMap);
			
			GameEngine.NPCManager.DrawVisibleNPCs(ActiveMap);
		}
		GameEngine.GetInput();
		GameEngine.ProcessPlayerInput(player, *ActiveMap, MapManager);
	}
	clear();
	mvprintw(6, 30, "Looks like you probably died....");
	getch();
	//GameEngine.~Engine();
	return 0;
}
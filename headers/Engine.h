#ifndef ENGINE_H
#define ENGINE_H

#include <sstream>
#include <curses.h>
#include "..\headers\Entity.h"
#include "..\headers\MapManager.h"
#include "..\headers\NPCManager.h"
using namespace std;

class Engine{
public:
	Engine();
	~Engine();
	int Input;
	bool NeedFullRedraw;
	vector<string> UIMessageBuffer;
	bool GetInput();
	bool ProcessPlayerInput(Player &player, GameMap &CurrentMap, MapManager &MapManager);
	void DrawUI(Player &player, GameMap &CurrentMap);
	void DrawUIMessage();
	void DrawUIMessageHistory(list<Monster> mlist);

	void DrawBorderedWindow(int yc, int xc, int ysize, int xsize, string title);
	WINDOW *Window;

	void WelcomeScreen();

	ifstream file_in2;
	stringstream MessageStream;
	string PlayerName;
	NPCManager NPCManager;
};

#endif

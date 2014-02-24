#include "..\headers\Engine.h"
#include <iostream>
using namespace std;

Engine::Engine()
{
	cout << "You enter a world in which shit just isn't right at all..\n" <<
			"Even if you think you know what you're doing, death lies around the corner..\n" <<
			"Either as a monstrous Gnoll or menacing Orc, or even a bugged out Alpha super Rat!\n" <<
			"\n" <<
			"What is your name:  ";
	cin >> PlayerName;

	Window = initscr();
	start_color();
	raw();
	keypad(Window, TRUE);
	noecho();
	refresh();
	NeedFullRedraw = true;
	for(int i = 0; i < 18; i++)
		UIMessageBuffer.push_back("");
	curs_set(0);
}

Engine::~Engine()
{
	endwin();
}

bool Engine::GetInput()
{
	Input = getch();
	return true;
}

bool Engine::ProcessPlayerInput(Player &player, GameMap &CurrentMap, MapManager &MapManager)
{
	string Flag = CurrentMap.TileMap[player.CurrentTile].Flag;
	string Flag2 = CurrentMap.TileMap[player.CurrentTile].Flag2;
	switch(Input)
	{
	case KEY_UP:
		player.Move("up", CurrentMap);
		break;
	case KEY_DOWN:
		player.Move("down", CurrentMap);
		break;
	case KEY_LEFT:
		player.Move("left", CurrentMap);
		break;
	case KEY_RIGHT:
		if(player.Move("right", CurrentMap))
		{
			
		}
		break;
	case 'E':
		player.Experience += 1000;
		NeedFullRedraw = true;
		break;
	case 'M':
		NPCManager.SpawnRandomMonster(MapManager.CurrentMap);
		NeedFullRedraw = true;
		break;
	case 'm':
		//DrawUIMessageHistory();
		break;
	case 'e':
	//	//E IS USE KEY
	//	string type = CurrentMap.TileMap[player.CurrentTile].type;
		if(Flag == "Town" || Flag == "Down Stairs" || Flag == "Cave")
		{
			MapManager.ChangeMap(player, Flag2);
			NeedFullRedraw = true;
			MessageStream << "You Entered " << Flag2;
			UIMessageBuffer.push_back(MessageStream.str());
			MessageStream.str("");
		}
		else if(Flag == "Up Stairs" || Flag == "EntranceExit")
		{
			MapManager.ChangeMap(player, MapManager.RemoveAndGetPreviousMap(player.CurrentMap));
			NeedFullRedraw = true;
			MessageStream << "You Entered " << player.CurrentMap;
			UIMessageBuffer.push_back(MessageStream.str());
			MessageStream.str("");
		}
		break;
			
	default:
		break;
	}
	return true;
}

void Engine::DrawUI(Player &player, GameMap &CurrentMap)
{
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, (COLOR_RED | COLOR_YELLOW), COLOR_BLACK);
	init_pair(7, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(4) | A_BOLD);
	move(0, 0);
	printw("%s", player.PlayerName.c_str());
	move(1, 0);
	printw("Human");
	move(2, 0);
	printw("Cleric");
	attroff(COLOR_PAIR(4) | A_BOLD);
	move(4, 0);
	printw("Level %i", player.Level);
	move(5, 0);
	printw("XP: %i", player.Experience);
	move(6, 0);
	attron(COLOR_PAIR(5) | A_BOLD);
	printw("HP: %i/%i", player.HP_Current, player.HP_Max);
	attroff(COLOR_PAIR(5) | A_BOLD);
	move(7, 0);
	attron(COLOR_PAIR(6));
	printw("STR: %i", player.Strength);
	move(8, 0);
	printw("AGI: %i", player.Agility);
	move(9, 0);
	printw("DEX: %i", player.Dexterity);
	move(10, 0);
	printw("STA: %i", player.Stamina);
	move(11, 0);
	printw("INT: %i", player.Intelligence);
	move(12, 0);
	printw("WIS: %i", player.Wisdom);
	move(13, 0);
	printw("CHA: %i", player.Charisma);
	attroff(COLOR_PAIR(6));

	////

	move(15, 0);
	printw("%s", CurrentMap.TileMap[player.CurrentTile].type.c_str());
	move(16, 0);
	printw("Turn: %i", player.Turns);
	/////////////////////////////////
	DrawUIMessage();
	//move(22,12);
	//printw("%i, %i, t: %i.  %s", player.y, player.x, player.CurrentTile, CurrentMap.TileMap[player.CurrentTile].type.c_str());
}

void Engine::DrawUIMessage()
{
	int y = 0;
	vector<string>::reverse_iterator i;
	for(i = UIMessageBuffer.rbegin(); i != UIMessageBuffer.rend(); i++)
	{
		if(y == 1)
		{
			mvprintw(22 + y, 12, (*i).c_str());
			break;
		}
		else
		{
			attron(A_BOLD);
			mvprintw(22 + y, 12, (*i).c_str());
			attroff(A_BOLD);
			y++;
		}
	}
}

void Engine::DrawUIMessageHistory(list<Monster> mlist)
{
	DrawBorderedWindow(0, 0, 20, 70, "Memory");
	
	int y = 0;
	int xbuffer = 1;
	int ybuffer = 1;
	vector<string>::reverse_iterator r;
	int lines = 0;
	/*for(r = UIMessageBuffer.rbegin(); r != UIMessageBuffer.rend(); r++)
	{
		if(lines == 17)
		{
			mvprintw(0 + y + ybuffer, 0 + xbuffer, "Your memory fails you...");
		}
		else
		{
			mvprintw(0 + y + ybuffer, 0 + xbuffer, (*r).c_str());
			y++;
			lines++;
		}
	}*/
	const char * constant;
	for (list<Monster>::iterator it = mlist.begin(); it != mlist.end(); it++)
	{
		//mvprintw(0 + y + ybuffer, 0 + xbuffer, (*it).Symbol);
	}

	getch();
	NeedFullRedraw = true;
}

void Engine::DrawBorderedWindow(int yc, int xc, int ysize, int xsize, string title)
{
	for(int y = 0; y < ysize; y++)
		for(int x = 0; x < xsize; x++)
		{
			if( y == 0 && x == 0)
				mvaddch(y+yc, x+xc, ACS_ULCORNER);
			else if(y == 0 && x == xsize - 1)
				mvaddch(y+yc, x+xc, ACS_URCORNER);
			else if(y == ysize - 1 && x == 0)
				mvaddch(y+yc, x+xc, ACS_LLCORNER);
			else if(y == ysize - 1 && x == xsize - 1)
				mvaddch(y+yc, x+xc, ACS_LRCORNER);
			else if(y == 0 || y == ysize - 1 && !(x == 0 || x == xsize - 1))
				mvaddch(y+yc, x+xc, ACS_HLINE);
			else if(y == ysize - 1 && !(x == 0 || x == xsize - 1))
				mvaddch(y+yc, x+xc, ACS_HLINE);
			else if(( y > 0 || y < ysize - 1 ) && (x == 0 || x == xsize - 1))
				mvaddch(y+yc, x+xc, ACS_VLINE);
			else
				mvaddch(y+yc, x+xc, ' ');
		}
	mvprintw(0 + yc, 2 + xc, title.c_str());
}

void Engine::WelcomeScreen()
{
	move(0,0);
	printw("Hello,\nThis is the Alpha of game titled thus far as..\n\n");
	init_pair(40, COLOR_BLUE, COLOR_BLACK);
	attron(COLOR_PAIR(40));
	printw("CAVES OF PROCRASTINATION");
	attroff(COLOR_PAIR(40));
	printw("\n\n\nAs such this game will be developed really slowly...");
	printw("\nThere is not much to show so far...");
	printw("\nSome features are partially implemented");
	printw("\n\nthe 'E' key is your Use key, use it on a tile to see what happens...");
	printw("\n...but you'll find it only works on stairs, town, and cave tiles");
	printw("\n");
	printw("\nThis serves as an archived project of sorts now, and is no longer");
	printw("\nupdated, at least for the time being. The successor to this project");
	printw("\nis called Posgima-1, and can be found on my github below.");
	printw("\nhttp://github.com/posmicanomaly/posgima-1");
	printw("\\nnenjoy! -Jesse(posmicanomaly)");
	getch();
}
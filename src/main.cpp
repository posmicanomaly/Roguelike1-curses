#include <curses.h>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <sstream>

using namespace std;

bool GlobalCheckMobCollision = false;

//LastTileScanned is usually set when ScanMapCollision is run


int LastTileScanned = 0;
string GlobalMessage;
stringstream GlobalStream;
//variable to push the map down x amount of lines to allow for text on top of map
int SHIMy = 1;

//contains all information about each tile
class Tile
{
public:
	char base_sym;
	char sym;
	int y;
	int x;
	string item;
	string type;
	bool passable;
	bool check(int y, int x);
	bool HasMonster;
	bool HasPlayer;
	bool Visible;
	bool Explored;
	Tile(char sym, int y, int x, string type, bool passable);
};

Tile::Tile(char syms, int ys, int xs, string types, bool passables)
{
	sym = syms;
	base_sym = syms;
	y = ys + SHIMy;
	x = xs;
	type = types;
	passable = passables;
	HasMonster = false;
	HasPlayer = false;
	Visible = false;
	Explored = false;
	item = "";
}
//Main MAP Class to hold all maps and most methods
class CMap
{
public:
	CMap(){ max_items = 20;}
	vector<string> Map;
	vector<string> ItemMap;
	vector<Tile> TileMap;
	int max_items;
	ifstream file_in;
	void GenerateItems();
	void LoadMap();
	void DrawMap();
	void DrawItems();
	
	//bool ScanMapCollision(int y, int x);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//checks if the next tile is able to be moved to, and then calls other functions if the move is sucessful such as item or death
bool ScanMapCollision(int y, int x, CMap &cmap, bool FOVCheck);
//Keep ScanMap right after CMap
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMap::GenerateItems()
{
	file_in.open("itemmap.txt");
	string temp;
	while(!file_in.eof())
	{
		getline(file_in, temp);
		ItemMap.push_back(temp);
	}
	file_in.close();
	for(int i = 0; i < ItemMap.size(); i++)
		for(int j = 0; j < ItemMap[i].size(); j++)
		{
			if(ItemMap[i][j] == '$')
				for(int t = 0; t < TileMap.size(); t++)
					if(TileMap[t].y == i && TileMap[t].x == j)
					{
						TileMap[t].item = "money";
						TileMap[t].sym = '$';
					}
					else
						;
		}
}

void CMap::LoadMap()
{
	printw("loading map\n");
	file_in.open("map.txt");
	if(file_in.fail())
		printw("couldn't open\n");
	else
		printw("loaded map\n");
	string temp;
	while(!file_in.eof())
	{
		getline(file_in, temp);
		Map.push_back(temp);
	}
	file_in.close();
	for(int i = 0; i < Map.size(); i++)
		for(int j = 0; j < Map[i].size(); j++)
		{
			if(Map[i][j] == ' ')
				TileMap.push_back(Tile(' ', i, j, "wall", false));
			else if(Map[i][j] == '-')
				TileMap.push_back(Tile('-', i, j, "wall", false));
			else if(Map[i][j] == '|')
				TileMap.push_back(Tile('|', i, j, "wall", false));
			else if(Map[i][j] == 'H')
				TileMap.push_back(Tile('H', i, j, "hall", true));
			else if(Map[i][j] == '.')
			{
				TileMap.push_back(Tile('.', i, j, "floor", true));
			}
			else if(Map[i][j] == '~')
			{
				TileMap.push_back(Tile('~', i, j, "water", true));
			}
			else if(Map[i][j] == 'L')
			{
				TileMap.push_back(Tile('~', i, j, "lava", true));
			}
		}
}
//call this after DrawMap()
void CMap::DrawItems()
{
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	for(int i = 0; i < TileMap.size(); i++)
	{
		if(TileMap[i].Visible == true)
		{
			move(TileMap[i].y, TileMap[i].x);
			if(TileMap[i].item == "money")
			{
				attron(COLOR_PAIR(3) | A_BOLD);
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(3) | A_BOLD);
			}
			else
			{
				//
			}
		}
	}
}
//draws basic tiles of map, not items. call DrawItems after
void CMap::DrawMap()
{
	
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	for(int i = 0; i < TileMap.size(); i++)
	{
		if(TileMap[i].Visible == true)
			attron(A_BOLD);
		if(TileMap[i].Explored == true)
		{
			move(TileMap[i].y, TileMap[i].x);
			if(TileMap[i].type == "water")
			{
				attron(COLOR_PAIR(1) | A_BOLD);
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(1) | A_BOLD);
			}
			else if(TileMap[i].type == "lava")
			{
				attron(COLOR_PAIR(2) | A_BOLD);
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(2) | A_BOLD);
			}
			else if(TileMap[i].type == "floor")
			{
				addch(ACS_BULLET | A_DIM);
			}
			else if(TileMap[i].type == "wall")
			{
				attron(A_BOLD);
				if(TileMap[i].sym == '-')
					addch(ACS_HLINE);
				else if(TileMap[i].sym == '|')
					addch(ACS_VLINE);
				attroff(A_BOLD);
			}
			else if(TileMap[i].type == "hall")
			{
				addch(ACS_BOARD);
			}
			else
				printw("%c", TileMap[i].sym);
		}
		attroff(A_BOLD);
	}
	move(0, 0);
	refresh();
}


//Multipurpose object currently using for player and npc
class GameObject
{
public:
	GameObject(char symbol, int ys, int xs, int money_amt);
	
	void GetFOV(CMap &currentmap);
	int cur_hp;
	int max_hp;
	int level;
	void LevelUp();
	void AddXp(int xptoadd);
	int xp;
	char ch;
	int y;
	int x;
	int money;
	int CurrentTile;
	int PrevTile;
	bool GameObject::move(string direction, CMap &cmap);
	bool dead;
	//only applies to npc
	int ct_down;
	int ct_up;
	int ct_left;
	int ct_right;
	int ct_limit;
};
bool ScanMobCollision(CMap &cmap, GameObject &player, vector<GameObject> &MonsterList);
GameObject::GameObject(char symbol, int ys, int xs, int money_amt)
{
	level = 1;
	max_hp = (level * 10 * 2) - (level * 7 / 2);
	cur_hp = max_hp;
	xp = 0;
	ch = symbol;
	y = ys;
	x = xs;
	ct_down = 0;
	ct_up = 0;
	ct_right = 0;
	ct_left = 0;
	ct_limit = 5;
	money = money_amt;
	CurrentTile = 0;
	PrevTile = 0;
	dead = false;
}
void GameObject::GetFOV(CMap &currentmap)
{
	//Clear all visibility to recalculate
	for(int i = 0; i < currentmap.TileMap.size(); i++)
		currentmap.TileMap[i].Visible = false;
	//set the current tile visible
	currentmap.TileMap[CurrentTile].Visible = true;
	currentmap.TileMap[CurrentTile].Explored = true;
	//end of init
	/////////////////////////////////////////////////////////////////////////
	///////////////begin checking top to right//////////////////////////////
	/////////////////////////////////////////////////////////////////////////



	/*int FOVRange = 10;
	for(int p = 1; p < FOVRange; p++)
	{
		if(ScanMapCollision(y - p, x, currentmap, true))
		{
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
		else
		{
			ScanMapCollision(y - p, x, currentmap, true);
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
			break;
		}
	}

	for(int p = 1; p < FOVRange; p++)
	{
		int ScanHolder;
		if(ScanMapCollision(y - (p + 2), x + p, currentmap, true))
		{
			ScanHolder = LastTileScanned;
			if(ScanMapCollision(y - p, x, currentmap, true))
			{
				LastTileScanned = ScanHolder;
				currentmap.TileMap[LastTileScanned].Visible = true;
				currentmap.TileMap[LastTileScanned].Explored = true;
			}
		}
		else
		{
			
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
			break;
		}
	}*/

	if(!ScanMapCollision(y - 1, x, currentmap, true) || ScanMapCollision(y - 1, x, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	if(ScanMapCollision(y - 1, x, currentmap, true))
	{		
		if(!ScanMapCollision(y - 2, x, currentmap, true) || ScanMapCollision(y - 2, x, currentmap, true))
		{
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////diagonal check, depends if there's a wall at p-1

	if(!ScanMapCollision(y - 2, x + 1, currentmap, true) || ScanMapCollision(y - 2, x + 1, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y - 1, x, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////////////////////////////////////////////////////////
	if(!ScanMapCollision(y - 1, x + 1, currentmap, true) || ScanMapCollision(y - 1, x + 1, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	////////////////////////////////////////////////////////
	
	////diagonal check, depends if there's a wall at p-1
	if(!ScanMapCollision(y - 1 , x + 2, currentmap, true) || ScanMapCollision(y - 1, x + 2, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y, x + 1, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////////////////////////////////////////////////////////
	/////////			bottom to left				////////
	////////////////////////////////////////////////////////
	if(!ScanMapCollision(y + 1, x, currentmap, true) || ScanMapCollision(y + 1, x, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	if(ScanMapCollision(y + 1, x, currentmap, true))
	{
		if(!ScanMapCollision(y + 2, x, currentmap, true) || ScanMapCollision(y + 2, x, currentmap, true))
		{
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	///////////////////////////diagonal check
	if(!ScanMapCollision(y + 2, x - 1, currentmap, true) || ScanMapCollision(y + 2, x - 1, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y + 1, x, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////////////////////////////////////////////////////////
	if(!ScanMapCollision(y + 1, x - 1, currentmap, true) || ScanMapCollision(y + 1, x - 1, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	////////////////////////////////////////////////////////
	///////////////////////////diagonal check
	if(!ScanMapCollision(y + 1, x - 2, currentmap, true) || ScanMapCollision(y + 1, x - 2, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y, x - 1, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////////////////////////////////////////////////////////
	/////////						right to bottom	////////
	////////////////////////////////////////////////////////
	if(!ScanMapCollision(y, x + 1, currentmap, true) || ScanMapCollision(y, x + 1, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	if(ScanMapCollision(y, x + 1, currentmap, true))
	{		
		if(!ScanMapCollision(y, x + 2, currentmap, true) || ScanMapCollision(y, x + 2, currentmap, true))
		{
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	///////////////////////////diagonal check
	if(!ScanMapCollision(y + 1, x + 2, currentmap, true) || ScanMapCollision(y + 1, x + 2, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y, x + 1, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////////////////////////////////////////////////////////
	if(!ScanMapCollision(y + 1, x + 1, currentmap, true) || ScanMapCollision(y + 1, x + 1, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	////////////////////////////////////////////////////////
	///////////////////////////diagonal check
	if(!ScanMapCollision(y + 2, x + 1, currentmap, true) || ScanMapCollision(y + 2, x + 1, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y + 1, x, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////////////////////////////////////////////////////////
	/////////							left to top	////////
	////////////////////////////////////////////////////////
	if(!ScanMapCollision(y, x - 1, currentmap, true) || ScanMapCollision(y, x - 1, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	if(ScanMapCollision(y, x - 1, currentmap, true))
	{		
		if(!ScanMapCollision(y, x - 2, currentmap, true) || ScanMapCollision(y, x - 2, currentmap, true))
		{
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
		///////////////////////////diagonal check
	if(!ScanMapCollision(y - 1, x - 2, currentmap, true) || ScanMapCollision(y - 1, x - 2, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y, x - 1, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	//////////////////////////////////////////////////////
	if(!ScanMapCollision(y - 1, x - 1, currentmap, true) || ScanMapCollision(y - 1, x - 1, currentmap, true))
	{
		currentmap.TileMap[LastTileScanned].Visible = true;
		currentmap.TileMap[LastTileScanned].Explored = true;
	}
	////////////////////////////////////////////////////////
		///////////////////////////diagonal check
	if(!ScanMapCollision(y - 2, x - 1, currentmap, true) || ScanMapCollision(y - 2, x - 1, currentmap, true))
	{
		int ScanHolder = LastTileScanned; // hold the last tile scanned because we need to check p - 1 for false return from ScanMapCollision()
		if(ScanMapCollision(y - 1, x, currentmap, true))
		{
			LastTileScanned = ScanHolder;	// must put the correct value back into last tile scanned otherwise it will work with p -1 again.
			currentmap.TileMap[LastTileScanned].Visible = true;
			currentmap.TileMap[LastTileScanned].Explored = true;
		}
	}
	////////////////////////////////////////////////////////
	/////////		done							////////
	////////////////////////////////////////////////////////
}
bool GameObject::move(string direction, CMap &cmap)
{
	if( direction == "up")
		if(!ScanMapCollision(y - 1, x, cmap, false))
		{
			return false;
		}
		else
		{
			y--;
			return true;
		}
	else if(direction == "down")
		if(!ScanMapCollision(y + 1, x, cmap, false))
		{
			return false;
		}
		else
		{
			y++;
			return true;
		}
	else if(direction == "left")
		if(!ScanMapCollision(y, x - 1, cmap, false))
		{
			return false;
		}
		else
		{
			x--;
			return true;
		}
	else if(direction == "right")
		if(!ScanMapCollision(y, x + 1, cmap, false))
		{
			return false;
		}
		else
		{
			x++;
			return true;
		}
}
void Combat(GameObject &player, GameObject &NPC, CMap &game_map);


bool first_turn = true;
/*

								------------------------------------------
								|				MAIN START				 |
								------------------------------------------

*/
int main()
{
	WINDOW *win;
	
	int rows;
	int cols;

	win = initscr();
	
	start_color();
	raw();
	keypad(win, TRUE);
	noecho();
	
	refresh();
	getmaxyx(win, rows, cols);
	GameObject player('@', rows/2, cols/2, 0);
	player.money = 0;
	refresh();
	int ch;
	CMap game_map;
	game_map.LoadMap();
	//game_map.LoadItemMap();
	vector<GameObject> MonsterList;
	
	srand(time(NULL));
	//item generation
	int counter;
	int max_items = 20;
	for(counter = 1; counter < max_items;)
	{
		int y = rand() % 20 + 2;
		int x = rand() % 80;
		int type = 0;
		ScanMapCollision(y, x, game_map, false);
		if(game_map.TileMap[LastTileScanned].passable == true && game_map.TileMap[LastTileScanned].item == "")
		{
			counter++;
			switch(type)
			{
			case 0:
				game_map.TileMap[LastTileScanned].item = "money";
				game_map.TileMap[LastTileScanned].sym = '$';
				break;
			default:
				break;
			}

		}
	}
	//mob generation
	int max_npc = 19;
	for(int i = 0; i < max_npc;)
	{
		int randy = rand() % 20+2;
		int randx = rand() % 75;
		int type = rand() % 3;
		ScanMapCollision(randy, randx, game_map, false);
		if(game_map.TileMap[LastTileScanned].passable == true && game_map.TileMap[LastTileScanned].HasMonster == false && game_map.TileMap[LastTileScanned].HasPlayer == false)
		{
			switch(type)
			{
			case 0:			
				MonsterList.push_back(GameObject('z', randy, randx, 0));
				MonsterList[i].CurrentTile = LastTileScanned;
				break;
			case 1:
				MonsterList.push_back(GameObject('p', randy, randx, 0));
				MonsterList[i].CurrentTile = LastTileScanned;
				break;
			case 2:
				MonsterList.push_back(GameObject('s', randy, randx, 0));
				MonsterList[i].CurrentTile = LastTileScanned;
			default:
				break;
			}
			i++;
		}
	}
	ScanMapCollision(player.y, player.x, game_map, false);
	player.CurrentTile = LastTileScanned;
	bool PlayerHasMoved = false;
	clear();
	int TotalMoves = 0;
	std::vector<GameObject>::iterator MonsterListItr;
	while(!player.dead)
	{
		
		if(first_turn)
		{
			player.GetFOV(game_map);
			move(0,0);
			init_pair(1,COLOR_GREEN, COLOR_BLACK);
			attron(COLOR_PAIR(1) | A_BOLD);
			printw("Welcome to Roguelike #1 by Jesse");
			attroff(COLOR_PAIR(1) | A_BOLD);
			first_turn = false;
			game_map.DrawMap();
			game_map.DrawItems();
			move(23, 0);
			printw("Sindlei the Champion\tHP %i/%i\t $:%i\n", player.cur_hp, player.max_hp, player.money);
			printw("Caverns of Diminz\t\t\t Moves: %i", TotalMoves);
			for(int i = 0; i < MonsterList.size(); i++)
			{
				if(game_map.TileMap[MonsterList[i].CurrentTile].Visible == true && MonsterList[i].dead == false)
				{
					attron(A_BOLD);
					mvprintw(MonsterList[i].y, MonsterList[i].x, "%c", MonsterList[i].ch);
					attroff(A_BOLD);
				}
			}

			//draws the @ position of  player
			move(player.y, player.x);
			attron(A_BOLD);
			printw("%c", player.ch);
			move(player.y, player.x);
			attroff(A_BOLD);
		}
		if(PlayerHasMoved == true)
		{
			TotalMoves++;
			//regen
			if(TotalMoves%6 == 0 && player.cur_hp != player.max_hp)
				player.cur_hp++;
			clear();
			//char msg = GlobalMessage.c_str();
			
			player.GetFOV(game_map);
			game_map.DrawMap();
			game_map.DrawItems();
			for(int i = 0; i < MonsterList.size(); i++)
			{
				if(game_map.TileMap[MonsterList[i].CurrentTile].Visible == true && MonsterList[i].dead == false)
				{
					attron(A_BOLD);
					mvprintw(MonsterList[i].y, MonsterList[i].x, "%c", MonsterList[i].ch);
					attroff(A_BOLD);
				}
			}
			
			
			

			//draws the @ position of  player
			move(player.y, player.x);
			attron(A_BOLD);
			printw("%c", player.ch);
			move(player.y, player.x);
			attroff(A_BOLD);
			PlayerHasMoved = false;
		}
		move(23, 0);
			printw("Sindlei the Champion\tHP %i/%i\t $:%i\n", player.cur_hp, player.max_hp, player.money);
			printw("Caverns of Diminz\t\t\t Moves: %i", TotalMoves);
		GlobalMessage = GlobalStream.str();
		//mvaddstr(0, 0, GlobalMessage.c_str());
		
		//wait for input
		ch = getch();
		switch(ch)
		{
		case KEY_UP:
			if(player.move("up", game_map))
			{
				player.PrevTile = player.CurrentTile;
				player.CurrentTile = LastTileScanned;
				game_map.TileMap[player.CurrentTile].HasPlayer = true;
				game_map.TileMap[player.PrevTile].HasPlayer = false;
				PlayerHasMoved = true;
				break;
			}
			else
				break;
			
		case KEY_DOWN:
			if(player.move("down", game_map))
			{
				player.PrevTile = player.CurrentTile;
				player.CurrentTile = LastTileScanned;
				game_map.TileMap[player.CurrentTile].HasPlayer = true;
				game_map.TileMap[player.PrevTile].HasPlayer = false;
				PlayerHasMoved = true;
				break;
			}
			else
				break;
			
		case KEY_LEFT:
			if(player.move("left", game_map))
			{
				player.PrevTile = player.CurrentTile;
				player.CurrentTile = LastTileScanned;
				game_map.TileMap[player.CurrentTile].HasPlayer = true;
				game_map.TileMap[player.PrevTile].HasPlayer = false;
				PlayerHasMoved = true;
				break;
			}
			else
				break;
		
		case KEY_RIGHT:
			if(player.move("right", game_map))
			{
				player.PrevTile = player.CurrentTile;
				player.CurrentTile = LastTileScanned;
				game_map.TileMap[player.CurrentTile].HasPlayer = true;
				game_map.TileMap[player.PrevTile].HasPlayer = false;
				PlayerHasMoved = true;
				break;
			}
			else
				break;
			
		case 'r':
			//GlobalStream << "You rest a moment...";
			PlayerHasMoved = true;
			break;
		case 'e':
			//use key
			if(game_map.TileMap[player.CurrentTile].item == "money")
			{
				player.money++;
				game_map.TileMap[player.CurrentTile].sym = game_map.TileMap[player.CurrentTile].base_sym;
				game_map.TileMap[player.CurrentTile].item = "";
				break;
			}
			else
				break;
		default:
			break;
		}

		

		//crude check to see if current tile has money
		//TODO: create seperate function and expand
		
		int random;

		//NPC Monster roam function
		//TODO: seperate out and then expand into different roam methods and add aggro
		
		if(PlayerHasMoved == true)
		{
		for(int m = 0; m < MonsterList.size();)
		{			
			random = rand() % 6;
			MonsterList[m].PrevTile = MonsterList[m].CurrentTile;
			
			switch(random)
			{
				case 0:
					//make sure the NPC hasn't tried to move in the same direction for too long
					if(MonsterList[m].ct_up < MonsterList[m].ct_limit && MonsterList[m].dead == false)
					{
						if(MonsterList[m].move("up", game_map))
						{
							MonsterList[m].CurrentTile = LastTileScanned;
							game_map.TileMap[MonsterList[m].CurrentTile].HasMonster = true;
							game_map.TileMap[MonsterList[m].PrevTile].HasMonster = false;
						}
						
						MonsterList[m].ct_up++;									//increment the current move direction
						MonsterList[m].ct_down = 0;								//reset the rest
						MonsterList[m].ct_left = 0;
						MonsterList[m].ct_right= 0;
						//MonsterList[m].CurrentTile = LastTileScanned;			//set the current tile of monster for later functions to utilize.
						m++;
						break;
					}
					else
						break;
				case 1:
					if(MonsterList[m].ct_down < MonsterList[m].ct_limit && MonsterList[m].dead == false)
					{
						if(MonsterList[m].move("down", game_map))
						{
							MonsterList[m].CurrentTile = LastTileScanned;
							game_map.TileMap[MonsterList[m].CurrentTile].HasMonster = true;
							game_map.TileMap[MonsterList[m].PrevTile].HasMonster = false;
						}
						
						MonsterList[m].ct_down++;
						MonsterList[m].ct_up = 0;
						MonsterList[m].ct_left = 0;
						MonsterList[m].ct_right= 0;
						//MonsterList[m].CurrentTile = LastTileScanned;
						m++;
						break;
					}
					else
						break;
				case 2:
					if(MonsterList[m].ct_left < MonsterList[m].ct_limit && MonsterList[m].dead == false)
					{
						if(MonsterList[m].move("left", game_map))
						{
							MonsterList[m].CurrentTile = LastTileScanned;
							game_map.TileMap[MonsterList[m].CurrentTile].HasMonster = true;
							game_map.TileMap[MonsterList[m].PrevTile].HasMonster = false;
						}
			
						MonsterList[m].ct_left++;
						MonsterList[m].ct_down = 0;
						MonsterList[m].ct_up = 0;
						MonsterList[m].ct_right= 0;
						//MonsterList[m].CurrentTile = LastTileScanned;
						m++;
						break;
					}
					else
						break;
				case 3:
					if(MonsterList[m].ct_right < MonsterList[m].ct_limit && MonsterList[m].dead == false)
					{
						if(MonsterList[m].move("right", game_map))
						{
							MonsterList[m].CurrentTile = LastTileScanned;
							game_map.TileMap[MonsterList[m].CurrentTile].HasMonster = true;
							game_map.TileMap[MonsterList[m].PrevTile].HasMonster = false;
						}
						MonsterList[m].ct_right++;
						MonsterList[m].ct_down = 0;
						MonsterList[m].ct_left = 0;
						MonsterList[m].ct_up= 0;
						
						m++;
						break;
					}
					else
						break;
				case 4://just stand still
					m++;
					break;
				case 5://just stand still
					m++;
					break;
				default:
					move(23, 0);
					printw("error monster move\n");
					getch();
					mvprintw(23,0,"random %i ------", random);
					getch();
					m++;
					break;
			}
			//ugly hack to prevent assertion error
			//due to not incrementing m in the for-loop declaration
			if(m > MonsterList.size() - 1)
				;
			
		
			else
			{
				//crude check to see if current tile has money
				//TODO: create seperate function and expand
				if(game_map.TileMap[MonsterList[m].CurrentTile].item == "money")
				{
					MonsterList[m].money++;
					GlobalStream << MonsterList[m].ch << " picked up some money\t\t\t";
					//mvprintw(0, 0, "%c picked up some money.\t\t\t", MonsterList[m].ch);
					game_map.TileMap[MonsterList[m].CurrentTile].sym = game_map.TileMap[MonsterList[m].CurrentTile].base_sym;
					game_map.TileMap[MonsterList[m].CurrentTile].item = "";
				}
				else
					;
			}
			
			/*if(GlobalCheckMobCollision)
			{
			ScanMobCollision(game_map, player, MonsterList);
			}*/
		
		}
		}//end IF LOOP MAIN
		
			if(GlobalCheckMobCollision)
			{
				ScanMobCollision(game_map, player, MonsterList);
			}
	}

		
	//is it worth putting this here?
	refresh();
	getch();
	endwin();
	return 0;
}
bool ScanMobCollision(CMap &cmap, GameObject &player, vector<GameObject> &MonsterList)
{
	for(int m = 0; m < MonsterList.size(); m++)
		if(MonsterList[m].CurrentTile == LastTileScanned)
		{
			Combat(player, MonsterList[m], cmap);
			GlobalCheckMobCollision = false;
			return false;
		}
}

bool ScanMapCollision(int y, int x, CMap &cmap, bool FOVCheck)
{
	for(int i = 0; i < cmap.TileMap.size(); i++)
	{
		if(cmap.TileMap[i].x == x && cmap.TileMap[i].y == y)
		{
			LastTileScanned = i;
			if(FOVCheck)
			{
				if(cmap.TileMap[i].passable == true)
					return true;
				else if(cmap.TileMap[i].passable == false)
					return false;
			}
			else if(cmap.TileMap[i].HasMonster == true || cmap.TileMap[i].HasPlayer == true)
			{
				GlobalCheckMobCollision = true;
				return false;
			}
			else if(cmap.TileMap[i].passable == true)
				return true;
			else if(cmap.TileMap[i].passable == false)
				return false;
		}
	}
}

void Combat(GameObject &player, GameObject &NPC, CMap &game_map)
{
	//player attack rolls
	int random;
	random = rand() % 7;
	int attack;
	attack = random * player.level;

	//npc attack rolls
	int random_npc;
	random_npc = rand() % 5;
	int attack_npc;
	attack_npc = random_npc * NPC.level;

	//adjust stats
	player.cur_hp -= attack_npc;
	NPC.cur_hp -= attack;
	GlobalStream << "You hit " << NPC.ch << " for " << attack << ", " << NPC.ch << " hits you for " << attack_npc;
	if(NPC.cur_hp <= 0)
	{
		GlobalStream << "..." << NPC.ch << " dies.";
		mvprintw(0, 0, "you killed it");
		NPC.dead = true;
		game_map.TileMap[LastTileScanned].HasMonster = false;
	}
	if(player.cur_hp <= 0)
	{
		mvprintw(0, 0, "YOU DIED!\npress q to quit");
		char ch;
		ch = getch();
		switch(ch)
		{
		case 'q':
			player.dead = true;
			break;
		}

	}
}

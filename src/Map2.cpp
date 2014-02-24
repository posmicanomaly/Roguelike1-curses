#include "Map.h"
GameMap::GameMap()
{
	LastTileScanned = 0;
	BoundsRegistered = false;
}

GameMap::GameMap(string FileName, ifstream &file_in)
{
	mvprintw(0, 0, "Loading Map %s\n", FileName.c_str());
	refresh();
	//
	file_in.open(FileName);
	if(file_in.fail())
		printw("Error opening map\n");
	else
		printw("Map opened\n");
	getline(file_in, MapName);
	getline(file_in, MapStyle);

	string temp;
	while(!file_in.eof())
	{
		getline(file_in, temp);
		RawMap.push_back(temp);
	}
	file_in.close();

	//ANALYZE COMMANDS
	//
	string RawCommand;
	string *CurrentCommand;
	//argumentpos is our switch to determine which string we're reading into
	int ArgumentPos = 0;
	string TileType;
	string Message;
	string Flag;
	string Flag2;
	//
	int TileIDCounter = 0;
	int BracketEndPos = 0;
	//
	int Y = 0;
	int X = 0;
	for(int Row = 0; Row < RawMap.size(); Row++)
	{
		Y = Row;
		X = 0;
		for(int Ch = 0; Ch < RawMap[Row].size(); Ch++)
		{
			switch(RawMap[Row][Ch])
			{
			case '[':
				for(int Cursor = Ch; Cursor < RawMap[Row].size(); Cursor++)
				{
					if(RawMap[Row][Cursor] == ']')
					{
						//end
						BracketEndPos = Cursor;
						break;
					}
				}
				//first stage
				for(int a = Ch + 1; a < BracketEndPos; a++)
					RawCommand.push_back(RawMap[Row][a]);
				//second stage
				for(int CmdCh = 0; CmdCh < RawCommand.size(); CmdCh++)
				{
					//argumentpos incremented when , is seen
					switch(ArgumentPos)
					{
					case 0:
						CurrentCommand = &TileType;
						break;
					case 1:
						CurrentCommand = &Message;
						break;
					case 2:
						CurrentCommand = &Flag;
						break;
					case 3:
						CurrentCommand = &Flag2;
						break;
					}
					//

					//
					switch(RawCommand[CmdCh])
					{
					case 0:
						break;
					case ',':
						ArgumentPos++;
						break;
					default:
						CurrentCommand->push_back(RawCommand[CmdCh]);
						break;
					}
				}
				//MAKE THE TILES HERE
				int RealX = X + 12;//stupid shim
				TileMap.push_back(Tile(TileIDCounter, Y, X, TileType, Message, Flag, Flag2));
				if(Flag == "EntranceExit" || Flag2 == "Top Level" || Flag == "Up Stairs")
					StorePlayerPos(Y, RealX);//stupid shim
				TileIDCounter++;
				X++;
				//MAKE THE TILES HERE
				//reset all
				TileType.clear();
				Message.clear();
				Flag.clear();
				Flag2.clear();
				RawCommand.clear();
				ArgumentPos = 0;
				//set Ch to end of last command found to keep going
				Ch = BracketEndPos;
				break;
			}//end main switch
		}//end main for loop
	}
	//MONSTERS HERE???
	/*for(int i = 0; i < 25; i++)
	{
		int randx = rand() % 66 + 12;
		int randy = rand() % 22;
		while(randx > 66)
			randx = rand() %66 + 12;
		ScanTileByCoord(randy, randx);
		if(TileMap[LastTileScanned].passable && !TileMap[LastTileScanned].HasNPC && !TileMap[LastTileScanned].HasPlayer)
			MonsterList.push_back(Monster(randy, randx, "Gnoll", 1, 0));
	}*/



	////load RawMap into TileMap
	//int counter = 0;
	//for(int i = 0; i < RawMap.size(); i++)
	//	for(int j = 0; j < RawMap[i].size(); j++)
	//	{
	//		switch(RawMap[i][j])
	//		{
	//		case '>':
	//			TileMap.push_back(Tile(counter, '>', i, j, "upstairs", true, false));
	//			StorePlayerPos(TileMap[counter].y, TileMap[counter].x);
	//			break;
	//		case '#':
	//			TileMap.push_back(Tile(counter, '#', i, j, "wall", false, true));
	//			break;
	//		case '*':
	//			TileMap.push_back(Tile(counter, '*', i, j, "bounding wall", false, true));
	//			break;
	//		case ' ':
	//			TileMap.push_back(Tile(counter, ' ', i, j, "wall", false, true));
	//			break;
	//		case '-':
	//			TileMap.push_back(Tile(counter, '-', i, j, "wall", false, true));
	//			break;
	//		case '|':
	//			TileMap.push_back(Tile(counter, '|', i, j, "wall", false, true));
	//			break;
	//		case 'H':
	//			TileMap.push_back(Tile(counter, 'H', i, j, "hall", true, false));
	//			break;
	//		case '.':
	//			TileMap.push_back(Tile(counter, '.', i, j, "floor", true, false));
	//			break;
	//		case 'W':
	//			TileMap.push_back(Tile(counter, '=', i, j, "water", false, false));
	//			break;
	//		case 'L':
	//			TileMap.push_back(Tile(counter, '~', i, j, "lava", false, false));
	//			break;
	//		//OVERWORLD
	//		case 'M':
	//			TileMap.push_back(Tile(counter, '^', i, j, "mountain", false, true));
	//			break;
	//		case 'B':
	//			TileMap.push_back(Tile(counter, '#', i, j, "Bridge", true, false));
	//			break;
	//		case '~':
	//			TileMap.push_back(Tile(counter, '~', i, j, "Hills", true, false));
	//			break;
	//		case 'C':
	//			TileMap.push_back(Tile(counter, 'C', i, j, "cave", true, false));
	//			break;
	//		case 'T':
	//			TileMap.push_back(Tile(counter, 'o', i, j, "town", true, false));
	//			break;
	//		case '`':
	//			TileMap.push_back(Tile(counter, '"', i, j, "Grass Plain", true, false));
	//			break;
	//		case '@':
	//			TileMap.push_back(Tile(counter, '"', i, j, "spawn", true, false));
	//			//StorePlayerPos(TileMap[counter].y, TileMap[counter].x);
	//			break;
	//		case '&':
	//			TileMap.push_back(Tile(counter, '&', i, j, "Forest", true, false));
	//			break;
	//		case 'F':
	//			TileMap.push_back(Tile(counter, ' ', i, j, "Open Field", true, false));
	//			break;
	//		case 'p':
	//			TileMap.push_back(Tile(counter, ' ', i, j, "Trail", true, false));
	//			break;
	//		case ',':
	//			TileMap.push_back(Tile(counter, ',', i, j, "Cut Grass", true, false));
	//			break;
	//		case '1':
	//			TileMap.push_back(Tile(counter, 'o', i, j, "Hamlet", true, false));
	//			StorePlayerPos(TileMap[counter].y, TileMap[counter].x);
	//			break;
	//		default:
	//			TileMap.push_back(Tile(counter, '%', i, j, "ERROR", true, false));
	//			break;
	//		}
	//		counter++;
	//	}
}

void GameMap::DrawMap()
{
	//colors
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(10, COLOR_GREEN, COLOR_BLACK);
	//hill
	init_pair(11, COLOR_YELLOW, COLOR_BLACK);	
	//new grass
	init_pair(12, COLOR_BLACK, COLOR_GREEN);
	//new water
	init_pair(13, COLOR_BLACK, COLOR_BLUE);

	//end colors
	//1 blue
	//2 red
	//3 yellow
	for(int i = 0; i < TileMap.size(); i++)
	{
		//TileMap[i].Explored = true;
		//If the tile is in field of view, set A_BOLD as default
		//if(TileMap[i].Visible)
			//attron(A_BOLD);
		//Draw if Explored
		if(TileMap[i].Explored == true)
		{
			move(TileMap[i].y, TileMap[i].x);
			
			if(TileMap[i].type == "water")
			{
				attron(COLOR_PAIR(1));
				//printw(" ");
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(1));
			}
			else if(TileMap[i].type == "lava")
			{
				attron(COLOR_PAIR(2));
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(2));
			}
			else if(TileMap[i].type == "Floor")
			{
				if(MapStyle == "Town")
				{
					attron(COLOR_PAIR(10));
					addch(ACS_BULLET);
					attroff(COLOR_PAIR(10));
				}
				else
					addch(ACS_BULLET);
			}
			else if(TileMap[i].type == "Wall")
			{
				//attron(A_BOLD);
				if(TileMap[i].sym == '-')
					addch(ACS_HLINE);
				else if(TileMap[i].sym == '|')
					addch(ACS_VLINE);
				else if(TileMap[i].sym == '#')
					printw("%c", TileMap[i].sym);
				//attroff(A_BOLD);
			}
			else if(TileMap[i].type == "hall")
			{
				addch(ACS_BOARD);
			}
			else if(TileMap[i].type == "Bridge")
			{
				addch(ACS_BLOCK | COLOR_PAIR(11));
			}
			else if(TileMap[i].type == "mountain")
			{
				attron(A_BOLD);
				printw("%c", TileMap[i].sym);
				attroff(A_BOLD);
			}
			else if(TileMap[i].type == "Town")
			{
				printw("%c", TileMap[i].sym);
			}
			else if(TileMap[i].type == "Hamlet")
			{
				printw("%c", TileMap[i].sym);
			}
			else if(TileMap[i].type == "Cave")
			{
				addch(ACS_LANTERN);
			}
			else if(TileMap[i].type == "Up Stairs")
			{
				printw("%c", TileMap[i].sym);
			}
			else if(TileMap[i].type == "Down Stairs")
			{
				printw("%c", TileMap[i].sym);
			}
			else if(TileMap[i].type == "spawn")
			{
				printw("%c", TileMap[i].sym);
			}
			else if(TileMap[i].type == "Forest" || TileMap[i].type == "Grass Plain")
			{
				attron(COLOR_PAIR(10));
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(10));
			}
			else if(TileMap[i].type == "Open Field")
			{
				attron(COLOR_PAIR(10));
				addch(ACS_BULLET);
				attroff(COLOR_PAIR(10));
			}
			else if(TileMap[i].type == "Hills")
			{
				attron(COLOR_PAIR(11));
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(11));
			}
			else if(TileMap[i].type == "Trail")
			{
				attron(COLOR_PAIR(11));
				addch(ACS_BULLET);
				attroff(COLOR_PAIR(11));
			}
			else if(TileMap[i].type == "Cut Grass")
			{
				attron(COLOR_PAIR(10));
				printw("%c", TileMap[i].sym);
				attroff(COLOR_PAIR(10));
			}
			else
				printw("%c", TileMap[i].sym);
		}
		attroff(A_BOLD);
	}
}

int GameMap::ScanTileByCoord(int y, int x)
{
	for(int i = y * 66; i < TileMap.size(); i++)
	{
		if(TileMap[i].x == x && TileMap[i].y == y)
		{
			LastTileScanned = i;
			return i;
		}
	}
	return 0;
}

bool GameMap::CheckCollision(int Dest)
{
	if(Dest > TileMap.size())
		return false;
	if(Dest < 0)
		return false;
	if(TileMap[Dest].passable == true)
		return false;
	else if(TileMap[Dest].passable == false)
		return true;
}

bool GameMap::CheckCollisionWithWall(int Dest)
{
	if(Dest > TileMap.size())
		return false;
	if(Dest < 0)
		return false;
	if(TileMap[Dest].IsWall)
		return true;
	else
		return false;
}

bool GameMap::CheckCollisionWithNPC(int Dest)
{
	if(TileMap[Dest].HasNPC == true)
		return true;
	if(TileMap[Dest].HasNPC == false)
		return false;
}

bool GameMap::CheckCollisionWithPlayer(int Dest)
{
	if(TileMap[Dest].HasPlayer == true)
		return true;
	if(TileMap[Dest].HasPlayer == false)
		return false;
}

bool GameMap::Bresenham(int y1, int x1, int const y2, int const x2)
{
	int delta_x(x2 - x1);
	signed char const ix((delta_x > 0) - (delta_x < 0));
	delta_x = abs(delta_x) << 1;

	int delta_y(y2 - y1);
	signed char const iy((delta_y > 0) - (delta_y < 0));
	delta_y = abs(delta_y) << 1;

	if(!CheckCollisionWithWall(ScanTileByCoord(y1, x1)))
	{
		TileMap[LastTileScanned].Visible = true;
		TileMap[LastTileScanned].Explored = true;
	}
	else
	{
		TileMap[LastTileScanned].Visible = true;
		TileMap[LastTileScanned].Explored = true;
		return false;
	}

	if(delta_x >= delta_y)
	{
		int error(delta_y - (delta_x >> 1));

		while(x1 != x2)
		{
			if((error >= 0) && (error || (ix > 0)))
			{
				error -= delta_x;
				y1 += iy;
			}
			// else do nothing

			error += delta_y;
			x1 += ix;

			if(!CheckCollisionWithWall(ScanTileByCoord(y1, x1)))
			{
				TileMap[LastTileScanned].Visible = true;
				TileMap[LastTileScanned].Explored = true;
			}
			else
			{
				TileMap[LastTileScanned].Visible = true;
				TileMap[LastTileScanned].Explored = true;
				return false;
			}
		}
	}
	else
	{
		int error(delta_x - (delta_y >> 1));

		while(y1 != y2)
		{
			if((error >= 0) && (error || (iy > 0)))
			{
				error -= delta_y;
				x1 += ix;
			}

			error += delta_x;
			y1 += iy;

			if(!CheckCollisionWithWall(ScanTileByCoord(y1, x1)))
			{
				TileMap[LastTileScanned].Visible = true;
				TileMap[LastTileScanned].Explored = true;
			}
			else
			{
				TileMap[LastTileScanned].Visible = true;
				TileMap[LastTileScanned].Explored = true;
				return false;
			}
		}
	}
	return true;
}
vector<int> GameMap::BresenhamNPC(int y1, int x1, int const y2, int const x2)
{
	vector<int> tempvistiles;
	int delta_x(x2 - x1);
	signed char const ix((delta_x > 0) - (delta_x < 0));
	delta_x = abs(delta_x) << 1;

	int delta_y(y2 - y1);
	signed char const iy((delta_y > 0) - (delta_y < 0));
	delta_y = abs(delta_y) << 1;

	if(!CheckCollisionWithWall(ScanTileByCoord(y1, x1)))
	{
		tempvistiles.push_back(LastTileScanned);
		//TileMap[LastTileScanned].Visible = true;
		//TileMap[LastTileScanned].Explored = true;
	}
	else
	{
		tempvistiles.push_back(LastTileScanned);
		//TileMap[LastTileScanned].Visible = true;
		//TileMap[LastTileScanned].Explored = true;
		//return false;
		return tempvistiles;
	}

	if(delta_x >= delta_y)
	{
		int error(delta_y - (delta_x >> 1));

		while(x1 != x2)
		{
			if((error >= 0) && (error || (ix > 0)))
			{
				error -= delta_x;
				y1 += iy;
			}
			// else do nothing

			error += delta_y;
			x1 += ix;

			if(!CheckCollisionWithWall(ScanTileByCoord(y1, x1)))
			{
				tempvistiles.push_back(LastTileScanned);
				//TileMap[LastTileScanned].Visible = true;
				//TileMap[LastTileScanned].Explored = true;
			}
			else
			{
				tempvistiles.push_back(LastTileScanned);
				//TileMap[LastTileScanned].Visible = true;
				//TileMap[LastTileScanned].Explored = true;
				//return false;
				break;
			}
		}
	}
	else
	{
		int error(delta_x - (delta_y >> 1));

		while(y1 != y2)
		{
			if((error >= 0) && (error || (iy > 0)))
			{
				error -= delta_y;
				x1 += ix;
			}

			error += delta_x;
			y1 += iy;

			if(!CheckCollisionWithWall(ScanTileByCoord(y1, x1)))
			{
				tempvistiles.push_back(LastTileScanned);
				//TileMap[LastTileScanned].Visible = true;
				//TileMap[LastTileScanned].Explored = true;
			}
			else
			{
				tempvistiles.push_back(LastTileScanned);
				//TileMap[LastTileScanned].Visible = true;
				//TileMap[LastTileScanned].Explored = true;
				//return false;
				break;
			}
		}
	}
	return tempvistiles;
}

void GameMap::StorePlayerPos(int &y, int &x)
{
	StoredPlayerY = y;
	StoredPlayerX = x;
}

void GameMap::LoadPlayerPos(int &y, int &x)
{
	y = StoredPlayerY;
	x = StoredPlayerX;
}
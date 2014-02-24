#ifndef MESSAGE_H
#define MESSAGE_H

#include <sstream>
#include <vector>
#include <curses.h>
using namespace std;

class Message{
public:
	Message();
	~Message();
	
	void DrawUIMessage();
	void DrawUIMessageHistory();

	void DrawBorderedWindow(int yc, int xc, int ysize, int xsize, string title);
	WINDOW *Window;

	void WelcomeScreen();

	stringstream MessageStream;
	vector<string> UIMessageBuffer;
};

#endif

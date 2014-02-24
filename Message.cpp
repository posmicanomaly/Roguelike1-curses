#include "Message.h"

void Message::DrawUIMessage()
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
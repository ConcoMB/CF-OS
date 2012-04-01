#include <curses.h>
#include <stdlib.h>
#include "strQueue.h"

int r, c, nrow, ncol;
WINDOW * window;
extern strQueue_t queue;

void initWindow()
{
	window = initscr();
	getmaxyx(window, nrow, ncol);
	clear();
	refresh();
	printw(
",------. ,---.  ,--.  ,--.,--------. ,---.   ,---.,--.   ,--.    ,--.   ,------.  ,---.   ,----.   ,--. ,--.,------. \n"
"|  .---'/  O  \\ |  ,'.|  |'--.  .--'/  O  \\ '   .-'\\  `.'  /     |  |   |  .---' /  O  \\ '  .-./   |  | |  ||  .---' \n"
"|  `--,|  .-.  ||  |' '  |   |  |  |  .-.  |`.  `-. '.    /      |  |   |  `--, |  .-.  ||  | .---.|  | |  ||  `--,  \n"
"|  |`  |  | |  ||  | `   |   |  |  |  | |  |.-'    |  |  |       |  '--.|  `---.|  | |  |'  '--'  |'  '-'  '|  `---.\n"
"`--'   `--' `--'`--'  `--'   `--'  `--' `--'`-----'   `--'       `-----'`------'`--' `--' `------'  `-----' `------' \n");
	refresh();
}

void* printThread(void* arg1)
{
	initWindow();
	echo();
	cbreak();
	char * string;
	while(1)
	{
		if(!isEmpty(queue))
		{
			printw("%s", dequeueStr(queue));
			refresh();
		}
	}
	endwin();
	pthread_exit(0);
}
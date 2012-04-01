#include <curses.h>
#include <stdlib.h>
#include "strQueue.h"

int r, c, nrow, ncol;
WINDOW * window;
extern strQueue_t queue;

void printTitle()
{
	move(0,0);
		printw(
",------. ,---.  ,--.  ,--.,--------. ,---.   ,---.,--.   ,--.    ,--.   ,------.  ,---.   ,----.   ,--. ,--.,------. \n"
"|  .---'/  O  \\ |  ,'.|  |'--.  .--'/  O  \\ '   .-'\\  `.'  /     |  |   |  .---' /  O  \\ '  .-./   |  | |  ||  .---' \n"
"|  `--,|  .-.  ||  |' '  |   |  |  |  .-.  |`.  `-. '.    /      |  |   |  `--, |  .-.  ||  | .---.|  | |  ||  `--,  \n"
"|  |`  |  | |  ||  | `   |   |  |  |  | |  |.-'    |  |  |       |  '--.|  `---.|  | |  |'  '--'  |'  '-'  '|  `---.\n"
"`--'   `--' `--'`--'  `--'   `--'  `--' `--'`-----'   `--'       `-----'`------'`--' `--' `------'  `-----' `------' \n"
"=====================================================================================================================\n");
	//move(nrow-4, 0);
	//printw("=====================================================================================================================\n");
	refresh();
}

void initWindow()
{
	window = initscr();
	getmaxyx(window, nrow, ncol);
	clear();
	printTitle();
	scrollok(window, 1);

	
}

void scrollDown()
{
	scroll(window);
	printTitle();
	move(nrow-4, 0);
	printw("=====================================================================================================================\n");
	move(nrow-5, 0);
}

void* printThread(void* arg1)
{
	initWindow();
	int i=0;
	move(nrow-4, 0);
	printw("=====================================================================================================================\n");
	refresh();
	char * string;
	move(nrow-3, 0);
	echo();
	scanw("&d" ,&i);
	getyx(window, r, c);
	move(r-1, c);
	deleteln();

	move(6,0);
	while(1)
	{
		sleep(1);
		wprintw(window,"aaaaaaaaaaaaaa%d\n", i);
		getyx(window, r,c);
		i++;
		if(r==nrow-3){
			scrollDown();
		}
		refresh();
	}
	endwin();
	pthread_exit(0);
}
#include "printer.h"

int r, c, nrow, ncol;
WINDOW * window;

void printTitle()
{
	int i;
	move(0,0);
	alignement();
	printw(",------. ,---.  ,--.  ,--.,--------. ,---.   ,---.,--.   ,--.    ,--.   ,------.  ,---.   ,----.   ,--. ,--.,------. \n");
	alignement();
	printw("|  .---'/  O  \\ |  ,'.|  |'--.  .--'/  O  \\ '   .-'\\  `.'  /     |  |   |  .---' /  O  \\ '  .-./   |  | |  ||  .---' \n");
	alignement();
	printw("|  `--,|  .-.  ||  |' '  |   |  |  |  .-.  |`.  `-. '.    /      |  |   |  `--, |  .-.  ||  | .---.|  | |  ||  `--,  \n");
	alignement();
	printw("|  |`  |  | |  ||  | `   |   |  |  |  | |  |.-'    |  |  |       |  '--.|  `---.|  | |  |'  '--'  |'  '-'  '|  `---.\n");
	alignement();
	printw("`--'   `--' `--'`--'  `--'   `--'  `--' `--'`-----'   `--'       `-----'`------'`--' `--' `------'  `-----' `------' \n");
	for(i=0; i<ncol; i++)
	{
		printw("=");
	}
	refresh();
}

void alignement()
{
	int i;
	for(i=0; i<(ncol-118)/2; i++)
	{
		printw(" ");
	}
}

void initWindow()
{
	window = initscr();
	getmaxyx(window, nrow, ncol);
	clear();
	printTitle();
	printCommandLine();
	scrollok(window, 1);
	echo();
	cbreak();
	r=6;
	move(nrow-3,0);
}

void printCommandLine()
{
	move(nrow-4,0);
	clrtobot();
	move(nrow-4, 0);
	int i;
	for(i=0; i<ncol; i++)
	{
		printw("=");
	}
	printw("\n");
}

void scrollUp()
{
	move(nrow-4,0);
	clrtobot();
	scroll(window);
	printTitle();
	refresh();
	move(nrow-5,0);
}

/*void scan(char* fmt, ...)
{
	va_list ap;
	int rAux=r;
	
	scanw(fmt, string);
	getyx(window, r, c);
	move(r-1, c);
	clrtobot();
	refresh();
	r=rAux;
	
}*/

void printString(char* fmt, ...)
{
	va_list ap;
	char string[300];
	va_start(ap, fmt);
	move(r, 0);
	//printw("r: %d",r);
	if(r==nrow-4)
	{
		scrollUp();
		//getyx(window,r,c);
	}
	else
	{
		r++;
	}
	vsprintf(string, fmt, ap);
	printw("%s", string);
	printCommandLine();
	move(nrow-3, 0);
}

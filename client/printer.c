#include "printer.h"

int r, c, nrow, ncol;
WINDOW * window;

void printTitle()
{
	int i;
	move(0,0);
	alinement();
	printw(",------. ,---.  ,--.  ,--.,--------. ,---.   ,---.,--.   ,--.    ,--.   ,------.  ,---.   ,----.   ,--. ,--.,------. \n");
	alinement();
	printw("|  .---'/  O  \\ |  ,'.|  |'--.  .--'/  O  \\ '   .-'\\  `.'  /     |  |   |  .---' /  O  \\ '  .-./   |  | |  ||  .---' \n");
	alinement();
	printw("|  `--,|  .-.  ||  |' '  |   |  |  |  .-.  |`.  `-. '.    /      |  |   |  `--, |  .-.  ||  | .---.|  | |  ||  `--,  \n");
	alinement();
	printw("|  |`  |  | |  ||  | `   |   |  |  |  | |  |.-'    |  |  |       |  '--.|  `---.|  | |  |'  '--'  |'  '-'  '|  `---.\n");
	alinement();
	printw("`--'   `--' `--'`--'  `--'   `--'  `--' `--'`-----'   `--'       `-----'`------'`--' `--' `------'  `-----' `------' \n");
	for(i=0; i<ncol; i++)
	{
		printw("=");
	}
	printw("\n");
	refresh();
}

void alinement()
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
	r=6;
	move(6,0);
}

void printCommandLine()
{
	move(nrow-5,0);
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
	scroll(window);
	printTitle();
	printCommandLine();
	refresh();
	move(nrow-5,0);
}

void scanInt(int *i)
{
	int rAux=r;
	move(nrow-3, 0);
	scanw("%d", i);
	getyx(window, r, c);
	move(r-1, c);
	clrtobot();
	refresh();
	r=rAux;
	move(r, 0);
}

void scanString(char * s)
{
	int rAux=r;	
	move(nrow-3, 0);
	scanw("%s", s);	
	getyx(window, r, c);
	move(r-1, c);
	insertln();
	clrtobot();
	refresh();
	r=rAux;
	move(r, 0);
}

void printString(char* string)
{
	if(r==nrow-4)
	{
		scrollUp();
	}
	else
	{
		r++;
	}
	printw("%s", string);
	
}
#include "../include/extras.h"
#include "../include/libc.h"


char * quotes[] = {"Maybe I keep seeing you die because I'm supposed to take your place.\nDESMOND HUME\n", 
"You and I both know... you're not supposed to take my place, brother.\nDESMOND HUME\n", 
"You killed us... you killed us all!\nCHARLIE PACE\n",
"I wasn't saving Claire, Charlie... I was saving you. This morning, you dove in after Claire. You tried to save her but you drowned.\nDESMOND HUME\n",
"The rules, rules don't apply to you. You're special. You're uniquely and miraculously special.\nDANIEL FARADAY\n",
"I'll see you in another life, brother.\nDESMOND HUME\n",
"What worries you, masters you.\nJOHN LOCKE\n", 
"Where there is no property there is no injustice. \nJOHN LOCKE\n",
"Things of this world are in so constant a flux, that nothing remains long in the same state. \nJOHN LOCKE\n",
"Our incomes are like our shoes; if too small, they gall and pinch us; but if too large, they cause us to stumble and to trip. \nJOHN LOCKE\n",
"No man's knowledge here can go beyond his experience. \nJOHN LOCKE\n",
"Every man has a property in his own person. This nobody has a right to, but himself.\nJOHN LOCKE\n",
"We have to get off this island... right now. \n DANIEL FARADAY\n",
"I'm your son. \nDANIEL FARADAY\n"}; 

int next=0;

char * get_quote()
{
	int index=rand()%14;
	return quotes[index];
}


int rand(void)
{
	next = next * 1103515245 + 12345;
	return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed)
{
	next = seed;
}

void print_zelda()
{
	printf(" \n"
  "           __ \n"
  "           || \n"
  "           || \n"
  "        ___||___ \n"
  "      _/#______#\\_ \n"
  "     / _/ | || \\_ \\ \n"
  "    |_|   | ||   |_|   \n"
  "          | || \n"
  "          | || \n"
  "          | || \n"
  "     _____|_||_______ \n"
  "    /      /\\     \\  \\ \n"
  "   #      /__\\     #  #  \n"
  "  /      /\\  /\\     \\  \\ \n"
  " #      /__\\/__\\     #  # \n"
  "/_____________________\\__\\ \n \n"
	);
}

void print_mario()
{
	printf(""
	"                __,...__                      \n"
    "            _,-'::::::::`-.                   \n"
    "          ,'::,----._::::::`.                 \n"
    "        ,'::,' /\\,-. \\:::::::`.                \n"
    "       /::::| ;    | |:::::::::`.             \n"
    "       |:::::`._   \\ |:::::::::::\\            \n"
    "     __\\,:--''--`_--':::::::::::::\\           \n"
    "   `'--::__:::::::`-:_:::::::::::::\\          \n"
    "       /--.`'--:_:::::`-::::::::::::\\         \n"
    "      /,-_.'    _`-:_::::`:::::::::::\\        \n"
    "      / /o\\|   ,-_`-.`--:::`::::::::::)       \n"
    "     || \\_/|  / /o\\         `:,'''-:'        \n"
    "   (#__`-_/   | \\_/|        /##|  ,-.\\        \n"
    "  ,''     `-. `.__/       ,'###| /   ||       \n"
    " /           \\      ,    (##### /    ||       \n"
    " |           |___,-##\\     /##/ \\__,'/        \n"
    " \\          /########)    |#,'|__..-'         \n"
    "  `-..__..-'######)          /                \n"
    "     \\`.###'`""'           /                 \n"
    "      \\\\\\__//            ,'                  \n"
    "       \\`--'         _,-'                    \n"
    "        `-..___..--''                        \n");
}

int print3(int argc, char** argv)
{
  int i=0;
  while(1)
  {
    printf("%d\n", i);
    i+=3;
    sleep(2000);
  }
}

int printO(int argc, char** argv)
{
  while(1)
  {
    printf("O");
    sleep(500);
  }
}

int printX(int argc, char** argv)
{
  while(1)
  {
    printf("X");
    sleep(1500);
  }
}

int kbTest(int argc, char** argv)
{
	while(1)
	{
		sleep(300);
		//putchar(getchar());
	}
}

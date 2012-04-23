#include "../include/speaker.h"

char* CODES[36]={
	 ".-",		//A
	 "-...",	//B
	 "-.-.",	//C
	 "-..",		//D
	 ".",		//E
	 "..-.",	//F
	 "--.",		//G
	 "....",	//H
	 "..",		//I
	 ".---",	//J
	 "-.-",		//K
	 ".-..",	//L
	 "--",		//M
	 "-.",		//N
	 "---",		//O
	 ".--.",	//P
	 "--.-",	//Q
	 ".-.",		//R
	 "...",		//S
	 "-",		//T
	 "..-",		//U
	 "...-",	//V
	 ".--",		//W
	 "-..-",	//X
	 "-.--",	//Y
	 "--..",	//Z
	 "-----",	//0
	 ".----",	//1
	 "..---",	//2
	 "...--",	//3
	 "....-",	//4
	 ".....",	//5
	 "-....",	//6
	 "--...",	//7
	 "---..",	//8
	 "----."	//9
      };

void sys_speak(char c)
{
	int index;
	if(c>='0' && c<='9'){
		index=26+c-'0';
	}
	else if(c>='a' && c<='z')
	{
		index=c-'a';
	}
	else if(c>='A' && c<='Z')
	{
		index=c-'A';
	}
	else{
		return;
	}
	int i=0;
	for(i=0; CODES[index][i]; i++){
		if(CODES[index][i]=='-'){
			long_beep();
		}else{
			short_beep();
		}
		timer_wait(500);
	}
	timer_wait(750);
		
}

 void play_sound(int nFrequence) 
 {
 	int Div;
 	char tmp;
  	Div = 1193180 / nFrequence;
 	_IO_out(0x43, 0xb6);
 	_IO_out(0x42, (char) (Div) );
 	_IO_out(0x42, (char) (Div >> 8));
  	tmp = _IO_in(0x61);
  	if (tmp != (tmp | 3)) {
 		_IO_out(0x61, tmp | 3);
 	}
 }
 
 void nosound() 
 {
 	char tmp = (_IO_in(0x61) & 0xFC);
 
 	_IO_out(0x61, tmp);
 }
 
 void short_beep() {
 	 play_sound(440);
 	 timer_wait(150);
 	 nosound();
 }
 
 
 void long_beep() {
 	 play_sound(440);
 	 timer_wait(350);
 	 nosound();
 }
 
 int getCant(char c){
	 return c>>5;
 }
 

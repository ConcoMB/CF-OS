
#include "../include/kb.h"

char ESP_SCAN_CODES[0x51]={'\x1B','@','1','2','3','4','5','6','7','8','9','0','#','*','\x08','\t','q','w','e','r','t','y','u','i','o','p','`','+','\n','@','a','s','d','f','g','h','j','k','l','{','#','}','@','<','z','x','c','v', 'b','n','m',',','.','-','@','@','@',' ','@','1','2','3','4','5','6','7','8','9','0','@','@','@','\x11','@','@','\x12','@','\x12','@','@','\x13'};

char SHIFT_ESP_SCAN_CODES[0x51]={'\x1B','#','!','"','-','$','%','&','/','(',')','=','?','@','\x08','\t','Q','W','E','R','T','Y','U','I','O','P','^','*','\n','@','A','S','D','F','G','H','J','K','L','"','+','@','@','>','Z','X','C','V','B','N','M',';',':','_','@','@','@',' ','@','1','2','3','4','5','6','7','8','9','0','@','@','@','\x11','@','@','\x12','@','\x12','@','@','\x13'};

char ENG_SCAN_CODES[0x51]={'\x1B','`','1','2','3','4','5','6','7','8','9','0','-','=','\x08','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n','@','a','s','d','f','g','h','j','k','l',';','#','\\','@','<','z','x','c','v', 'b','n','m',',','.','/','@','@','@',' ','@','1','2','3','4','5','6','7','8','9','0','@','@','@','\x11','@','@','\x12','@','\x12','@','@','\x13'};

char SHIFT_ENG_SCAN_CODES[0x51]={'\x1B','~', '+', '@','#','$','%','^','&','*','(',')','_','+','\x08','\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n','@','A','S','D','F','G','H','J','K','L',':','"','|','@','>','Z','X','C','V','B','N','M','<','>','?', '@','@','@',' ','@','1','2','3','4','5','6','7','8','9','0','@','@','@','\x11','@','@','\x12','@','\x12','@','@','\x13'};


/*BUFFER CIRCULAR*/
char buffer[BUFFER_SIZE]; 
int head=0;
int tail=0;
char* current_scan_code;
char* current_shifted_scan_code;

void kb_init(){
	sys_set_scancode(LANG_ES);
}
void buffer_putchar(char c)
{
	buffer[head++]=c;
	if(head==BUFFER_SIZE){
		head=0;
	}
	awake();
}

char buffer_getchar_nb()
{
	_Cli();
	//sys_sleep(500);

	if(head==tail || !processHasFocus())
	{
		return 0;
	}
	char next;
	
	next=buffer[tail++];
	if(tail==BUFFER_SIZE){
		tail=0;
	}
	//printf("f(%d)", processHasFocus());
	_Sti();
	return next;
}

char buffer_getchar(){
	_Cli();
	//sys_sleep(500);
	while(!processHasFocus() || head==tail)
	{
		blockInput();
		_Sti();
		//return 0;
		//_sys_yield();
	}
	char next;
	
	next=buffer[tail++];
	if(tail==BUFFER_SIZE){
		tail=0;
	}
	//printf("f(%d)", processHasFocus());
	_Sti();
	return next;
}

/*INT 09 rutine*/

int shift=0;
int caps=0;

void int_09(){
	//_Cli();

	char scanCode=_IO_in(0x60);
	if(scanCode & 0x80){
		/*RELEASED KEY*/
		if(scanCode==(char)0xAA||scanCode==(char)0xB6){
			/*SHIFT*/
			shift=0;
		}
		
	} else {
		/*PRESSED KEY*/
		if(scanCode>=0x3b &&scanCode<=0x42)
		{
			/* FUNCTION KEY*/
			//printf("\nSwap %d\n",scanCode-0x3b);
			//while(1);
			swapTTY(scanCode-0x3b);
			return;
		}
		if(scanCode==0x2a||scanCode==0x36){
			/*SHIFT*/
			shift=1;
		}
		else if(scanCode==0x3A){
			/*CAPS*/
			caps=(caps+1)%2;
			update_leds(caps<<2);
		}
		else if(scanCode!=0x01 && scanCode!=0x0F && scanCode!=0x1D && scanCode!=0x38 && scanCode!=0x53 && scanCode!=0x47 && scanCode!=0x49 && scanCode!=0x4F && scanCode!=0x51){
			/*ORDINARY KEYS*/
			char ascii;
			if(!shift){
				ascii=current_scan_code[(int)scanCode];
				if(caps){
					ascii=to_upper(ascii);
				}
			}
			else{
				ascii=current_shifted_scan_code[(int)scanCode];
				if(caps){
					ascii=to_lower(ascii);
				}
			}
			//awake();
			buffer_putchar(ascii);
		}
	}
	//_Sti();
}

char to_upper(char c){
	if(c>='a'&&c<='z'){
		c=c-'a'+'A';
	}
	return c;
}

char to_lower(char c){
	if(c>='A'&&c<='Z'){
		c=c-'A'+'a';
	}
	return c;
}

void sys_set_scancode(int i){
	if(i==LANG_ES){
		current_scan_code=ESP_SCAN_CODES;
		current_shifted_scan_code=SHIFT_ESP_SCAN_CODES;
	}else if(i==LANG_EN){
		current_scan_code=ENG_SCAN_CODES;
		current_shifted_scan_code=SHIFT_ENG_SCAN_CODES;
	}
	scancode_lang=i;
	printScancode();
}

void update_leds(char status)
{
  while((_IO_in(0x64)&2)!=0)
  {
}
  _IO_out(0x60,0xED);
 
  while((_IO_in(0x64)&2)!=0)
  {
}
  _IO_out(0x60,status);
}

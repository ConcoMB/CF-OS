#include "../include/stdio.h"

int out_stream = 1;

void putchar(char c){
	__write(out_stream,&c,1);
}

void putc(int outstream, char c){
	__write(outstream, &c, 1);
}
char getchar(){
	char c;
	do{
		__read(0,&c,1);
	}while(c==0);
	return c;
}

void prints(char* string){
	int i;
	for(i=0;string[i];i++){
		putchar(string[i]);
	}
}

void speak(char*s)
{
	int i;
	for(i=0; s[i];i++)
	{
		putc(4, s[i]);
	}
}

void printf(char * format, ...)
{	
	char buffer[50];
	va_list args;
	va_start(args, format);
	while(*format!=0)
	{
		if(*format=='%')
		{
			format++;
			switch(*format)
			{
				case 'd':
					prints(itoa(va_arg(args, int), buffer));
					break;	
				case 's':
					prints(va_arg(args,char*));
					break;
				case 'c':
					putchar(va_arg(args,char));
					break;
				case 'X':
					prints(xtoa(va_arg(args, int), buffer));
					break;
				case '%':
					putchar('%');
					break;
			}
		}
		else
		{
			putchar(*format);
		}
	format++;					
	}

	va_end(args);
}
char* gets(char* buffer){
	do{
		*buffer=getchar();
		putchar(*buffer);
	}while(*buffer++!=' '&&*(buffer-1)!='\n');
	*(buffer-1)=0;
	return buffer;
}

int scanf(const char* format, ...){
	char buffer[50];
	int count;
	va_list args;
	va_start(args, format);
	void* p;
	while(*format!=0)
	{
		if(*format=='%')
		{
			format++;
			p=va_arg(args,void*);
			switch(*format)
			{
				case 'c':
					*(char*)p=getchar();
					count++;
					putchar(*(char*)p);
					break;
				case 's':
					gets((char*)p);
					count++;
					break;
				case 'd':
					gets(buffer);
					*(int*)p=atoi(buffer);
					count++;
					break;
			}
		}
	format++;					
	}
	va_end(args);
	return count;
}

void set_out_stream(int i){
	out_stream=i;
}

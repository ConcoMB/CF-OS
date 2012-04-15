#ifndef __LIBC__
#define __LIBC__

void printf(char * format, ...);
void putchar(char c);
void speak(char*c);
void putc(int f, char c);
void set_out_stream(int i);
int stack_count();
int strcmp(char* str1, char* str2);
int substr(char* str1, char* str2);
void strcpy(char* dest, char* src);
char getchar();
char* itoa(int val, char* buffer);
char* xtoa(int mem, char* buffer);
int atoi(char* a);
int pow(int b,int e);
int strlen(char* s);
char gethour();
char getmin();
char* gets(char* buffer);
int scanf(const char* format, ...);
void memcpy(void* dest, void* source, int count);
void* malloc(int bytes);
void* calloc(int bytes);
int free(void* page);
int heap_count();
void set_scancode(int i);
int pageprint(int page);

#endif

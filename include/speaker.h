#ifndef __SPEAKER__
#define __SPEAKER__
#include "kernel.h"
#include "timertick.h"

void sys_speak(char c);
void play_sound(int nFrequence) ;
void nosound();
void short_beep();
void long_beep();
int getCant(char c);
#endif

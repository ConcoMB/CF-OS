#include "stdio.h"
#include "systemcalls.h"
#include "extras.h"
#include "kasm.h"
#include "fileCalls.h"
#include "filesystem.h"
#include "string.h"
#include "libc.h"
#include "msg.h"

int who(int argc, char** argv);

int echo(int argc, char** argv);

int chColor(int argc, char** argv);

char color(char* color_name);

int time(int argc, char** argv);

int keyboard(int argc, char** argv);

int lost(int argc, char** argv);

int mastersword(int argc, char** argv);

int mario(int argc, char** argv);

int help(int argc, char** argv);

int Kill(int argc, char** argv);

int top(int argc, char** argv);

int Malloc(int argc, char** argv);

int Free(int argc, char** argv);

int format(int argc, char** argv);

int mkdir(int argc, char** argv);

int ls(int argc, char** argv);

int lsr(int argc, char** argv);

int cat(int argc, char** argv);

int touch(int argc, char** argv);

int attach(int argc, char** argv);

int ln(int argc, char** argv);

int rm(int argc, char** argv);

int rms(int argc, char** argv);

int cp(int argc, char** argv);

int mv(int argc, char** argv);

int rvl(int argc, char** argv);

int cd(int argc, char** argv);

int version(int argc, char** argv);

int revert(int argc, char** argv);

int printTableCMD(int argc, char** argv);

int printTreeCMD(int argc, char** argv);

int printBitmapCMD(int argc, char** argv);

int bigfile(int argc, char** argv);

int cwdCMD(int argc, char** argv);

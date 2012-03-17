#ifndef __MSG__
#define __MSG__

void sndMsg(int id, void* data, int size);
void rcvMsg(int id, void* data, int size);
void connect(int id);

#endif
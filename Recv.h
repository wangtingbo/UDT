#ifndef _RECV_H_
#define _RECV_H_
#include "Queue.h"
#include "Msg.h"


unsigned int get_check(const char* buf);
void* recv_syn(void* arg);
void* recv_file(void* arg);
unsigned int get_check(const char* buf);
#endif

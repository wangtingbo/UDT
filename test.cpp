#include "fileheader.h"
#include "thread.h"
#include "Msg.h"
#include "Queue.h"

#include <iostream>
using namespace std;

int handler(void* arg)
{
	Info_SYN* tmp = (Info_SYN*)arg;
	if(tmp->sockfd!=0)
	{
		printf("hello world form tid = %d,tid = %d\n",tmp->sockfd,pthread_self());
	}
}


int main(int argc, char const *argv[])
{
	struct Info_SYN syn;
	syn.sockfd = 0;
	signal(SIGALRM,signal_handler);
	ThreadPool pool;
	pool.Check_Recv();
	int i = 0;
	while(i < 3)
	{
		// gettimeofday(&syn.pack.head.Begin_Time,NULL);
		// syn.pack.head.End_Time.tv_sec = syn.pack.head.Begin_Time.tv_sec + 5;
		// syn.pack.head.End_Time.tv_usec = syn.pack.head.Begin_Time.tv_usec;
		// syn.pack.head.Run_Count = 0;
		// syn.sockfd++;
		// syn.fun = handler;	
		// pool.add_task(syn);
		// i++;
	}
	pause();
	return 0;
}



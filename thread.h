#ifndef    _THREAD_H_
#define    _THREAD_H_

#include "Queue.h"
#include "Msg.h"
#define THREAD_NUM 5
 
typedef void (*HAND)(void* arg);
 
void signal_handler(int signo);
                           				   //现在总共有的线程的数
class ThreadPool
{
public:
	ThreadPool(int _num);                     //构造函数
	ThreadPool();
	~ThreadPool(){printf("ThreadPool End\n");}			  //析构函数
	static void* Run(void* arg);			  //提供接口
	static void* Get_Task(void* arg);         //从缓冲队列中拿取数据	
	
	int add_task(Info_SYN syn);	
	void copy_node(Queue<Info_SYN>* q);
	void Send_signal();
	int Get_Usingnode_Size();
	void Check_Time(); 
	void Check_Recv(int sockfd);
	static void* cond_signal(void* arg);
	void cond_signal();
	static void* check_time(void* arg);    
	static void* check_recv(void* arg);  
	static void  change_time(int num);               
private:
	int Stop_ALL_Threads();                       //将所有的线程释放
	void create_thread(int num);                   //创建线程的函数
	int Init_Threads_Num;                         //初始化的时候创建线程的个数  
	int Usingnode_size;                           //可以使用的节点个数
};

#endif

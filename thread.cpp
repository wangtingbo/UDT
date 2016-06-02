#include "Msg.h"
#include "thread.h"
#include <pthread.h>

pthread_mutex_t MYMUTEX = PTHREAD_MUTEX_INITIALIZER;                 //互斥锁
pthread_cond_t  MYCOND  = PTHREAD_COND_INITIALIZER;                  //条件变量

pthread_mutex_t MYMUTEX_ADDTASK = PTHREAD_MUTEX_INITIALIZER;                 //互斥锁


/*
	sem[0]  来控制addtask的函数是否可以进行
	sem[1]  来控制在删除了一个节点之后,首先第一件事情应该是添加一个节点进去
*/
sem_t sem[4];  //用于管理执行的顺序


static int MYPTHREAD_NUM = 0; 		//线程的数目										 
static Queue<Info_SYN> Q;          //可以发送的队列
static Queue<Info_SYN> TMP;        //可以待发送的队列                                                
static int TASKNUM = 0;            //线程的任务数目
static int MYFLAGS = 0;
static int SOCKFD = 0;
static struct itimerval itime;                                                   

/**************************************************************
*	函数:      signal_handler(int signo)
*	函数功能:  信号处理函数,唤醒线程
*	参数:      无意义
*	返回值:    无返回值
***************************************************************/
void signal_handler(int signo)
{
	static int i = 0;
	//printf("处理 %d\n",i++);
	pthread_cond_signal(&MYCOND);
	MYFLAGS = 0;
}

void signal_handler_SIGVTALRM(int signo)
{
	struct timeval timeout;
	memset(&timeout,0,sizeof(timeout));
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(SOCKFD,&readfds);
	int ret = select(SOCKFD+1,&readfds,NULL,NULL,&timeout);
	if(ret > 0)
	{
		Info_ACK ack;
		memset(&ack,0,sizeof(ACK_PACK));
		
		int ret = recv(SOCKFD,&ack,sizeof(ack),0);
		pthread_mutex_lock(&MYMUTEX);
		printf("Recv::Pack_id = %d\n",ack.pack.Pack_id);
		Q.exchange(ack.pack.Pack_id);
		printf("Length:::%d\n",Q.get_reallysize());
		Q.showdata();
		pthread_mutex_unlock(&MYMUTEX);
	}
}

/**************************************************************
*	函数:       ThreadPool(int _num)
*	函数功能:   初始化成员变量,同时调用create_thread函数创建线程
*	参数:       _num表示要创建多少个线程
*	返回值:     无返回值
***************************************************************/
ThreadPool::ThreadPool(int _num)
{
	Usingnode_size = 0;
	Init_Threads_Num = _num;
	sem_init(&sem[0],0,1);
	sem_init(&sem[1],0,1);
	sem_init(&sem[2],0,0);
	sem_init(&sem[3],0,0);
	create_thread(_num);
}

/**************************************************************
*	函数:      ThreadPool()
*	函数功能:  初始化成员变量,同时调用create_thread函数创建线程
*	参数:      无参数
*	返回值:    无返回值
***************************************************************/
ThreadPool::ThreadPool()
{
	Usingnode_size = 0;
	Init_Threads_Num = THREAD_NUM;
	sem_init(&sem[0],0,1);
	sem_init(&sem[1],0,1);
	sem_init(&sem[2],0,0);
	sem_init(&sem[3],0,0);
	create_thread(Init_Threads_Num);
}

/**************************************************************
*	函数:      create_thread(int _num)
*	函数功能:  创建线程
*	参数:      _num表示要创建多少个线程
*	返回值:    无返回值
***************************************************************/
void ThreadPool::create_thread(int _num)
{
	int i = 0;
	for(i = 0; i < _num; i++)
	{
		pthread_t tid = 0;
		int ret = pthread_create(&tid,NULL,Run,NULL);
		while(ret == -1)
		{
			ret = pthread_create(&tid,NULL,Run,NULL);
		}
	}
}

/**************************************************************
*	函数:       check_time(void* arg)
*	函数功能:   检测哪个节点的时间已经到达,并且已经发送了5次以上,如果达到条件,便删除该节点
*	参数:       arg为标准格式,这里是表示this指针
*	返回值:     无返回值
***************************************************************/
void* ThreadPool::check_time(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	int i = 0;
	while(1)
	{
		if(Q.get_reallysize() > 0)
		{
			if(i >= Q.get_reallysize())
			{
				i = 0;
			}
			if(Q.get_reallysize() > 0)
			{
				Info_SYN tmp2;
				memset(&tmp2,0,sizeof(tmp2));
				Info_SYN tmp = Q.Get_val(i);
				if(tmp.sockfd != 0)
				{					
					struct timeval t;
					memset(&t,0,sizeof(t));
					gettimeofday(&t,NULL);
					if(tmp.pack.head.End_Time.tv_sec < t.tv_sec)//||((tmp.pack.head.End_Time.tv_sec == t.tv_sec)\
					//	 && (tmp.pack.head.End_Time.tv_usec < t.tv_usec)))
				
					{
						if(tmp.pack.head.Run_Count > 5)
						{
							pthread_mutex_lock(&MYMUTEX);

							//Q.delete_node(tmp.pack.head.Pack_id);        //删除编号
							Q.exchange(tmp.pack.head.Pack_id);
							//printf("132行   Length = %d\n",Q.get_reallysize());
							sem_wait(&sem[1]);
							//printf("133行\n");
							//Q.sub_really_len();
							
							pthread_mutex_unlock(&MYMUTEX);
							i = -1;                           //这里需要注意,是要将i++后将i变成0
							
							printf("______________________________________\n");
							pthread_mutex_lock(&MYMUTEX);
							(pool->Usingnode_size)--;
							pthread_mutex_unlock(&MYMUTEX);
						}
					}
					i++;
				}	
			}		
		}
	}
}

/**************************************************************
*	函数:      Check_Time()
*	函数功能:  创建执行check_time的线程
*	参数:      无参
*	返回值:    无返回值
***************************************************************/
void ThreadPool::Check_Time()
{
	printf("创建检查时间的线程....\n");
	pthread_t tid;
	pthread_create(&tid,NULL,check_time,(void*)this);
} 


/**************************************************************
*	函数:      Check_Recv();
*	函数功能:  创建接收的线程
*	参数:      无参
*	返回值:    无返回值
***************************************************************/
void ThreadPool::Check_Recv(int sockfd)
{
	printf("创建接收的线程......\n");
	SOCKFD = sockfd;
	signal(SIGVTALRM,signal_handler_SIGVTALRM);
	struct itimerval value;
	memset(&value,0,sizeof(value));
	value.it_interval.tv_sec  = 0;
	value.it_interval.tv_usec = 10;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 1;
    setitimer(ITIMER_VIRTUAL,&value,NULL);

	// pthread_t tid;
	// pthread_create(&tid,NULL,check_recv,(void*)&sockfd);

}

/**************************************************************
*	函数:      check_recv();
*	函数功能:  接收数据
*	参数:      无参
*	返回值:    无返回值
***************************************************************/
void* ThreadPool::check_recv(void* arg)
{
	// int sockfd = *(int*)arg;
	// fd_set readfds;
	// FD_ZERO(&readfds);
	// FD_SET(sockfd,&readfds);
	// while(1)
	// {	
	// 	FD_SET(sockfd,&readfds);
	// 	int ret = select(sockfd+1,&readfds,NULL,NULL,NULL);
	// 	if(ret > 0)
	// 	{
	// 		printf("sockfd can read..............\n");
	// 		Info_ACK ack;
	// 		memset(&ack,0,sizeof(ACK_PACK));
			
	// 		int ret = recv(sockfd,&ack,sizeof(ack),0);
	// 		pthread_mutex_lock(&MYMUTEX);
	// 		printf("Recv::Pack_id = %d\n",ack.pack.Pack_id);
	// 		Q.exchange(ack.pack.Pack_id);
	// 		printf("Length:::%d\n",Q.get_reallysize());
	// 		printf("197行:");
	// 		Q.showdata();
	// 		pthread_mutex_unlock(&MYMUTEX);
	// 		//sem_wait(&sem[1]);
	// 	}
	// }
}

/**************************************************************
*	函数:       copy_node
*	函数功能:   如果Q队列的长度达到最大的时候,就将其他的节点放到这个队列里面
*	参数:       Queue<Info_SYN>* q 一个队列的对象
*	返回值:     没有返回值
***************************************************************/
void ThreadPool::copy_node(Queue<Info_SYN>* q)
{
	TMP.Copy_All_Node(q);
	TMP.delete_all_node(q);
}

/**************************************************************
*	函数:       Get_Task
*	函数功能:   检查窗口队列中Q的实际长度,如果少于really_len的时候,自动从缓冲区取出
*	参数:       
*	返回值:     
***************************************************************/
void* ThreadPool::Get_Task(void* arg)
{
	while(1)
	{
		if(Q.get_reallysize()<Q.get_size())
		{
			if(TMP.get_reallysize() > 0)
			{
				pthread_mutex_lock(&MYMUTEX_ADDTASK);
				
				Info_SYN tmp = TMP.Get_val();
				if(tmp.sockfd != 0)
				{
					TMP.delete_node(tmp.pack.head.Pack_id);
				}
				
				gettimeofday(&tmp.pack.head.Begin_Time,NULL);
      			tmp.pack.head.End_Time.tv_sec = tmp.pack.head.Begin_Time.tv_sec + 3;
                tmp.pack.head.End_Time.tv_usec = tmp.pack.head.Begin_Time.tv_usec;
				Q.push_suitable_pos(tmp);
				cout<<"滑进去的窗口";
				Q.showdata();

				sem_post(&sem[1]);
				printf("********************************\n");
	
				pthread_mutex_unlock(&MYMUTEX_ADDTASK);
			}
			else
			{
				sem_post(&sem[1]);
			}	
		}
	}
}

/**************************************************************
*	函数:       add_task(Info_SYN syn)
*	函数功能:   将任务节点放入到队列中里面去,该函数是对外部的接口
*	参数:       syn为任务节点
*	返回值:     返回正在任务队列现可用的长度
***************************************************************/
int ThreadPool::add_task(Info_SYN syn)       
{
	if(Q.get_reallysize() < Q.get_size())
	{
		//sem_wait(&sem[0]);        //该信号量的初值为 1,这里是为了填加一次,就让显示一次
		pthread_mutex_lock(&MYMUTEX_ADDTASK);
		Q.push_queue(syn,0);
		pthread_mutex_unlock(&MYMUTEX_ADDTASK);
	}
	else
	{
		pthread_mutex_lock(&MYMUTEX_ADDTASK);
		static int flag_getTask = 0;
		TMP.push_queue(syn,1);
		pthread_mutex_unlock(&MYMUTEX_ADDTASK);
		pthread_t tid;
		if(flag_getTask == 0)
		{
			pthread_create(&tid,NULL,Get_Task,(void*)this);
			flag_getTask = 1;
		}
	}
	
	pthread_mutex_lock(&MYMUTEX);
	Usingnode_size++;	
	pthread_mutex_unlock(&MYMUTEX);
	static int flags = 0;
	if(Usingnode_size <= 1 && flags == 0)
	{
		signal(SIGALRM,signal_handler);
		flags = 1;		
		printf("创建唤醒线程\n");
		pthread_t tid;
		pthread_create(&tid,NULL,cond_signal,(void*)this);
		Check_Time();
	}
	return Q.get_reallysize();
}

/**************************************************************
*	函数:		change_time(int num)
*	函数功能:   重新计算唤醒线程的时间
*	参数:       num = 0,表示停止唤醒,num>=1表示需要唤醒,根据num的值,来设置唤醒的频率
*	返回值:     无返回值
***************************************************************/
void  ThreadPool::change_time(int num)
{
	if(num > 0)
	{
		itime.it_interval.tv_sec = 0;
		itime.it_interval.tv_usec = 1000000/num;

		//printf("修改后的时间间隔为: %d \n", itime.it_interval.tv_usec);

		itime.it_value.tv_sec  = 0;
		itime.it_value.tv_usec  = 1;
		int ret = setitimer(ITIMER_REAL,&itime,NULL);
		if(ret == 0)
		{
			//printf("重新设定唤醒时间成功\n");
		}
		else if(ret == -1)
		{	
			//printf("重新设定时间失败\n");
		}
	}
	else
	{
		itime.it_interval.tv_sec = 0;
		itime.it_interval.tv_usec = 0;
		itime.it_value.tv_sec  = 0;
		itime.it_value.tv_usec  = 0;
		setitimer(ITIMER_REAL,&itime,NULL);
	}
	
}

/**************************************************************
*	函数:		cond_signal(void* arg)
*	函数功能:   唤醒线程,根据Q队列中实际节点的个数来调节唤醒线程的间隔
*	参数:       在这里,arg的值为this指针
*	返回值:     无返回值
***************************************************************/
void* ThreadPool::cond_signal(void* arg)   
{	
	printf("in Cond_Signal\n");
	change_time(1);
	int ret = 0;
	int num = 0;
	while(1)
	{
		num = Q.get_reallysize();
		while(num == 0)
		{
			change_time(0);
		}	
		if(ret != num)
		{
			cout<<"现在的节点有:";
			Q.showdata();
			//sem_post(&sem[0]);                //这里是是上面可以继续添加节点进去
			ret = num;
			change_time(ret);
		}
		if(ret == num)     //如果相等的会怎么样呢?
		{

		}
	}
}

// void ThreadPool::cond_signal()
// {
// 	int ret = Q.get_reallysize();
// 	Q.showdata();
// 	sem_post(&sem[0]);                //这里是是上面可以继续添加节点进去
// 	change_time(ret);
// }

/**************************************************************
*	函数:      Get_Usingnode_Size()
*	函数功能:  获得现在可以使用的节点的数目
*	参数:      无参
*	返回值:    返回实际节点的数目
***************************************************************/
int ThreadPool::Get_Usingnode_Size()
{
	return Usingnode_size;
}

/**************************************************************
*	函数:      Run(void* arg)
*	函数功能:  运行线程
*	参数:      arg在这里没有使用
*	返回值:    无返回值
***************************************************************/
void* ThreadPool::Run(void* arg)
{
	static int i = 0;
	pthread_mutex_lock(&MYMUTEX);
	MYPTHREAD_NUM++;
	pthread_t tid = pthread_self();
	printf("pthread %d create\n",MYPTHREAD_NUM);
	pthread_mutex_unlock(&MYMUTEX);
	while(1)
	{
		pthread_mutex_lock(&MYMUTEX);
		pthread_cond_wait(&MYCOND,&MYMUTEX);	
		if(Q.get_reallysize()>0)
		{
			Info_SYN* tmp = Q.get_val(); 
			printf("被唤醒 %d 次,执行的节点的id = %d\n",i++,tmp->pack.head.Pack_id);		
			tmp->pack.head.Run_Count++; 
			pthread_mutex_unlock(&MYMUTEX);       
			tmp->fun(tmp);
		}
		else
		{
			
		}
	}
}



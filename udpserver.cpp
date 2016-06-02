#include "fileheader.h"
#include "Queue.h"
#include "Send.h"
#include "Msg.h"
#include "Recv.h"
#include "thread.h"
#include <sys/time.h>

/*
    服务端
*/
struct sockaddr_in seraddr;
struct sockaddr_in cliaddr;
int Ready()
{
    int udp_sockfd = socket(AF_INET,SOCK_DGRAM,0);   //获得udp的套接字
       
    bzero(&seraddr,0);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port   = htons(8989);
    seraddr.sin_addr.s_addr = inet_addr("192.168.116.3");

    bind(udp_sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr));


    bzero(&cliaddr,sizeof(cliaddr));
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port=htons(9999);
    cliaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    return udp_sockfd;
}

Queue<Info_SYN> q_send;
Queue<Info_SYN> q_wait;

static int packid = 0;  //计算编号所用

int main(int argc, char const *argv[])
{    
    int udp_sockfd = Ready();      //为连接做准备
   
    struct Info_SYN syn;
    syn.sockfd = 0;
 
    ThreadPool pool;
    pool.Check_Recv(udp_sockfd);
    int i = 0;
    int j = 0;
    while(i < 12)
    {
        gettimeofday(&syn.pack.head.Begin_Time,NULL);
        syn.pack.head.End_Time.tv_sec = syn.pack.head.Begin_Time.tv_sec + 3;
        syn.pack.head.End_Time.tv_usec = syn.pack.head.Begin_Time.tv_usec;
        syn.pack.head.Run_Count = 0;
        syn.pack.head.Pack_id = ++j;
        syn.pack.head.Check_Num = 0;        //设置校验和  

        syn.sockfd = udp_sockfd;            //设置套接字
        syn.fun = send_syn;                 //这只在run方法里面执行的函数
        syn.pack.to = cliaddr;              //设置给客户端进行发送
        sprintf(syn.pack.filename,"hello wolrd%d",i);   //设置给客户端发送的内容
        sprintf(syn.pack.buf,"hello wolrd%d",i);        //设置实际的内容 
        syn.pack.data_length = strlen(syn.pack.buf);

        pool.add_task(syn);
        i++;
    }

   // sleep(10);
    // while(i < 6)
    // {
    //     gettimeofday(&syn.pack.head.Begin_Time,NULL);
    //     syn.pack.head.End_Time.tv_sec = syn.pack.head.Begin_Time.tv_sec + 5;
    //     syn.pack.head.End_Time.tv_usec = syn.pack.head.Begin_Time.tv_usec;
    //     syn.pack.head.Run_Count = 0;
    //     syn.pack.head.Pack_id = ++j;
    //     syn.pack.head.Check_Num = 0;        //设置校验和  

    //     syn.sockfd = udp_sockfd;            //设置套接字
    //     syn.fun = send_syn;                 //这只在run方法里面执行的函数
    //     syn.pack.to = cliaddr;              //设置给客户端进行发送
    //     sprintf(syn.pack.filename,"hello wolrd%d",i);   //设置给客户端发送的内容
    //     sprintf(syn.pack.buf,"hello wolrd%d",i);        //设置实际的内容 
    //     syn.pack.data_length = strlen(syn.pack.buf);

    //     pool.add_task(syn);
       
    //     i++;
    // }

    while(1)
    {

    }
    return 0;
}



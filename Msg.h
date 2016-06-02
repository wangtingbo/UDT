#ifndef    _MSG_H_
#define   _MSG_H_
#include "fileheader.h"
#include "Queue.h"

typedef void* (*Fun)(void* arg);

typedef struct _UDT_HEAD_
{
	struct timeval Begin_Time;   //开始时间
    struct timeval End_Time;     //结束时间
    int  Run_Count;          //发送的次数
	int  Pack_id;            //包的序列号
	unsigned int Check_Num;  //校验和 
}UDT_HEAD;


typedef struct _ACK_PACK_
{
	int window_size;        //窗口的大小
	int length;             //接收到的长度
	int Pack_id;            //回发的序列号		
}ACK_PACK;

typedef struct _UDT_SEND_PACK
{
	struct sockaddr_in to;  //给哪个地址发
	char filename[20];      //文件名称 
	UDT_HEAD  head;         //头部信息 
	int data_length;        //数据的长度,默认数据时为0,当data_length不为0的时候,就应该是发送的是文件的总长度
	char buf[100];          //真实的数据
}UDT_SEND_PACK;

typedef struct Info_SYN
{
	Fun fun;
	int sockfd;
	UDT_SEND_PACK pack;	
}Info_SYN;

typedef struct Info_ACK
{
	int sockfd;
	ACK_PACK pack;
}Info_ACK;

#endif

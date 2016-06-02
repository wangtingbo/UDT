#include "Recv.h"
#include "Msg.h"
#include "Send.h"
#include "fileheader.h"

int FLAGS = 0;

/**********************************************************
*函数:get_check
*功能:可以获得每个发送数据的校验和
*返回值:返回校验和
**********************************************************/
unsigned int get_check(const char* buf)
{
	int len = strlen(buf);
	int i = 0;
	unsigned int check = 0;
	for(i = 0;i < len;i++)
	{
		check += buf[i];
	}
	return check;
}

void* recv_syn(void* arg)
{
	int sockfd = *(int*)arg;

	UDT_SEND_PACK *pack = (UDT_SEND_PACK*)malloc(1*sizeof(UDT_SEND_PACK));

	memset(&pack,0,sizeof(pack));

	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	socklen_t len = sizeof(addr);
	int ret = recvfrom(sockfd,&pack,sizeof(pack),0,(struct sockaddr*)&addr,&len);

	unsigned int check = get_check(pack->buf);
	
	if(check == pack->head.Check_Num)
	{	
		if(FLAGS == 0)
		{
			connect(sockfd,(struct sockaddr*)&addr,len);
			FLAGS = 1;
		}	
		return pack;
	}
	else
	{
		return NULL;
	}	
}

void* recv_file(void* arg)   //这里的接收与上面的接收完全不同
{
	int sockfd = *(int*)arg;

	UDT_SEND_PACK *pack = (UDT_SEND_PACK*)malloc(1*sizeof(UDT_SEND_PACK));
	memset(&pack,0,sizeof(pack));

	int ret = recvfrom(sockfd,&pack,sizeof(pack),0,NULL,NULL);
	unsigned int check = get_check(pack->buf);
	
	if(check == pack->head.Check_Num)
	{		
		return pack;
	}
	else
	{
		return NULL;
	}	
}

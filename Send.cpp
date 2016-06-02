#include "fileheader.h"
#include "Msg.h"

void* send_syn(void*  arg)
{
	Info_SYN *info = (Info_SYN*)arg;
	
	int ret = sendto(info->sockfd,&(info->pack),sizeof(info->pack),0,(struct sockaddr*)&(info->pack.to),sizeof(info->pack.to));
	
	if(ret == -1)
	{
		perror("sendto:");
		exit(-1);
	}
	printf("send suceesss\n");
}

void* send_file(void* arg)
{
	Info_SYN *info = (Info_SYN*)arg;
	
	int ret = sendto(info->sockfd,&(info->pack),sizeof(info->pack),0,(struct sockaddr*)&(info->pack.to),sizeof(info->pack.to));
	if(ret == -1)
	{
		perror("sendto:");
		exit(-1);
	}
}

void* send_ack(void*  arg)
{
	Info_ACK *info = (Info_ACK*)arg;
	int ret = send(info->sockfd,&(info->pack),sizeof(info->pack),0);
	if(ret == -1)
	{
		perror("sendto:");
		exit(-1);
	}
}

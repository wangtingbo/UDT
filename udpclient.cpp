#include "fileheader.h"
#include "Msg.h"

jmp_buf env[2];
void handler1(int signo)
{
	printf("in handler1\n");
	siglongjmp(env[0],1);
}

void* recv_ack(void* fd)
{
	static int i = 0;
	while(1)
	{
		struct sockaddr_in cliaddr;
		memset(&cliaddr,0,sizeof(cliaddr));

		int addrlen=sizeof(cliaddr);

		UDT_SEND_PACK syn;
		memset(&syn,0,sizeof(syn));
		int ret = recvfrom(*(int*)fd,&syn,sizeof(Info_SYN),0,(struct sockaddr*)&cliaddr,(socklen_t*)&addrlen);//
		if(ret > 0)
		{
			printf("recv:Msg is %s,Pack_id = %d\n",syn.buf,syn.head.Pack_id);
		}
		Info_ACK ack;
		memset(&ack,0,sizeof(Info_ACK));
		ack.pack.Pack_id =  syn.head.Pack_id; //+ 1;
		if(ack.pack.Pack_id != 3 && (ack.pack.Pack_id != 2))
			sendto(*(int*)fd,&ack,sizeof(ack),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
	}
}


int main(int argc, char const *argv[])
{
	signal(SIGALRM,handler1);
	int socket1=socket(AF_INET,SOCK_DGRAM,0);
    //监听套接字
    if(socket1==-1)
    {
    	perror("socket:");
    	exit(-1);
    }
     //设置自身地址结构
    struct sockaddr_in clientaddr;
    bzero(&clientaddr,sizeof(clientaddr));
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_port=htons(9999);
    clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(socket1,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
    
    struct sockaddr_in seraddr;
    bzero(&seraddr,sizeof(seraddr));
  
    pthread_t tid[2];
    //pthread_create(&tid[0],NULL,send_syn,NULL);
    pthread_create(&tid[1],NULL,recv_ack,(void*)&socket1);

    //pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);

	return 0;
}

#include "fileheader.h"

typedef struct fileblock
{
	unsigned char* srcfileaddres;
	unsigned char* destfileaddres;
	unsigned int startfilepos;
	unsigned int blocksize;
}fileblock;

/*********************************************************************
*功能:计算文件大小
*参数:unsigned int fd  源文件描述符
*返回值: 文件大小
**********************************************************************/
unsigned int  get_file_size(unsigned int fd)
{
	struct stat sa;
	fstat(fd,&sa);
	unsigned int filesize = sa.st_size;
	return filesize;
}

/********************************************************************
*函数功能:依据文件大小计算文件分块数
*参数:unsigned int fd  源文件描述符
*返回值: 文件块数
*********************************************************************/
unsigned int  get_file_block_cnt(unsigned int fd)
{
	unsigned int fileblockcnt=0;
	unsigned int  filesize=get_file_size(fd);	           //获得文件大小
	unsigned int fileremaindsize=filesize%BLOCKSIZE;	   //判断该文件是不是BLOCKSIZE的整数倍
	fileblockcnt=filesize/BLOCKSIZE;				
	if(fileremaindsize>0)
	{
       fileblockcnt=fileblockcnt+1;
	}
	return fileblockcnt;
}

/******************************************************************
*函数功能:如果文件分块不是整数块,计算出剩余的字节数
*参数:unsigned int fd  源文件描述符
*返回值: 最后一块大小,文件最后一块剩余大小
******************************************************************/
unsigned int   get_remainsize(unsigned int fd)
{
	unsigned int remainsize=0;
	unsigned int filesize=get_file_size(fd);
	if(filesize%BLOCKSIZE>0)
	{
		remainsize=filesize%BLOCKSIZE;
	}
	return remainsize;
}

/*****************************************************************
函数功能:建立文件内存映射
参数:unsigned int fd  源文件描述符
返回值:返回内存映射地址值
******************************************************************/
unsigned char *  get_srcfile_map_addres(unsigned int fd)
{
	unsigned int filesize=get_file_size(fd);//获得文件大小
	//unsigned char * filestartp=NULL;
	g_srcfilestartp=(unsigned char*)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(g_srcfilestartp==NULL)
	{
		perror("mmap:");
		exit(-1);
	}
	return g_srcfilestartp;      //得到文件映射的首地址
} 


/*
函数功能:释放源文件内存映射区
参数:int fd 映射对应的文件描述符
*/
void  set_srcfile_munmap(int fd)
{
	unsigned int filesize=get_file_size(fd);
    if(g_srcfilestartp!=NULL)
    {
        munmap(g_srcfilestartp,filesize);
    }
}


/****************************************************************************
*函数:计算文件分块,并记录每块文件的信息
*参数:unsigned int fd  源文件描述符
*返回值:指向记录文件块信息结构数组指针
*****************************************************************************/

fileblock*  get_file_block(unsigned int fd)
{
   unsigned int fileblockcnt=get_file_block_cnt(fd);   //得到分块数
   unsigned int fileremaindsize=get_remainsize(fd);	   //得到最后一块,余下的大小
   fileblock* fileblockarray=(fileblock*)malloc(fileblockcnt*sizeof(fileblock));//文件块结构信息
   if(fileblockarray==NULL)
   {
   	   output_sys_errmsg("get_file_block malloc:");
   	   exit(-1);
   }
   int i;
   for(i=0;i<fileblockcnt-1;i++)
   {
      fileblockarray[i].startfilepos=i*BLOCKSIZE;
      fileblockarray[i].blocksize=BLOCKSIZE;
   }

   fileblockarray[i].startfilepos=i*BLOCKSIZE;
   fileblockarray[i].blocksize=fileremaindsize>0?fileremaindsize:BLOCKSIZE;
   return fileblockarray;
}

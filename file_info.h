/*
	主要是获得一些文件的参数,以及可以按大小分成的块数
*/
#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#define BLOCKSIZE 1024

unsigned int  get_file_size(unsigned int fd);
unsigned int  get_file_block_cnt(unsigned int fd);
unsigned int   get_remainsize(unsigned int fd);
unsigned char *  get_srcfile_map_addres(unsigned int fd);
void  set_srcfile_munmap(int fd);
fileblock*  get_file_block(unsigned int fd);

#endif

#include "Time.h"
#include "fileheader.h"

int get_time(struct timeval* src)
{
	int ret = gettimeofday(src,NULL);
	return ret;
}


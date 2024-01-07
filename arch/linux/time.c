#include "../time.h"
#include <sys/time.h> 
#include <stddef.h>
// #include <time.h>

double get_time(){
	struct timeval start;
	gettimeofday(&start, NULL);
	//printf("%d %d\n", start.tv_sec, start.tv_usec);
	
	//TODO probably don't need this precision
	double us = (start.tv_sec*1e6 + start.tv_usec);
	return us * 1e-6;

//	return clock() / CLOCKS_PER_SEC
}





//TODO include guard naming scheme
//#ifndef 

#include<stdlib.h>
#include<string.h>

//#include"linalg.h"

//TODO put in .c file

#define SIZE(b)(((b).w+1)*(b).h*sizeof(char))

typedef struct {
	char *buf;
	//note width is +1
	size_t w, h;
} fbuf;

fbuf init_fbuf(size_t w, size_t h){
	char *ret = malloc(sizeof(char)*(w+1)*h + 1);
	memset(ret, '.', sizeof(char)*(w+1)*h);
	for(size_t i = 0; i < h; i++)
		ret[w + i*(w+1)] = '\n';
	ret[(w+1)*h] = 0;
	return (fbuf){ret, w, h};
}

void print_buf(fbuf b){
	fflush(stdout);
	fwrite(b.buf, sizeof(char), (b.w+1)*b.h, stdout);
	fflush(stdout);
}


void copy_buf(fbuf a, fbuf b){
	if(a.w == b.w && a.h == b.h){
		memcpy(b.buf, a.buf, SIZE(b));
	}else{
		//TODO case where they don't match :/
	}
}


/*
void draw_line(fbuf a, vec2 p1, vec2 p2){
	//TODO Bresenham's line algorithm
}
*/


//TODO probably use vectors
void txt_shader(fbuf b, void *ctx, char (*fn)(size_t,size_t,void*)){
	for(size_t i = 0; i < b.h; i++)
		for(size_t j = 0; j < b.w; j++)
			b.buf[j + i*(b.w+1)] = fn(j, i, ctx);
}



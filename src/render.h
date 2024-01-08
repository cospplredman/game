
//TODO include guard naming scheme
//#ifndef 

#include<stdlib.h>
#include<string.h>

#include"linalg.h"

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

void draw_point(fbuf a, vec2 p1, char c){
	a.buf[(int)p1.x + (int)p1.y*(a.w+1)] = c;
}

/*
void draw_line(fbuf a, vec2 p1, vec2 p2, float th){
	draw_point(a, p1, '#');
	draw_point(a, p2, '#');

	vec2 k = sub2(p2, p1);
	while(k.x > th || k.x < -th || k.y > th || k.y < -th){
		k = add2(mul2(k, 0.5), p1);
		draw_line(a, p1, k, th);
		p1 = k;
		k = sub2(p2, p1);
	}
}
*/

void draw_line_(fbuf a, vec2 p1, vec2 p2, float th){
	draw_point(a, p2, '#');

	vec2 k = sub2(p2, p1);
	
	float l = len2(k)/th;

	vec2 os = k;
	for(int i = 0; i < (l-1); i++){
		vec2 cp = add2(p1, mul2(os, 1.0/l));
		draw_point(a, cp, '*');
		os = add2(os, k);
	}
}

void draw_line(fbuf a, vec2 p1, vec2 p2, float th){
	draw_line_(a, p2, p1, th*0.5);
	draw_line_(a, p1, p2, th*0.5);
}


//TODO probably use vectors
void txt_shader(fbuf b, void *ctx, char (*fn)(size_t,size_t,void*)){
	for(size_t i = 0; i < b.h; i++)
		for(size_t j = 0; j < b.w; j++)
			b.buf[j + i*(b.w+1)] = fn(j, i, ctx);
}



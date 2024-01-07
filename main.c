#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>
#include<math.h>
#include<stdbool.h>
#include <sys/time.h> 


typedef struct {
	float x, y, z;
} vec3;

typedef struct {
	float dat[4][4];
} mat4;

vec3 add(vec3 a, vec3 b){
	return (vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }

vec3 sub(vec3 a, vec3 b){
	return (vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }

vec3 mul(vec3 a, vec3 b){
	return (vec3){a.x * b.x, a.y * b.y, a.z * b.z}; }

float dot(vec3 a, vec3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z; }

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

void free_fbuf(fbuf buf){
	free(buf.buf);
}

#define HIDE_CURSOR printf("\033[?25l")
#define SHOW_CURSOR printf("\033[?25h")
#define SET_CURSOR_POS(row, col) printf("\033[%d;%dH", (row), (col))
void clear_screen(){
	SET_CURSOR_POS(0, 0);
	//system("clear");
}

void print_buf(fbuf b){
	fflush(stdout);

	fwrite(b.buf, sizeof(char), (b.w+1)*b.h, stdout);
	fflush(stdout);
}


#define min(a, b) ((a)<(b)?(a):(b))

int main(){
	setbuf(stdout, malloc(min(BUFSIZ, 200 * 70)));
	HIDE_CURSOR;

	fbuf buf = init_fbuf(160, 50);	

	struct timeval stop, start;
	gettimeofday(&start, NULL);
	
	double total_time = 0.0, prev_time = 0.0;

	while(1){
		buf.buf[(int)(total_time / 5)] = '#';

		double dt = total_time - prev_time;
		clear_screen();
		print_buf(buf);
		printf("time: %lf dt: %lf fps: %lf\n", total_time, dt, 1.0/dt);
		SET_CURSOR_POS(0, 0);

		gettimeofday(&stop, NULL);
		double t = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
		prev_time = total_time;

		total_time = t / 1000000;




	}

	printf("%d\n", BUFSIZ);
	

	return 0;
}


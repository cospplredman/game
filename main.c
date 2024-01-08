#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>
#include<math.h>
#include<stdbool.h>
#include<stdint.h>

#include "arch/tty.h"
#include "arch/time.h"
#include "arch/input.h"

#include "src/linalg.h"
#include "src/render.h"

#define min(a, b) ((a)<(b)?(a):(b))

struct shader_context{
	double time;
	size_t w, h;
};


char shader(size_t x, size_t y, void *ctx_){
	struct shader_context *ctx = (struct shader_context*)ctx_;

	double uvx = (double)(2*(int)x - (int)ctx->w)/(double)ctx->w * 2.1,
	       uvy = (double)(2*(int)y - (int)ctx->h)/(double)ctx->h;

	double dist = sqrt(uvx*uvx + uvy*uvy);
	if(dist < (0.5*cos(0.1*ctx->time) + 1.1))
		return (char)(13 * sin(dist*dist + ctx->time) + 'A' + 13);


	return ' ';
}


int main(){


	//make sure our frame buffer can fit into the output buffer
	setbuf(stdout, malloc(min(BUFSIZ, get_width()*get_height()*3)));
	setup_input();

	clear();
	hide_cursor();

	//no need to cleanup because that is handled by c at program exit
	fbuf buf = init_fbuf(get_width(), get_height());	
	double start = get_time(), frame_start = start, frame_end;
	while(1){

		frame_end = get_time();
		double dt = frame_end - frame_start;
		frame_start = frame_end;

		reset_position();

		struct shader_context ctx = {
			.time = frame_start,
			.w = buf.w,
			.h = buf.h
		};

		txt_shader(buf, &ctx, shader);

		draw_line(buf, (vec2){5, 5 + 4*cos(frame_start)}, (vec2){buf.w - 5, buf.h - 5}, 2 + cos(frame_start));

		print_buf(buf);
		printf("time: %lf dt: %lf fps: %lf\n", frame_start - start, dt, 1.0/dt);

		if(have_input()){
			printf("%c\n", get_key());
		}
	}

	return 0;
}


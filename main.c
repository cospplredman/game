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
#include "arch/networking.h"

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

int x = 0, y = 0;
int px = 0, py = 0;

void packet_callback(int p, char *buf, size_t len){
	printf("%d: %s", p, buf);
	for(size_t i = 0; i < len; i++){
		printf("%d: %c\n", p, buf[i]);
		switch(buf[i]){
			case 'a':
				px--;
				break;
			case 's':
				py++;
				break;
			case 'd':
				px++;
				break;
			case 'w':
				py--;
				break;
		}
	}
}

void handle_input(){
	char key = get_key();
	char buf[100];
	int port;
	
	switch(key){
		case 'a':
			x--;
			break;
		case 's':
			y++;
			break;
		case 'd':
			x++;
			break;
		case 'w':
			y--;
			break;
		case 'c':
			fflush(stdout);
			clear();
			show_cursor();

			printf("\n\nconnect to: ");
			fflush(stdout);

			scanf("%s", buf); 
			printf("\nport: ");
			fflush(stdout);

			scanf("%d", &port);
			connect_to_peer(buf, port);
			hide_cursor();
			break;
	}

	send_to_peer(0, &key, 1);
}

int main(int c, char **argv){
	if(c != 2){
		printf("usage: game PORT\n");
		return 1;
	}

	int port = atoi(argv[1]);
	setup_peer_con(port, packet_callback);

	//make sure our frame buffer can fit into the output buffer
	setbuf(stdout, malloc(min(BUFSIZ, get_width()*get_height()*3)));

	setup_input();
	clear();
	hide_cursor();

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
		draw_point(buf, (vec2){x, y}, '%');
		draw_point(buf, (vec2){px, py}, '&');

		print_buf(buf);
		printf("time: %lf dt: %lf fps: %lf\n", frame_start - start, dt, 1.0/dt);
		printf("\f\f");

		while(have_input())
			handle_input();
	}
	return 0;
}


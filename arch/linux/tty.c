#include "../tty.h"
#include <stdio.h>

//ansi specific
#define HIDE_CURSOR printf("\033[?25l")
#define SHOW_CURSOR printf("\033[?25h")
#define SET_CURSOR_POS(row, col) printf("\033[%d;%dH", (row), (col))

#define TXT_ASPECT 2.1  //h/w

void hide_cursor(){
	HIDE_CURSOR;
}

void show_cursor(){
	SHOW_CURSOR;
}

void reset_position(){
	SET_CURSOR_POS(0, 0);
	//system("clear");
}

//TODO for realzies
size_t get_width(){
	return 160;
}

size_t get_height(){
	return 50;
}


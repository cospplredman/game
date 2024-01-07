
#ifndef GAME_TTY_H
#define GAME_TTY_H
#include<stddef.h>

void hide_cursor();
void show_cursor();
void reset_position();
size_t get_width();
size_t get_height();

#endif


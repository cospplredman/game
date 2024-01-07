




//ansi specific
#define HIDE_CURSOR printf("\033[?25l")
#define SHOW_CURSOR printf("\033[?25h")
#define SET_CURSOR_POS(row, col) printf("\033[%d;%dH", (row), (col))

#define TXT_ASPECT 2.1  //h/w

static inline void hide_cursor(){
	HIDE_CURSOR;
}

static inline void show_cursor(){
	SHOW_CURSOR;
}

static inline void reset_position(){
	SET_CURSOR_POS(0, 0);
	//system("clear");
}


//TODO for realzies
static inline size_t get_width(){
	return 160;
}

static inline size_t get_height(){
	return 50;
}


CC ?= gcc
CFLAGS ?= -std=c99 -g -O2 -Wall -Wextra -pedantic -lm

all: game
clean:
	rm -f ./game

game: main.c
	${CC} $< -o $@ ${CFLAGS}

.PHONY: all clean


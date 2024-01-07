CC ?= gcc
CFLAGS ?= -std=c99 -g -O2 -Wall -Wextra -pedantic -lm

platform=linux

all: game
clean:
	rm -f ./game

game: main.c ./arch/${platform}/*.c
	${CC} $^ -o $@ ${CFLAGS}

.PHONY: all clean


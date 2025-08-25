
CC=gcc
SOURCES=cosu.c\
        parser.c\
        game.c
OUT=cosu
CFLAGS=-Wall -Werror -Wpedantic -ggdb -lSDL3

build:
	$(CC) $(SOURCES) -o $(OUT) $(CFLAGS)

all: build

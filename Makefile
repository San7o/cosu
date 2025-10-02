# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# License: MIT

#
# Compiler files
#
CFLAGS      = -Wall -Werror -Wpedantic -std=c99
DEBUG_FLAGS = -ggdb
LDFLAGS     = -lXrandr -lX11 -lm
CC          = gcc

#
# Project files
#
OUT_NAME = cosu
OBJ      = cosu.o\
           parser.o\
           game.o\
           rgfw_impl.o\
           miniaudio_impl.o\
           note.o

#
# Commands
#
all: $(OUT_NAME)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(OUT_NAME)

run: $(OUT_NAME)
	chmod +x $(OUT_NAME)
	./$(OUT_NAME)

clean:
	rm -f $(OBJ)

distclean:
	rm -f $(OUT_NAME)

$(OUT_NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(CLAGS) -o $(OUT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

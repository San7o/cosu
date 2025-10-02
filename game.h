// SPDX-License-Identifier: MIT

#ifndef _COSU_GAME_H_
#define _COSU_GAME_H_

// Defaults
#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 800
#define SCROLL_SPEED 1
#define FPS 60
#define NOTE_FREQUENCY 8
#define RANDOM_MODE true

#include "RGFW.h"
#include "miniaudio.h"
#include "note.h"

#include <stdbool.h>

typedef struct {
  int window_width;
  int window_height;
  char* window_name;
  RGFW_window* win;
  unsigned char *screen;
  RGFW_surface *surface;
  CosuNoteList map;
  CosuNoteList notes;
  double note_frequency;
  double note_time;
  double scroll_speed;
  double delta_time;
  double fps;
  ma_engine audio_engine;
  bool random_mode;
} Game;

// Render a frame
void game_frame(void *game);

#endif // _COSU_GAME_H_

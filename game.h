// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT

#ifndef _COSU_GAME_H_
#define _COSU_GAME_H_

//
// Default settings
//

// Screen size
#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 800
// Vertical speed of the notes
#define SCROLL_SPEED 1
// Number of frames that should be rendered per second
#define FPS 60
// With random mode enabled, the game will randomly generate notes
// endlessly. With this feature disabled, the game will read a map and
// play it (map mode)
#define RANDOM_MODE false
// Notes per second to be generated if random mode is set to true
#define NOTE_FREQUENCY 8
// Audio file of the hitsound to play when a note is hit
#define HITSOUND "sound/hitsound.mp3"
// Default map
#define MAP_FILE "maps/test.osu"

#include "RGFW.h"
#include "miniaudio.h"
#include "note.h"

#include <stdbool.h>

// The cosu! game
typedef struct {
  // Window data
  int window_width;
  int window_height;
  char* window_name;
  RGFW_window* win;
  // Array of pixels of size
  // screen_width*screen_height*micro_draw_get_channels(MICRO_DRAW_RGBA8)
  unsigned char *screen;
  RGFW_surface *surface;

  // Wheter or not random mode is enabled
  bool random_mode;
  // Show the help message and exit
  bool show_help;

  // The file of the map to play if random mode is set to false
  char *map_file;
  // Notes to play if random mode is set to false
  CosuNoteList map;
  // List of notes during gameplay
  CosuNoteList notes;
  // Miniaudio engine
  ma_engine audio_engine;
  
  // See NOTE_FREQUENCY
  double note_frequency;
  // See SCROLL_SPEED
  double scroll_speed;
  // Time since last frame rendered
  double delta_time;
  // Time since last generated note for random mode, or time since the
  // start for map mode
  double note_time;
  // See FPS
  int fps;
} Game;

// Render a frame
void game_frame(void *game);

#endif // _COSU_GAME_H_

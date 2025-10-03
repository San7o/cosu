// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT

#define _GNU_SOURCE  // getline
#define MICRO_DRAW_IMPLEMENTATION
#include "micro-draw.h"
#define MICRO_LOG_IMPLEMENTATION
#include "micro-log.h"
#define MICRO_FLAG_IMPLEMENTATION
#include "micro-flag.h"
#include "miniaudio.h"
#include "RGFW.h"
#include "game.h"
#include "parser.h"

// Optional compilation using emscripten
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int main(int argc, char** argv)
{  
  Game game = (Game) {
    .window_width   = SCREEN_WIDTH,
    .window_height  = SCREEN_HEIGHT,
    .window_name    = "cosu!",
    .scroll_speed   = SCROLL_SPEED,
    .fps            = FPS,
    .note_frequency = NOTE_FREQUENCY,
    .notes          = {NULL, NULL},
    .note_time      = 0.0,
    .random_mode    = RANDOM_MODE,
    .delta_time     = 0,
    .map            = {NULL, NULL},
    .map_file       = MAP_FILE,
    .show_help      = false,
  };

  MicroFlag flags[] =
  {
     { MICRO_FLAG_BOOL, &game.show_help, "-h", "--help", "show help message" },
     { MICRO_FLAG_BOOL, &game.random_mode, "-r", "--random", "generate notes randomly" },
     { MICRO_FLAG_DOUBLE, &game.note_frequency, "-f", "--frequency", "set random note frequency" },
     { MICRO_FLAG_INT, &game.window_width, "-x", "--width", "set screen width" },
     { MICRO_FLAG_INT, &game.window_height, "-y", "--height", "set screen height" },
     { MICRO_FLAG_INT, &game.fps, "-p", "--fps", "set flames per second" },
     { MICRO_FLAG_DOUBLE, &game.scroll_speed, "-s", "--speed", "set the scroll speed" },
     { MICRO_FLAG_STR, &game.map_file, "-m", "--map", "use map file" },
  };

  size_t num_flags = sizeof(flags) / sizeof(flags[0]);  
  if (micro_flag_parse(flags, num_flags, argc, argv) != MICRO_FLAG_OK)
    return 1;

  if (game.show_help)
  {
    micro_flag_print_help("cosu",
                          "A rhythm game written in C, (not yet fully) compatible with osu!mania maps.",
                          flags,
                          num_flags);
    return 0;
  }
  
  // Logging
  micro_log_init();
  micro_log_set_flags(MICRO_LOG_FLAG_LEVEL
                      | MICRO_LOG_FLAG_DATE
                      | MICRO_LOG_FLAG_TIME);
  
  if (!game.random_mode)
  {
    // Read Map file
    int ret = cosu_note_list_parse_file(&game.map, game.map_file);
    if (ret < 0)
    {
      micro_log_error("Error %d parsing file\n", ret);
      micro_log_close();
      return 1;
    }
  }

  // Window
  game.win = RGFW_createWindow("cosu",
                               0, 0,
                               game.window_width, game.window_height,
                               RGFW_windowCenter
                               | RGFW_windowNoResize);
  game.screen = malloc(game.window_width*game.window_height
                       *micro_draw_get_channels(MICRO_DRAW_RGBA8));
  game.surface =
    RGFW_window_createSurface(game.win, (u8*)game.screen,
                              game.window_width, game.window_height,
                              RGFW_formatRGBA8);
  RGFW_window_setExitKey(game.win, RGFW_escape);
  micro_log_info("Initialized window");

  // Audio
  ma_result result;
  result = ma_engine_init(NULL, &game.audio_engine);
  if (result != MA_SUCCESS) {
    micro_log_error("Error %d initializing miniaudio engine", result);
    goto exit;
  }

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(game_frame, &game, game.fps, 1);
  #else
  while (RGFW_window_shouldClose(game.win) == RGFW_FALSE) game_frame(&game);
  #endif // __EMSCRIPTEN__

 exit:
  micro_log_info("Cleaning up resources");
  ma_engine_uninit(&game.audio_engine);
  cosu_note_list_free(&game.notes);
  if (!game.random_mode)
    cosu_note_list_free(&game.map);
  RGFW_surface_free(game.surface);
  free(game.screen);
  RGFW_window_close(game.win);
  micro_log_close();
  return 0;
}

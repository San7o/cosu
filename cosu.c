// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#define MICRO_DRAW_IMPLEMENTATION
#include "micro-draw.h"
#define MICRO_LOG_IMPLEMENTATION
#include "micro-log.h"
#include "miniaudio.h"
#include "RGFW.h"
#include "game.h"
#include "parser.h"

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
  };

  if (!game.random_mode)
  {
    int ret = cosu_note_list_parse_file(&game.map, "maps/test.osu");
    if (ret < 0)
    {
      fprintf(stderr, "Error %d parsing file\n", ret);
      return -1;
    }
  }
  
  micro_log_init();
  micro_log_set_flags(MICRO_LOG_FLAG_LEVEL
                      | MICRO_LOG_FLAG_DATE
                      | MICRO_LOG_FLAG_TIME);
    
  game.win = RGFW_createWindow("cosu",
                               0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                               RGFW_windowCenter
                               | RGFW_windowNoResize);
  micro_log_info("Initialized window");

  ma_result result;
  result = ma_engine_init(NULL, &game.audio_engine);
  if (result != MA_SUCCESS) {
    return result;
  }
  
  // Note: X11 uses RGBA
  game.screen = malloc(SCREEN_WIDTH*SCREEN_HEIGHT
                       *micro_draw_get_channels(MICRO_DRAW_RGBA8));

  game.surface =
    RGFW_window_createSurface(game.win, (u8*)game.screen,
                              SCREEN_WIDTH, SCREEN_HEIGHT, RGFW_formatRGBA8);

  RGFW_window_setExitKey(game.win, RGFW_escape);

  game.delta_time = 0;
  game.note_time = 0;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(game_frame, &game, game.fps, 1);
  #else
  while (RGFW_window_shouldClose(game.win) == RGFW_FALSE) game_frame(&game);
  #endif // __EMSCRIPTEN__

  micro_log_info("Cleaning up resources");
  ma_engine_uninit(&game.audio_engine);
  cosu_note_list_free(&game.notes);
  RGFW_surface_free(game.surface);
  free(game.screen);
  RGFW_window_close(game.win);
  micro_log_close();
  return 0;
}

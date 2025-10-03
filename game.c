// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT

#include "game.h"
#include "parser.h"
#include "note.h"
#include "error.h"
#include "micro-draw.h"
#include "micro-log.h"
#include "miniaudio.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// LCG pseudo random number generator
#define MAGIC1 16843009  // a
#define MAGIC2 826366247 // c
unsigned int lcg_seed = 69696969;
unsigned int lcg(const unsigned int seed)
{
  return (MAGIC1 * seed + MAGIC2);
}

// In order:
// - read events, check if exit key was pressed or user hit a note
//   - if a note was hit, remove it from the current notes
// - check if a new note should be added to the current notes
//   - if random mode is enabled, generate a new note
//   - otherwise, add the last map note to the current notes
// - when a frame whould be rendered, iterate over the notes and draw them
//   - if a note is outside the screen, remove it
// - update time data
void game_frame(void *arg)
{
  Game *game = (Game*) arg;
  unsigned char color_white[4] = {255, 255, 255, 255};
  unsigned char color_black[4] = {0, 0, 0, 255};
  unsigned char color_red[4] = {255, 0, 0, 255};
  
  RGFW_event event;
  clock_t frame_start = clock();

  while (RGFW_window_checkEvent(game->win, &event))
  {
    if (event.type == RGFW_quit)
      break;

    if (event.type == RGFW_keyPressed)
    {
      CosuNote* note = cosu_note_list_tail(&game->notes);
      if (!note) continue;

      bool hit = false;
      switch(event.key.sym)
      {
      case 'd':
        if (note->x == 0 && note->y > 3 * (game->window_height / 4))
          hit = true;
        break;
      case 'f':
        if (note->x == 1 && note->y > 3 * (game->window_height / 4))
          hit = true;
        break;
      case 'j':
        if (note->x == 2 && note->y > 3 * (game->window_height / 4))
          hit = true;
        break;
      case 'k':
        if (note->x == 3 && note->y > 3 * (game->window_height / 4))
          hit = true;
        break;
      default:
        break;
      }

      if (hit)
      {
        cosu_note_list_pop_back(&game->notes);
        ma_engine_play_sound(&game->audio_engine, HITSOUND, NULL);
      }
    }
  }

  if (game->random_mode) // Generate notes randomly
  { 
    if (game->note_time > 1 / (double) game->note_frequency)
    {    
      game->note_time = 0;

      lcg_seed = lcg(lcg_seed);
      if (lcg_seed % 5 != 4) // module 5 to allow empty space, also
                             // lcg does not work modulo 4
      {
        CosuNote note = (CosuNote) {
          .type = COSU_TYPE_NOTE,
          .x = (lcg_seed % 5),
          .y = 0
        };
        cosu_note_list_push_front(&game->notes, note);
        //micro_log_trace("Created note at position x: %u, y: %u", note.x, note.y);
      }
    }
  }
  else
  {
    CosuNoteListElem *it = game->map.head;
    while (it && it->note.time / (double) 1000 < game->note_time)
    {
      CosuNoteListElem *next = it->next;
      cosu_note_list_push_front(&game->notes, *cosu_note_list_head(&game->map));
      cosu_note_list_pop_front(&game->map);
      it = next;
    }
  }
  
  #ifndef __EMSCRIPTEN__
  if (game->delta_time > 1 / game->fps) // Should draw
  {
  #endif
    game->delta_time = 0;
    
    micro_draw_clear(game->screen, game->window_width, game->window_height,
                     color_black, MICRO_DRAW_RGBA8); 

    CosuNoteListElem *it = game->notes.tail;
    while(it)
    {
      if (it->note.y > game->window_height) // Note is out of screen
      {
        it = it->prev;
        cosu_note_list_pop_back(&game->notes);
        micro_log_trace("removed note");
        continue;
      }

      // Hit line
      micro_draw_line(game->screen,
                      game->window_width, game->window_height,
                      0, 3 * (game->window_height / 4) + game->window_height / 10,
                      game->window_width, 3 * (game->window_height / 4) + game->window_height / 10,
                      color_red, MICRO_DRAW_RGBA8);

      // Note
      micro_draw_fill_rect(game->screen,
                           game->window_width, game->window_height,
                           it->note.x * game->window_width / 4, it->note.y,
                           game->window_width / 4, game->window_height / 10,
                           color_white, MICRO_DRAW_RGBA8);

      // Update note position
      it->note.y += 1.0 / game->fps * game->window_height * game->scroll_speed;
      it = it->prev;
    }

    RGFW_window_blitSurface(game->win, game->surface);
    
  #ifndef __EMSCRIPTEN__
  }
  #endif

  clock_t frame_end = clock();
  double diff = (double)(frame_end - frame_start) / CLOCKS_PER_SEC;
  game->delta_time += diff;
  game->note_time += diff;
  return;
}

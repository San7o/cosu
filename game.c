/*
 * MIT License
 *
 * Copyright (c) 2025 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "game.h"
#include "error.h"

#include <stdio.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_events.h>

int game_loop(Game_t *game, CosuHitObjectList_t *list)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    return -COSU_ERROR_SDL;
  }

  int err = 0;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  
  bool ret = SDL_CreateWindowAndRenderer(game->window_name,
                                         game->window_width,
                                         game->window_height,
                                         0,
                                         &window,
                                         &renderer);
  if (!ret)
  {
    return -COSU_ERROR_SDL;
  }

  while(1){
    
    if (SDL_PollEvent(&event))
    {
      if (SDL_EVENT_QUIT == event.type)
      {
        break;
      }
    }

    // TODO: call update

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    if (!SDL_RenderClear(renderer))
    {
      err = -COSU_ERROR_SDL;
      goto cleanup;
    }
    if (!SDL_RenderPresent(renderer))
    {
      err = -COSU_ERROR_SDL;
      goto cleanup;
    }
  }

 cleanup:
  
  if (err == -COSU_ERROR_SDL)
  {
    printf("SDL Error: %s\n", SDL_GetError());
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return err;
}

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

#include "cosu.h"

#include <stdio.h>

int main(int argc, char** argv)
{
  CosuHitObjectList_t list = {0};
  int ret = cosu_hit_object_list_parse_file(&list, "maps/test.osu");
  if (ret < 0)
  {
    fprintf(stderr, "Error %d parsing file\n", ret);
    return -1;
  }
  // cosu_hit_object_list_print(&list);

  Game_t game = (Game_t){
    .window_width = 300,
    .window_height = 800,
    .window_name = "cosu!",
  };
  
  ret = game_loop(&game, &list);
  if (ret < 0)
  {
    fprintf(stderr, "Error %d in game loop\n", ret);
    return -1;
  }
  
  cosu_hit_object_list_free(&list);
  return 0;
}

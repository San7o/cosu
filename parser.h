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

#ifndef _COSU_PARSER_H_
#define _COSU_PARSER_H_

enum CosuHitObjectType {
  COSU_TYPE_HIT_CIRCLE = 0,
  COSU_TYPE_SLIDER = 1,
  COSU_TYPE_NEW_COMBO = 2,
  COSU_TYPE_SPINNER = 3,
  COSU_TYPE_HOLD_NOTE = 7,
  COSU_TYPE_ERROR = -1,
};

struct CosuHitObject {
  long int x;
  long int y;
  long int time;
  enum CosuHitObjectType type;
};

typedef struct CosuHitObject CosuHitObject_t;


struct CosuHitObjectListElem {
  struct CosuHitObjectListElem *next;
  struct CosuHitObjectListElem *prev;
  struct CosuHitObject val;
};

typedef struct CosuHitObjectListElem CosuHitObjectListElem_t;

struct CosuHitObjectList {
  CosuHitObjectListElem_t *head;
  CosuHitObjectListElem_t *tail;
};

typedef struct CosuHitObjectList CosuHitObjectList_t;

CosuHitObject_t cosu_hit_object_parse(char* input, int input_len);
void cosu_hit_object_print(CosuHitObject_t object);
int cosu_hit_object_list_parse_file(CosuHitObjectList_t *list,
                                      char* file_name);
void cosu_hit_object_list_append(CosuHitObjectList_t *list,
                                 CosuHitObject_t value);
void cosu_hit_object_list_pop(CosuHitObjectList_t *list);
void cosu_hit_object_list_elem_free(CosuHitObjectListElem_t *elem);
void cosu_hit_object_list_free(CosuHitObjectList_t *list);
void cosu_hit_object_list_elem_print(CosuHitObjectListElem_t *elem);
void cosu_hit_object_list_print(CosuHitObjectList_t *list);

#endif // _COSU_PARSER_H_

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

#include "parser.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parser_next(char *input, int input_len, char separator)
{
  int i = 0;
  while (input[i] != separator && i < input_len) ++i;
  if (i == input_len) return -1;
  return ++i;
}

CosuHitObject_t cosu_hit_object_parse(char* input, int input_len)
{
  if (input_len == 0)
  {
    return (CosuHitObject_t) {
      .type = COSU_TYPE_ERROR,
    };
  }
  
  CosuHitObject_t obj = {0};
  char* endptr = NULL;
  int len = 0;
  
#define CHECK_ERROR  if (endptr >= input + len) \
  do { \
    return (CosuHitObject_t) {  \
      .type = COSU_TYPE_ERROR, \
    }; \
  } while(0)

  len = parser_next(input, input_len, ',');
  obj.x = strtol(input, &endptr, 10);
  CHECK_ERROR;
  input += len;
  
  len = parser_next(input, input_len, ',');
  obj.y = strtol(input, &endptr, 10);
  CHECK_ERROR;
  input += len;

  len = parser_next(input, input_len, ',');
  obj.time = strtol(input, &endptr, 10);
  CHECK_ERROR;
  input += len;

  len = parser_next(input, input_len, ',');
  obj.type = (enum CosuHitObjectType) strtol(input, &endptr, 10);
  CHECK_ERROR;
  input += len;

#undef CHECK_ERROR

  return obj;
}

void cosu_hit_object_print(CosuHitObject_t object)
{
  printf("x: %ld, y: %ld, time: %ld, type: %d\n",
         object.x, object.y, object.time, object.type);
  return;
}

int cosu_hit_object_list_parse_file(CosuHitObjectList_t *list,
                                    char* file_name)
{
  if (list == NULL) return -COSU_ERROR_LIST_NULL;
  if (file_name == NULL) return -COSU_ERROR_FILE_NAME;

  FILE *file = fopen(file_name, "r");
  char* line = NULL;
  size_t line_len;
  ssize_t bytes_read = 0;
  CosuHitObject_t hit_object;
  
  while (bytes_read >= 0) {
    bytes_read = getline(&line, &line_len, file);
    if (bytes_read < 0) return 0;
    
    hit_object = cosu_hit_object_parse(line, line_len);
    cosu_hit_object_list_append(list, hit_object);
    if (line != NULL)
    {
      free(line);
      line = NULL;
    }
  }

  fclose(file);
  return 0;
}

void cosu_hit_object_list_append(CosuHitObjectList_t *list,
                                 CosuHitObject_t value)
{
  if (list == NULL) return;
  
  CosuHitObjectListElem_t *new_elem = (CosuHitObjectListElem_t*) malloc(sizeof(CosuHitObjectListElem_t));

  if (list->tail == NULL)
  {
    new_elem->next = NULL;
    new_elem->prev = NULL;
    new_elem->val = value;
    list->tail = new_elem;
    list->head = new_elem;
    return;
  }
  
  new_elem->next = list->tail;
  new_elem->prev = NULL;
  new_elem->val = value;
  list->tail->prev = new_elem;
  list->tail = new_elem;
  return;
}

void cosu_hit_object_list_pop(CosuHitObjectList_t *list)
{
  if (list == NULL) return;
  if (list->head == NULL) return;
  
  CosuHitObjectListElem_t* head = list->head;
  if (list->head->prev == NULL)
  {
    list->head = NULL;
    free(head);
    return;
  }

  list->head->prev = NULL;
  list->head = list->head->prev;
  free(head);
  return;
}

void cosu_hit_object_list_elem_free(CosuHitObjectListElem_t *elem)
{
  if (elem == NULL) return;
  
  CosuHitObjectListElem_t *next = elem->next;
  free (elem);
  cosu_hit_object_list_elem_free(next);
  return;
}

void cosu_hit_object_list_free(CosuHitObjectList_t *list)
{
  if (list == NULL) return;

  cosu_hit_object_list_elem_free(list->tail);
  return;
}


void cosu_hit_object_list_elem_print(CosuHitObjectListElem_t *elem)
{
  if (elem == NULL) return;
  
  cosu_hit_object_print(elem->val);
  cosu_hit_object_list_elem_print(elem->prev);
  return;
}

void cosu_hit_object_list_print(CosuHitObjectList_t *list)
{
  cosu_hit_object_list_elem_print(list->head);
  return;
}

// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT

#include "note.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void cosu_note_list_push_back(CosuNoteList *list,
                              CosuNote note)
{
  if (!list) return;
  
  CosuNoteListElem *new_elem =
    (CosuNoteListElem*) malloc(sizeof(CosuNoteListElem));

  if (!list->tail)
  {
    new_elem->next = NULL;
    new_elem->prev = NULL;
    new_elem->note = note;
    list->tail = new_elem;
    list->head = new_elem;
    return;
  }
  
  new_elem->next = NULL;
  new_elem->prev = list->tail;
  new_elem->note = note;
  list->tail->next = new_elem;
  list->tail = new_elem;
  return;
}

void cosu_note_list_push_front(CosuNoteList *list,
                               CosuNote note)
{
  if (!list) return;
  
  CosuNoteListElem *new_elem =
    (CosuNoteListElem*) malloc(sizeof(CosuNoteListElem));

  if (!list->head)
  {
    new_elem->next = NULL;
    new_elem->prev = NULL;
    new_elem->note = note;
    list->tail = new_elem;
    list->head = new_elem;
    return;
  }
  
  new_elem->next = list->head;
  new_elem->prev = NULL;
  new_elem->note = note;
  list->head->prev = new_elem;
  list->head = new_elem;
  return;
}

void cosu_note_list_pop_back(CosuNoteList *list)
{
  if (!list) return;
  if (!list->tail) return;
  
  if (list->tail->prev == NULL)
  {
    assert(list->head == list->tail);
    
    free(list->tail);
    list->head = NULL;
    list->tail = NULL;
    return;
  }

  CosuNoteListElem *tail = list->tail;
  list->tail->prev->next = NULL;
  list->tail = list->tail->prev;
  free(tail);
  return;
}

void cosu_note_list_pop_front(CosuNoteList *list)
{
  if (!list) return;
  if (!list->head) return;
  
  if (list->head->next == NULL)
  {
    assert(list->head == list->tail);

    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    return;
  }

  CosuNoteListElem* head = list->head;
  list->head->next->prev = NULL;
  list->head = list->head->next;
  free(head);
  return;
}

CosuNote* cosu_note_list_head(CosuNoteList *list)
{
  if (!list || !list->head) return NULL;
  return &list->head->note;
}

CosuNote* cosu_note_list_tail(CosuNoteList *list)
{
  if (!list || !list->tail) return NULL;
  return &list->tail->note;
}

int cosu_note_list_len(CosuNoteList *list)
{
  if (!list) return 0;

  CosuNoteListElem *it = list->head;
  int count = 0;
  while(it)
  {
    count++;
    it = it->next;
  }
  
  return count;
}

void cosu_note_list_elem_free(CosuNoteListElem *elem)
{
  if (!elem) return;
  
  CosuNoteListElem *next = elem->next;
  free (elem);
  cosu_note_list_elem_free(next);
  return;
}

void cosu_note_list_free(CosuNoteList *list)
{
  if (!list) return;

  cosu_note_list_elem_free(list->head);
  return;
}

void cosu_note_print(CosuNote object)
{
  printf("x: %ld, y: %ld, time: %ld, type: %d\n",
         object.x, object.y, object.time, object.type);
  return;
}

void cosu_note_list_elem_print(CosuNoteListElem *elem)
{
  if (!elem) return;
  
  cosu_note_print(elem->note);
  cosu_note_list_elem_print(elem->next);
  return;
}

void cosu_note_list_print(CosuNoteList *list)
{
  if (!list) return;
  cosu_note_list_elem_print(list->head);
  return;
}

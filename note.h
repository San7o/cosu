// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT

#ifndef _COSU_NOTE_H_
#define _COSU_NOTE_H_

//
// Types
//

typedef enum {
  COSU_TYPE_NOTE = 0,
  COSU_TYPE_SLIDER = 1,
  COSU_TYPE_NEW_COMBO = 2,
  COSU_TYPE_SPINNER = 3,
  COSU_TYPE_HOLD_NOTE = 7,
  COSU_TYPE_ERROR = -1,
} CosuNoteType;

// A single note
typedef struct {
  CosuNoteType type;
  long int x;
  long int y;
  long int time;
} CosuNote;

struct CosuNoteListElem;
typedef struct CosuNoteListElem CosuNoteListElem;

struct CosuNoteListElem {
  CosuNoteListElem *next;
  CosuNoteListElem *prev;
  CosuNote note;
};

typedef struct {
  CosuNoteListElem *head;
  CosuNoteListElem *tail;
} CosuNoteList;

//
// Function definitions
//

void cosu_note_list_push_front(CosuNoteList *list,
                               CosuNote note);
void cosu_note_list_push_back(CosuNoteList *list,
                              CosuNote note);
void cosu_note_list_pop_front(CosuNoteList *list);
void cosu_note_list_pop_back(CosuNoteList *list);

CosuNote* cosu_note_list_head(CosuNoteList *list);
CosuNote* cosu_note_list_tail(CosuNoteList *list);
int cosu_note_list_len(CosuNoteList *list);

void cosu_note_list_free(CosuNoteList *list);

void cosu_note_print(CosuNote object);
void cosu_note_list_print(CosuNoteList *list);

#endif // _COSU_NOTE_H_

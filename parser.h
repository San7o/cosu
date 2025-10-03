// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT

#ifndef _COSU_PARSER_H_
#define _COSU_PARSER_H_

#include "game.h"
#include "note.h"

CosuNote cosu_note_parse(char* input, int input_len);
int cosu_note_list_parse_file(CosuNoteList *list,
                              char* file_name);

#endif // _COSU_PARSER_H_

// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#include "parser.h"
#include "error.h"

#include <string.h>

int parser_next(char *input, int input_len, char separator)
{
  int i = 0;
  while (input[i] != separator && i < input_len) ++i;
  if (i == input_len) return -1;
  return ++i;
}

CosuNote cosu_note_parse(char* input, int input_len)
{
  if (input_len == 0)
  {
    return (CosuNote) {
      .type = COSU_TYPE_ERROR,
    };
  }
  
  CosuNote obj = {0};
  char* endptr = NULL;
  int len = 0;
  
#define CHECK_ERROR  if (endptr >= input + len) \
  do { \
    return (CosuNote) {  \
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
  obj.type = (CosuNoteType) strtol(input, &endptr, 10);
  CHECK_ERROR;
  input += len;

#undef CHECK_ERROR

  // Normalize
  obj.y = 0;
  obj.x = obj.x / 128;
  
  return obj;
}

int cosu_note_list_parse_file(CosuNoteList *list,
                              char* file_name)
{
  if (list == NULL) return -COSU_ERROR_LIST_NULL;
  if (file_name == NULL) return -COSU_ERROR_FILE_NAME;

  FILE *file = fopen(file_name, "r");
  char* line = NULL;
  size_t line_len;
  ssize_t bytes_read = 0;
  CosuNote note;
  
  while (bytes_read >= 0) {
    bytes_read = getline(&line, &line_len, file);
    if (bytes_read < 0) return 0;
    
    note = cosu_note_parse(line, line_len);
    cosu_note_list_push_back(list, note);
    if (line != NULL)
    {
      free(line);
      line = NULL;
    }
  }

  fclose(file);
  return 0;
}

//This file is auto-generated from ./type/lines_type.c
#ifndef __TYPE_LINES_TYPE_C
#define __TYPE_LINES_TYPE_C
//EXPORT

typedef struct _lines {
  unum width_count;
  unum position_count;
  mutable_string* mutable_string;
  struct _lines *prev;
  struct _lines *next;
} lines;


//PUBLIC
lines *lines_malloc(void)  ;
lines *lines_insert(lines *current)  ;
void lines_free(lines *t)  ;
void lines_combine_next(lines *current)  ;
void lines_divide(lines *current_lines, mutable_string *current, uint byte, utf8char divide_char)  ;
lines *lines_select_position_y(lines *head, unum position_y)  ;
void lines_calculate_width(lines *head)  ;
#endif

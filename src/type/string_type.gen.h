//This file is auto-generated from ./type/string_type.c
#ifndef __TYPE_STRING_TYPE_C
#define __TYPE_STRING_TYPE_C
//EXPORT

typedef struct _mutable_string {
  uint byte_count;
  uint position_count;
  uchar string[BUFFER_SIZE];
  struct _mutable_string *next;
} mutable_string;

typedef struct _lines {
  unum width_count;
  unum position_count;
  mutable_string* mutable_string;
  struct _lines *prev;
  struct _lines *next;
} lines;


//PUBLIC
mutable_string *mutable_string_insert(mutable_string *current)  ;
void mutable_string_add_char(mutable_string *head, multibyte_char c)  ;
lines *lines_insert(lines *current)  ;
lines *lines_malloc(void)  ;
void lines_free(lines *t)  ;
void lines_combine_next(lines *current)  ;
void lines_divide(lines *current_lines, mutable_string *current, uint byte, multibyte_char divide_char)  ;
lines *getTextFromPositionY(lines *head, unum position_y)  ;
mutable_string *getLineAndByteFromPositionX(mutable_string *head, unum position_x, uint *byte)  ;
void insert_multibyte_char(mutable_string *mutable_string, uint byte, multibyte_char c)  ;
void delete_multibyte_char(mutable_string *mutable_string, uint byte)  ;
void calculatotion_width(lines *head, uint max_width)  ;
#endif

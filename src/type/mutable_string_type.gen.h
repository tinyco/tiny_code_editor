//This file is auto-generated from ./type/mutable_string_type.c
#ifndef __TYPE_MUTABLE_STRING_TYPE_C
#define __TYPE_MUTABLE_STRING_TYPE_C
//EXPORT

#include "./unsigned_type.gen.h"

typedef struct _mutable_string {
  uint byte_count;
  uint position_count;
  uchar string[BUFFER_SIZE];
  struct _mutable_string *next;
} mutable_string;


//PUBLIC
mutable_string *mutable_string_insert(mutable_string *current)  ;
void mutable_string_add_char(mutable_string *head, multibyte_char c)  ;
mutable_string *mutable_string_select_position_x(mutable_string *head, unum position_x, uint *byte)  ;
multibyte_char get_tail(mutable_string *mutable_string)  ;
void insert_multibyte_char(mutable_string *mutable_string, uint byte, multibyte_char c)  ;
void delete_multibyte_char(mutable_string *mutable_string, uint byte)  ;
#endif

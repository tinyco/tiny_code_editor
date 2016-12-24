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
#include "../main.gen.h" 
mutable_string *mutable_string_malloc()  ;
void mutable_string_all_free(mutable_string *head)  ;
void mutable_string_free(mutable_string *ms)  ;
mutable_string *mutable_string_insert(mutable_string *target)  ;
void mutable_string_add_utf8char_to_tail(mutable_string *head, utf8char c)  ;
mutable_string *mutable_string_select_position_x(mutable_string *head, unum position_x, uint *byte)  ;
utf8char mutable_string_get_tail(mutable_string *ms)  ;
void insert_utf8char(mutable_string *ms, uint byte, utf8char c)  ;
void delete_utf8char(mutable_string *ms, uint byte)  ;
void mutable_string_calculate_width(mutable_string *ms, unum *width_count, unum *position_count)  ;
#endif

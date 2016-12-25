//This file is auto-generated from ./type/cursor_type.c
#ifndef __TYPE_CURSOR_TYPE_C
#define __TYPE_CURSOR_TYPE_C
//EXPORT
typedef struct _cursor {
  unum start_position_x;
  unum start_position_y;
  unum end_position_x;
  unum end_position_y;
} cursor;

//PUBLIC
#include "../main.gen.h"           
#include "../render/context.gen.h" 
cursor cursor_sort_start_end(cursor in)  ;
void cursor_delete_one(context *context)  ;
void cursor_delete_range(context *context)  ;
int cursor_is_range(cursor c)  ;
#endif

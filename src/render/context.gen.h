//This file is auto-generated from ./render/context.c
#ifndef __RENDER_CONTEXT_C
#define __RENDER_CONTEXT_C
//EXPORT
#include "../type/view_size.gen.h"
typedef struct _context {
  char* filename;
  lines* lines;
  view_size view_size;
  mutable_string* clip_board;
  cursor cursor;
  uint header_height;
  uint body_height;
  uint footer_height;
  uint render_start_height;
} context;

//PUBLIC
#include "../main.gen.h" 
void context_initialize(context *context)  ;
void context_read_file(context *context, char *filename)  ;
void context_write_override_file(context *context)  ;
#endif

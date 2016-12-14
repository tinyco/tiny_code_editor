//This file is auto-generated from ./type/context_type.c
#ifndef __TYPE_CONTEXT_TYPE_C
#define __TYPE_CONTEXT_TYPE_C
//EXPORT
#include "../console/screen.gen.h"

typedef struct _context {
	char* filename;
	text* text;
  view_size view_size;
  cursor cursor;
} context;

//PUBLIC
void context_read_file(context* context, char *filename);
void context_write_override_file(context* context);
#endif

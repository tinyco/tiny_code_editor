//This file is auto-generated from ./type/context_type.c
#ifndef __TYPE_CONTEXT_TYPE_C
#define __TYPE_CONTEXT_TYPE_C
//EXPORT
typedef struct _context {
	char* filename;
	string* filestr;
} context;

//PUBLIC
void context_read_file(context* context, char *filename);
#endif


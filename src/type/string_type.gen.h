//This file is auto-generated from ./type/string_type.c
#ifndef __TYPE_STRING_TYPE_C
#define __TYPE_STRING_TYPE_C
//EXPORT
typedef struct _string {
	char str[BUFFER_SIZE];
	struct _string *prev;
	struct _string *next;
} string;

//PUBLIC
string* insert(string *current);
#endif


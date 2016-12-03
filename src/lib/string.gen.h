//This file is auto-generated from ./lib/string.c
#ifndef __LIB_STRING_C
#define __LIB_STRING_C
//EXPORT
#include "../main.gen.h"
typedef struct _string {
  char str[BUFFER_SIZE];
  struct _string *prev;
  struct _string *next;
} string;

//PUBLIC
#endif
string* insert(string *from);

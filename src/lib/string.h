#ifndef STRING
#define STRING

#include "../const.h"

typedef struct _string {
  char str[BUFFER_SIZE];
  struct _string *prev;
  struct _string *next;
} string;

string* insert(string *from);

#endif

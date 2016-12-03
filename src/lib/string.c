typedef struct _string {
  char str[BUFFER_SIZE];
  struct _string *prev;
  struct _string *next;
} string;

string* insert(string *from) {
  string* to = malloc(sizeof(string));
  if (from->next) {
    from->next->prev = to;
    to->next = from->next;
  } else {
    to->next = NULL;
  }
  if(from){
    from->next = to;
  }
  to->prev = from;
  return to;
}

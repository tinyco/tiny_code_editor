#include "../main.gen.h"
#include "./mutable_string_type.gen.h"

/*EXPORT

typedef struct _lines {
  unum width_count;
  unum position_count;
  mutable_string* mutable_string;
  struct _lines *prev;
  struct _lines *next;
} lines;

*/

lines *lines_insert(lines *current) // PUBLIC;
{
  lines *i = malloc(sizeof(lines));
  i->prev = current;
  i->next = NULL;
  if (current) {
    if (current->next) {
      current->next->prev = i;
      i->next = current->next;
    }
    current->next = i;
  }
  i->mutable_string = mutable_string_malloc();
  return i;
}

lines *lines_malloc(void) // PUBLIC;
{
  lines *i = malloc(sizeof(lines));
  i->prev = NULL;
  i->next = NULL;
  i->mutable_string = mutable_string_malloc();
  return i;
}

void lines_free(lines *t) // PUBLIC;
{
  lines *p = t->prev;
  lines *n = t->next;
  p->next = n;
  n->prev = p;
  free(t);
}

void lines_combine_next(lines *current) // PUBLIC;
{
  if (!current && !current->next) {
    return;
  }
  mutable_string *tail = current->mutable_string;
  while (tail->next) {
    tail = tail->next;
  }
  delete_utf8char(tail, tail->byte_count - safed_utf8char_size(mutable_string_get_tail(tail)));
  tail->next = current->next->mutable_string;
  free(current->next);
  current->next = NULL;
}

void lines_divide(lines *current_lines, mutable_string *current, uint byte, utf8char divide_char) // PUBLIC;
{
  lines_insert(current_lines);
  if (current->next) {
    free(current_lines->next->mutable_string);
    current_lines->next->mutable_string = current->next;
    current->next = NULL;
  }
  while (current->byte_count > byte) {
    utf8char tail = mutable_string_get_tail(current);
    current->byte_count -= safed_utf8char_size(tail);
    insert_utf8char(current_lines->next->mutable_string, 0, tail);
  }
  mutable_string_add_utf8char_to_tail(current, divide_char);
}

lines *lines_select_position_y(lines *head, unum position_y) // PUBLIC;
{
  unum i = position_y - 1;
  lines *current_lines = head;
  while (i-- > 0 && current_lines) {
    current_lines = current_lines->next;
  }
  return current_lines;
}

void lines_calculate_width(lines *head) // PUBLIC;
{
  lines *i = head;
  while (i) {
    mutable_string_calculate_width(i->mutable_string, &i->width_count, &i->position_count);
    i = i->next;
  }
}

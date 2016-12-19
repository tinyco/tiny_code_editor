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
  i->mutable_string = malloc(sizeof(mutable_string));
  i->mutable_string->next = NULL;
  i->mutable_string->byte_count = 0;
  return i;
}

lines *lines_malloc(void) // PUBLIC;
{
  lines *head = malloc(sizeof(lines));
  head->prev = NULL;
  head->next = NULL;
  head->mutable_string = malloc(sizeof(mutable_string));
  head->mutable_string->next = NULL;
  head->mutable_string->byte_count = 0;
  return head;
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
  mutable_string *tail = current->mutable_string;
  while (tail->next) {
    tail = tail->next;
  }
  delete_utf8char(tail, tail->byte_count - safed_utf8char_size(mutable_string_get_tail(tail)));
  tail->next = current->next->mutable_string;
  lines_free(current->next);
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
  mutable_string_add_char(current, divide_char);
}

lines *lines_select_position_y(lines *head, unum position_y) // PUBLIC;
{
  unum i = position_y - 1;
  lines *current_lines = head;
  while (i-- > 0 && current_lines) {
    current_lines = current_lines->next;
  }
  if (current_lines)
    return current_lines;
  return NULL;
}

void lines_calculatotion_width(lines *head, uint max_width) // PUBLIC;
{
  static uint prev_width = 0;
  // if(prev_width == max_width) return;//cache hit TODO
  prev_width = max_width;
  lines *current_lines = head;
  mutable_string *current_mutable_string = head->mutable_string;

  uint i;
  while (current_lines) {
    unum total_width = 0;
    unum total_position = 0;
    current_mutable_string = current_lines->mutable_string;
    while (current_mutable_string) {
      int mutable_string_postition = 0;
      i = 0;
      while (i < current_mutable_string->byte_count) {
        int w = utf8char_width(&current_mutable_string->string[i]);
        total_width += w;
        mutable_string_postition++;
        i += safed_utf8char_size(&current_mutable_string->string[i]);
      }
      current_mutable_string->position_count = mutable_string_postition;
      total_position += mutable_string_postition;
      current_mutable_string = current_mutable_string->next;
    }
    current_lines->width_count = total_width;
    current_lines->position_count = total_position;
    current_lines = current_lines->next;
  }
}

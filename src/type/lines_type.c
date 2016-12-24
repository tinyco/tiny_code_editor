#include "../main.gen.h" //PUBLIC

/*EXPORT

typedef struct _lines {
  unum width_count;
  unum position_count;
  mutable_string* mutable_string;
  struct _lines *prev;
  struct _lines *next;
} lines;

*/

lines *lines_malloc(void) // PUBLIC;
{
  lines *ls = malloc(sizeof(lines));
  ls->prev = NULL;
  ls->next = NULL;
  ls->mutable_string = mutable_string_malloc();
  return ls;
}

lines *lines_insert(lines *ls) // PUBLIC;
{
  if (ls) {
    lines *i = lines_malloc();
    i->prev = ls;
    i->next = ls->next;
    if (ls->next) {
      ls->next->prev = i;
    }
    ls->next = i;
    return i;
  } else {
    return lines_malloc();
  }
}

void lines_free_without_mutable_string(lines *ls) {
  lines *p = ls->prev;
  lines *n = ls->next;
  if (p) {
    p->next = n;
  }
  if (n) {
    n->prev = p;
  }
  free(ls);
}

void lines_free(lines *ls) // PUBLIC;
{
  mutable_string_all_free(ls->mutable_string);
  lines_free_without_mutable_string(ls);
}

void lines_combine_next(lines *ls) // PUBLIC;
{
  if (!ls && !ls->next) {
    return;
  }
  mutable_string *tail = ls->mutable_string;
  while (tail->next) {
    tail = tail->next;
  }
  delete_utf8char(tail, tail->byte_count - safed_utf8char_size(mutable_string_get_tail(tail)));
  tail->next = ls->next->mutable_string;
  lines_free_without_mutable_string(ls->next);
}

void lines_divide(lines *ls, mutable_string *target, uint byte, utf8char divide_char) // PUBLIC;
{
  lines_insert(ls);
  if (target->next) {
    free(ls->next->mutable_string);
    ls->next->mutable_string = target->next;
    target->next = NULL;
  }
  while (target->byte_count > byte) {
    utf8char tail = mutable_string_get_tail(target);
    target->byte_count -= safed_utf8char_size(tail);
    insert_utf8char(ls->next->mutable_string, 0, tail);
  }
  mutable_string_add_utf8char_to_tail(target, divide_char);
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

#include "../main.gen.h"           //PUBLIC
#include "../render/context.gen.h" //PUBLIC

/*EXPORT
typedef struct _cursor {
  unum start_position_x;
  unum start_position_y;
  unum end_position_x;
  unum end_position_y;
} cursor;
*/

cursor cursor_sort_start_end(cursor in) // PUBLIC;
{
  cursor c = in;
  unum tmp;
  if (c.end_position_y < c.start_position_y) {
    tmp = c.end_position_x;
    c.end_position_x = c.start_position_x;
    c.start_position_x = tmp;
    tmp = c.end_position_y;
    c.end_position_y = c.start_position_y;
    c.start_position_y = tmp;
  }
  if (c.end_position_x < c.start_position_x && c.end_position_y == c.start_position_y) {
    tmp = c.end_position_x;
    c.end_position_x = c.start_position_x;
    c.start_position_x = tmp;
  }
  return c;
}

void cursor_delete_one(context *context) // PUBLIC;
{
  cursor c = cursor_sort_start_end(context->cursor);
  if (c.start_position_x > 1) {
    uint byte;
    lines *head = lines_select_position_y(context->lines, c.start_position_y);
    mutable_string *ms = mutable_string_select_position_x(head->mutable_string, c.start_position_x - 1, &byte);
    delete_utf8char(ms, byte);
    c.start_position_x -= 1;
  } else if (c.start_position_y > 1) {
    lines *head = lines_select_position_y(context->lines, c.start_position_y - 1);
    lines_combine_next(head);
    c.start_position_x = lines_select_position_y((*context).lines, c.start_position_y - 1)->position_count;
    c.start_position_y -= 1;
  }
}

void cursor_delete_range(context *context) // PUBLIC;
{
  cursor c = cursor_sort_start_end(context->cursor);
  unum i = c.end_position_x - c.start_position_x + 1;
  while (i--) {
    uint byte;
    lines *head = lines_select_position_y(context->lines, c.start_position_y);
    mutable_string *ms = mutable_string_select_position_x(head->mutable_string, c.start_position_x, &byte);
    delete_utf8char(ms, byte);
  }
}

int cursor_is_range(cursor c) // PUBLIC;
{
  return c.start_position_x == c.end_position_x && c.start_position_y == c.end_position_y;
}

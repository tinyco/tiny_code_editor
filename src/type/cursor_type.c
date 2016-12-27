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

void cursor_copy_range(context *context) // PUBLIC;
{
  mutable_string_all_free(context->clip_board);
  context->clip_board = mutable_string_malloc();
  cursor c = cursor_sort_start_end(context->cursor);
  unum s = c.end_position_x - c.start_position_x + 1;
  unum i = 0;
  while (i < s) {
    uint byte;
    lines *head = lines_select_position_y(context->lines, c.start_position_y);
    mutable_string *ms = mutable_string_select_position_x(head->mutable_string, c.start_position_x + i, &byte);
    mutable_string_add_utf8char_to_tail(context->clip_board, &ms->string[byte]);
    i++;
  }
}

void cursor_paste_range(context *context) // PUBLIC;
{
  mutable_string *ms = context->clip_board;
  while (ms) {
    uint i = 0;
    while (i < ms->byte_count) {

      uint byte;
      lines *head = lines_select_position_y(context->lines, context->cursor.start_position_y);
      mutable_string *mutable_string =
          mutable_string_select_position_x(head->mutable_string, context->cursor.start_position_x + i, &byte);
      insert_utf8char(mutable_string, byte, &ms->string[i]);

      i++;
    }
    ms = ms->next;
  }
}

int cursor_is_range(cursor c) // PUBLIC;
{
  return !(c.start_position_x == c.end_position_x && c.start_position_y == c.end_position_y);
}

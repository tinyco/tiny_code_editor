#include "../main.gen.h" //PUBLIC

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


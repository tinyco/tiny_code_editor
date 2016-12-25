#include "../console/color.gen.h"
#include "../main.gen.h" //PUBLIC
#include "context.gen.h"
#include "render_util.gen.h"

void render_body(context context) // PUBLIC;
{
  int height = context.body_height;
  int render_max_height = context.render_start_height + height;
  lines *current_lines = context.lines;
  mutable_string *current_mutable_string = context.lines->mutable_string;
  cursor c = cursor_sort_start_end(context.cursor);

  unum pos_x = 1;
  unum pos_y = 1;
  unum wrote_byte;
  while (current_lines) {
    current_mutable_string = current_lines->mutable_string;
    while (current_mutable_string) {
      wrote_byte = 0;
      if (current_lines->position_count <= 1 && c.start_position_y == pos_y) {
        color_cursor_normal(1);
        printf(" ");
        color_cursor_normal(0);
      }
      while (wrote_byte < current_mutable_string->byte_count) {
        if ((unum)render_max_height > pos_y && pos_y > context.render_start_height) {
          if (c.start_position_x == pos_x && c.start_position_y == pos_y) {
            color_cursor_normal(1);
          }
          if (current_lines->position_count == c.end_position_x && c.end_position_x != 1 &&
              is_break(&current_mutable_string->string[wrote_byte]) && c.end_position_y == pos_y) {
            color_cursor_normal(1);
            printf(" ");
            color_cursor_normal(0);
          }
          wrote_byte += print_one_utf8char(&(current_mutable_string->string[wrote_byte]));
          if (c.end_position_x == pos_x && c.end_position_y == pos_y) {
            color_cursor_normal(0);
          }
          pos_x++;
        } else {
          wrote_byte++;
        }
      }
      current_mutable_string = current_mutable_string->next;
    }
    current_lines = current_lines->next;
    pos_y++;
    height--;
    pos_x = 1;
  }
  while (height > 0) {
    printf("\n");
    height--;
  }
}

#include "render_util.gen.h"

void render_body(context context) // PUBLIC;
{
  int height = context.body_height;
  int render_max_height = context.render_start_height + height;
  text *current_text = context.text;
  mutable_string *current_mutable_string = context.text->mutable_string;

  unum pos_x = 1;
  unum pos_y = 1;
  unum wrote_byte;
  int cursor_color_flag = 0;
  while (current_text) {
    current_mutable_string = current_text->mutable_string;
    while (current_mutable_string) {
      wrote_byte = 0;
      if (current_text->position_count <= 1 && context.cursor.position_y == pos_y) {
        color_cursor_normal(1);
        printf(" ");
        color_cursor_normal(0);
      }
      while (wrote_byte < current_mutable_string->byte_count) {
        if ((unum)render_max_height > pos_y && pos_y > context.render_start_height) {
          if (cursor_color_flag) {
            color_cursor_normal(0);
            cursor_color_flag = 0;
          }
          if (context.cursor.position_x == pos_x && context.cursor.position_y == pos_y) {
            color_cursor_normal(1);
            cursor_color_flag = 1;
          }
          wrote_byte += print_one_multibyte_char(&(current_mutable_string->string[wrote_byte]));
          pos_x++;
        } else {
          wrote_byte++;
        }
      }
      current_mutable_string = current_mutable_string->next;
    }
    current_text = current_text->next;
    pos_y++;
    height--;
    pos_x = 1;
  }
  while (height > 0) {
    printf("\n");
    height--;
  }
}

#include "render_util.gen.h"

void render_body(context context)//PUBLIC;
{
  text* current_text = context.text;
	line* current_line = context.text->line;

  unum pos_x = 1;
  unum pos_y = 1;
  unum wrote_byte;
  int cursor_color_flag = 0;
	while(current_text)
  {
    current_line = current_text->line;
  	while(current_line)
    {
      wrote_byte = 0;
      while(wrote_byte < current_line->byte_count)
      {
        if(cursor_color_flag)
        {
          color_cursor_normal(0);
          cursor_color_flag = 0;
        }
        if(context.cursor.position_x == pos_x && context.cursor.position_y == pos_y)
        {
          color_cursor_normal(1);
          cursor_color_flag = 1;
        }
    		wrote_byte += print_one_mbchar(&(current_line->string[wrote_byte]));
        pos_x++;
      }
	  	current_line = current_line->next;
    }
		current_text = current_text->next;
    pos_y++;
    pos_x=1;
	}
  printf("\n");
}

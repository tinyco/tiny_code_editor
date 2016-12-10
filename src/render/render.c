#include "render_util.gen.h"
void debug_print_text(context context);

void render(context context)//PUBLIC;
{
  view_size view_size;
  view_size = console_size();
  context_header context_header;
  context_header.message = (uchar*)context.filename;
  context_header.view_size = view_size;

  console_clear();
  calculatotion_width(context.text, view_size.width);
  render_header(context_header);
  render_body(context);
  printf("\n---debug---\n");
  debug_print_text(context);
}

void debug_print_text(context context)
{
  text* current_text = context.text;
	line* current_line = context.text->line;

  unum i;
	while(current_text)
  {
    current_line = current_text->line;
    printf("#%llu# ",current_text->width_count);
  	while(current_line)
    {
      i = 0;
      printf("[%db,%dp]",current_line->byte_count, current_line->position_count);
      while(i < current_line->byte_count)
      {
    		printf("%c", current_line->string[i]);
        i++;
      }
	  	current_line = current_line->next;
      if(current_line)printf(" -> ");
    }
		current_text = current_text->next;
    if(current_text)printf("\n-------\n");
	}
  printf("\n");
}

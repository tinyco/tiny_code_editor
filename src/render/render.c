#include "render_util.gen.h"
void debug_print_text(context context);

void vailidate_render_position(context *context) {
  while ((*context).cursor.position_y <= (*context).render_start_height) {
    (*context).render_start_height -= 1;
  }
  while ((*context).cursor.position_y >= (*context).render_start_height + (*context).body_height) {
    (*context).render_start_height += 1;
  }
}

void render_setting(context *context) // PUBLIC;
{
  view_size view_size;
  view_size = console_size();
  (*context).view_size = view_size;
  (*context).header_height = 1;
  (*context).body_height = (*context).view_size.height - 2;
  (*context).footer_height = 1;
  vailidate_render_position(context);
}

void render(context context) // PUBLIC;
{
  context_header context_header;
  context_header.message = (uchar *)context.filename;
  context_header.view_size = context.view_size;
  context_footer context_footer;
  uchar pathname[256]; // TODO
  getcwd((char *)pathname, 256);
  context_footer.message = pathname;
  context_footer.view_size = context.view_size;
  console_clear();
  render_header(context_header);
  render_body(context);
  render_footer(context_footer);
  // debug_print_text(context);
}

void debug_print_text(context context) {
  printf("\n---debug---\n");
  text *current_text = context.text;
  line *current_line = context.text->line;

  unum i;
  while (current_text) {
    current_line = current_text->line;
    printf("#%lluw %llup", current_text->width_count, current_text->position_count);
    while (current_line) {
      i = 0;
      printf("[%db,%dp]", current_line->byte_count, current_line->position_count);
      while (i < current_line->byte_count) {
        if (is_line_break(&current_line->string[i])) {
          printf("<BR>");
        } else {
          printf("%c", current_line->string[i]);
        }
        i++;
      }
      current_line = current_line->next;
      if (current_line)
        printf(" -> ");
    }
    current_text = current_text->next;
    if (current_text)
      printf("\n-------\n");
  }
  printf("\n");
}

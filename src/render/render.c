#include "render_util.gen.h"
void debug_print_lines(context context);

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
  // debug_print_lines(context);
}

void debug_print_lines(context context) {
  printf("\n---debug---\n");
  lines *current_lines = context.lines;
  mutable_string *current_mutable_string = context.lines->mutable_string;

  unum i;
  while (current_lines) {
    current_mutable_string = current_lines->mutable_string;
    printf("#%lluw %llup", current_lines->width_count, current_lines->position_count);
    while (current_mutable_string) {
      i = 0;
      printf("[%db,%dp]", current_mutable_string->byte_count, current_mutable_string->position_count);
      while (i < current_mutable_string->byte_count) {
        if (is_mutable_string_break(&current_mutable_string->string[i])) {
          printf("<BR>");
        } else {
          printf("%c", current_mutable_string->string[i]);
        }
        i++;
      }
      current_mutable_string = current_mutable_string->next;
      if (current_mutable_string)
        printf(" -> ");
    }
    current_lines = current_lines->next;
    if (current_lines)
      printf("\n-------\n");
  }
  printf("\n");
}

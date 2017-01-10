#include "../console/screen.gen.h"
#include "../main.gen.h" //PUBLIC
#include "context.gen.h"
#include "render_body.gen.h"
#include "render_footer.gen.h"
#include "render_header.gen.h"

#ifdef DEBUG_MODE
void debug_print_lines(context context);
#endif

void validate_render_position(context *context) {
  while ((*context).cursor.end_position_y <= (*context).render_start_height) {
    (*context).render_start_height -= 1;
  }
  while ((*context).cursor.end_position_y >= (*context).render_start_height + (*context).body_height) {
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
#ifdef DEBUG_MODE
  (*context).body_height = (*context).view_size.height - 38;
#endif
  (*context).footer_height = 1;
  validate_render_position(context);
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
#ifdef DEBUG_MODE
  debug_print_lines(context);
#endif
}

#ifdef DEBUG_MODE

void debug_print_lines(context context) {
  printf("\n---debug---\n");
  printf("- clip_board: ");
  mutable_string *ms = context.clip_board;
  while (ms) {
    uint i = 0;
    while (i < ms->byte_count) {
      if (is_break(&ms->string[i])) {
        printf("<BR>");
      } else {
        printf("%c", ms->string[i]);
      }
      i++;
    }
    ms = ms->next;
  }
  printf("\n");

  lines *current_lines = context.lines;
  mutable_string *current_mutable_string = context.lines->mutable_string;

  unum i;
  while (current_lines) {
    current_mutable_string = current_lines->mutable_string;
    printf("#%lluw %llup", current_lines->width_count, current_lines->position_count);
    while (current_mutable_string) {
      i = 0;
      printf("[%ub,%up]", current_mutable_string->byte_count, current_mutable_string->position_count);
      while (i < current_mutable_string->byte_count) {
        if (is_break(&current_mutable_string->string[i])) {
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

#endif

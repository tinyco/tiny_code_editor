#include "../file/reader.gen.h"
#include "../file/writer.gen.h"
#include "../main.gen.h" //PUBLIC
#include "context.gen.h"

/*EXPORT
#include "../type/view_size.gen.h"
typedef struct _context {
  char* filename;
  lines* lines;
  view_size view_size;
  mutable_string* clip_board;
  cursor cursor;
  uint header_height;
  uint body_height;
  uint footer_height;
  uint render_start_height;
} context;
*/

void context_initialize(context *context) // PUBLIC;
{
  context->clip_board = mutable_string_malloc();
  context->cursor.start_position_x = 1;
  context->cursor.start_position_y = 1;
  context->cursor.end_position_x = 1;
  context->cursor.end_position_y = 1;
  context->render_start_height = 0;
}

void context_read_file(context *context, char *filename) // PUBLIC;
{
  (*context).filename = malloc(strlen(filename) + 1);
  strcpy((*context).filename, filename);
  (*context).lines = file_read((*context).filename);
}

void context_write_override_file(context *context) // PUBLIC;
{
  file_write((*context).filename, (*context).lines);
}

#include "../main.gen.h"
#include "../file/reader.gen.h"
#include "../file/writer.gen.h"

/*EXPORT
#include "../console/screen.gen.h"

typedef struct _context {
	char* filename;
	text* text;
  view_size view_size;
  uint header_height;
  uint body_height;
  uint footer_height;
  uint render_start_height;
  cursor cursor;
} context;
*/

void context_read_file(context* context, char *filename)//PUBLIC;
{
  (*context).filename = malloc(sizeof(filename));
  strcpy((*context).filename, filename);
  (*context).text = file_read((*context).filename);
}

void context_write_override_file(context* context)//PUBLIC;
{
  file_write((*context).filename, (*context).text);
}

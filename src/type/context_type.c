#include "../main.gen.h"
#include "../file/reader.gen.h"

/*EXPORT
typedef struct _context {
	char* filename;
	text* text;
} context;
*/

void context_read_file(context* context, char *filename)//PUBLIC;
{
  (*context).filename = malloc(sizeof(filename));
  strcpy((*context).filename, filename);
  (*context).text = file_read((*context).filename);
}

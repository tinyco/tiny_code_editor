#include "../main.gen.h"
#include "../file/reader.gen.h"

/*EXPORT
typedef struct _context {
	char* filename;
	string* filestr;
} context;
*/

void context_read_file(context* context, char *filename)//PUBLIC;
{
  (*context).filename = malloc(sizeof(filename));
  strcpy((*context).filename, filename);
  (*context).filestr = file_read((*context).filename);
}

#include "render_util.gen.h"

/*EXPORT
typedef struct _context_header {
  view_size view_size;
	char* message;
} context_header;
*/

void render_header(context_header context)//PUBLIC;
{
	int space = context.view_size.width - strlen(context.message);
  color_header(1);
	printf("%s",context.message);
	while(space-- > 0)
  {
		printf(" ");
	}
	color_header(0);
}

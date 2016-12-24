#include "../console/color.gen.h"
#include "../main.gen.h" //PUBLIC
#include "render_header.gen.h"
#include "render_util.gen.h"

/*EXPORT
typedef struct _context_header {
  view_size view_size;
  uchar* message;
} context_header;
*/

void render_header(context_header context) // PUBLIC;
{
  color_header(1);
  printf(" ");
  trim_print(context.message, context.view_size.width - 2);
  printf(" ");
  color_header(0);
  printf("\n");
}

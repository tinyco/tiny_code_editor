#include "../console/color.gen.h"
#include "../main.gen.h" //PUBLIC
#include "render_footer.gen.h"
#include "render_util.gen.h"

/*EXPORT
typedef struct _context_footer {
  view_size view_size;
  uchar* message;
} context_footer;
*/

void render_footer(context_footer context) // PUBLIC;
{
  color_footer(1);
  printf(" ");
  trim_print(context.message, context.view_size.width - 2);
  printf(" ");
  color_footer(0);
  printf("\n");
}

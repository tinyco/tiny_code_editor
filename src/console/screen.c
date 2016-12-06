#include "../main.gen.h"
#include "screen.gen.h"

/*EXPORT
typedef struct _view_size {
  int width;
  int height;
} view_size;
*/

void console_clear(void)//PUBLIC;
{
  printf("\e[;H\e[2J");
}

view_size console_size(void)//PUBLIC;
{
  view_size view_size;
  view_size.width = 0;
  view_size.height = 0;

  struct winsize ws;
  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1)
  {
    view_size.width = ws.ws_col;
    view_size.height = ws.ws_row;
  }
  return view_size;
}

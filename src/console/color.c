#include "../main.gen.h"

void color_header(int bool)//PUBLIC;
{
  if (bool)
  {
    printf("\e[7m");
  } else {
    printf("\e[m");
  }
}

void color_cursor_normal(int bool)//PUBLIC;
{
  if (bool)
  {
    printf("\e[30m\e[45m");
  } else {
    printf("\e[m");
  }
}

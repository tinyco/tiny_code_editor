#include "../main.gen.h"

void color_header(int bool)//PUBLIC;
{
  if (bool) {
    printf("\e[7m");
  } else {
    printf("\e[m");
  }
}
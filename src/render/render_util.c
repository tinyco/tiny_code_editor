#include "render_util.gen.h"
/*EXPORT
#include "../main.gen.h"
#include "../console/color.gen.h"
#include "../console/screen.gen.h"
#include "render_header.gen.h"
*/

int print_one_mbchar(char* str) //PUBLIC;
{
  int bytes = safed_mbchar_size(str);
  for(int i = 0; i < bytes; i++)
  {
    printf("%c",str[i]);
  }
  return bytes;
}

void trim_print(char* message, int max_width) //PUBLIC;
{
  int messsage_width = string_width(message);
  if (messsage_width <= max_width)
  {
    printf("%s",message);
    int i = max_width - messsage_width;
    while(i-- > 0)
    {
      printf(" ");
    }
  } else {
    int wrote_bytes = 0;
    int wrote_width = 0;
    while(max_width - wrote_width - mbchar_width(&message[wrote_bytes]) > 2)
    {
      wrote_width += mbchar_width(&message[wrote_bytes]);
      wrote_bytes += print_one_mbchar(&message[wrote_bytes]);
    }
    printf("...");
  }
}
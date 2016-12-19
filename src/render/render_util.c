#include "render_util.gen.h"
/*EXPORT
#include "../main.gen.h"
#include "../console/color.gen.h"
#include "render_header.gen.h"
#include "render_body.gen.h"
#include "render_footer.gen.h"
*/

uint print_one_multibyte_char(uchar *str) // PUBLIC;
{
  uint bytes = safed_multibyte_char_size(str);
  uint i;
  for (i = 0; i < bytes; i++) {
    printf("%c", str[i]);
  }
  return bytes;
}

void trim_print(uchar *message, uint max_width) // PUBLIC;
{
  unum messsage_width = string_width(message);
  if (messsage_width <= max_width) {
    printf("%s", message);
    unum i = max_width - messsage_width;
    while (i-- > 0) {
      printf(" ");
    }
  } else {
    unum wrote_bytes = 0;
    unum wrote_width = 0;
    while (max_width - wrote_width - multibyte_char_width(&message[wrote_bytes]) > 2) {
      wrote_width += multibyte_char_width(&message[wrote_bytes]);
      wrote_bytes += print_one_multibyte_char(&message[wrote_bytes]);
    }
    printf("...");
  }
}

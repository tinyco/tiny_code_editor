#include "main.gen.h"

/*EXPORT
#define BUFFER_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "type/unsigned_type.gen.h"
#include "type/mbchar_type.gen.h"
#include "type/string_type.gen.h"
#include "type/cursor_type.gen.h"
#include "type/context_type.gen.h"
*/

#include "render/render.gen.h"

int main(int argc, char *argv[]) {

  if(argc != 2) {
    printf("[error]illegal args\n");
    return(EXIT_FAILURE);
  } else {
    context context;
    context_read_file(&context, argv[1]);
    context.cursor.position_x = 0;
    context.cursor.position_y = 0;
  // while(1)
  // {
  //   input_key key = key_scan();
  //   command = key_parse(key);
  //   context = update_context(context, command);
       render(context);
  // }

    return EXIT_SUCCESS;
  }
}

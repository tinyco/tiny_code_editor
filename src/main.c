#include "main.gen.h"

/*EXPORT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>

#define BUFFER_SIZE 100
#include "type/string_type.gen.h"
#include "type/context_type.gen.h"
*/


int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("[error]illegal args\n");
    return(EXIT_FAILURE);
  } else {
    context context;
    context_set_filename(&context, argv[1]);
    printf("%s\n",context.filename);

  // while(1)
  // {
  //   input_key key = key_scan();
  //   command = key_parse(key);
  //   context = update_context(context, command);
  //   render(context);
  // }

    return EXIT_SUCCESS;
  }
}

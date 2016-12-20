#include "test.gen.h"

/*EXPORT
#define BUFFER_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#include "type/unsigned_type.gen.h"
#include "type/utf8char_type.gen.h"
#include "type/mutable_string_type.gen.h"
#include "type/lines_type.gen.h"
#include "type/cursor_type.gen.h"
#include "render/context.gen.h"
*/

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("illegal args\n");
    return (EXIT_FAILURE);
  } else {
    if (strcmp(argv[1], "utf8char") == 0) {
      printf("test - utf8char\n");
      utf8char uc = utf8char_malloc();
      mbcher_zero_clear(uc);
      printf("- %s\n", uc);
      strcpy((char *)uc, "あ");
      printf("- %s\n", uc);
      printf("- %d\n", safed_utf8char_size(uc));
      printf("- %d\n", is_mutable_string_break(uc));
      printf("- %d\n", utf8char_width(uc));
      strcpy((char *)uc, "\n");
      printf("- %s\n", uc);
      printf("- %d\n", safed_utf8char_size(uc));
      printf("- %d\n", is_mutable_string_break(uc));
      printf("- %d\n", utf8char_width(uc));
      printf("- %d\n", string_width("テストです"));
      printf("- %d\n", string_width("12345"));
      utf8char_free(uc);
    } else if (strcmp(argv[1], "mutable_string") == 0) {
      printf("test mutable_string\n");
      printf("TODO\n");
    }
    printf("finish\n");
    return EXIT_SUCCESS;
  }
}

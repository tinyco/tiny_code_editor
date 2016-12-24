#include "test_src/utf8char.gen.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("illegal args\n");
    return 1;
  } else {
    if (strcmp(argv[1], "utf8char") == 0) {
      test_utf8char();
    } else if (strcmp(argv[1], "mutable_string") == 0) {
      printf("test mutable_string\n");
      printf("TODO\n");
    }
    printf("finish\n");
    return 0;
  }
}

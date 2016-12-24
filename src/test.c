#include "test_src/test_cases.gen.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("illegal args\n");
    return 1;
  } else {
    if (strcmp(argv[1], "utf8char") == 0) {
      printf("test_utf8char start\n");
      test_utf8char();
    } else if (strcmp(argv[1], "mutable_string") == 0) {
      printf("test_mutable_string start\n");
      test_mutable_string();
    }
    printf("finish\n");
    return 0;
  }
}

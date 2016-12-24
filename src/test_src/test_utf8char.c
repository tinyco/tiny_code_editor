#define BUFFER_SIZE 10
#include "../common_includes.h"

void test_utf8char(void) // PUBLIC;
{
  printf("test - utf8char\n");
  utf8char uc = utf8char_malloc();

  utf8char_zero_clear(uc);
  printf("- clear %s\n", uc);

  strcpy((char *)uc, "あ");
  printf("- char %s\n", uc);
  printf("- size %d\n", safed_utf8char_size(uc));
  printf("- is_\\n %d\n", is_mutable_string_break(uc));
  printf("- width %d\n", utf8char_width(uc));

  strcpy((char *)uc, "\n");
  printf("- char %s\n", uc);
  printf("- size %d\n", safed_utf8char_size(uc));
  printf("- is_\\n %d\n", is_mutable_string_break(uc));
  printf("- width %d\n", utf8char_width(uc));

  printf("- jp-width %llu\n", string_width((uchar *)"テストです"));
  printf("- ascii-width %llu\n", string_width((uchar *)"12345"));

  utf8char_free(uc);
}

#include "../consts_for_test.h" // PUBLIC
#include "../common_includes.h" // PUBLIC

void test_utf8char_print(utf8char uc) // PUBLIC;
{
  uint s = safed_utf8char_size(uc);
  printf("- size %d\n", s);
  printf("- char ");
  if (is_mutable_string_break(uc)) {
    printf("\\n");
  } else {
    uint i = 0;
    while (i < s) {
      printf("%c", uc[i]);
      i++;
    }
  }
  printf("\n");
  printf("- width %d\n", utf8char_width(uc));
}

void test_utf8char(void) // PUBLIC;
{
  utf8char uc = utf8char_malloc();

  utf8char_zero_clear(uc);
  printf("- clear %s\n", uc);

  strcpy((char *)uc, "a");
  test_utf8char_print(uc);

  strcpy((char *)uc, "あ");
  test_utf8char_print(uc);

  strcpy((char *)uc, "\n");
  test_utf8char_print(uc);

  printf("- jp-width %llu\n", string_width((uchar *)"テストです"));
  printf("- ascii-width %llu\n", string_width((uchar *)"12345"));

  utf8char_free(uc);
}

#include "../test.gen.h"       // PUBLIC
#include "test_utf8char.gen.h" // PUBLIC

void test_mutable_string_print(mutable_string *ms) // PUBLIC;
{
  while (ms) {
    uint i = 0;
    printf(" - byte_count: %u\n", ms->byte_count);
    printf(" - string: ");
    while (i < ms->byte_count) {
      if (is_break(&ms->string[i])) {
        printf("<BR>");
      } else {
        printf("%c", ms->string[i]);
      }
      i++;
    }
    ms = ms->next;
    printf("\n - move next mutable_string\n");
  }
}

void test_mutable_string(void) // PUBLIC;
{
  printf("---malloc\n");
  mutable_string *ms = mutable_string_malloc();
  test_mutable_string_print(ms);

  printf("---add_utf8char_to_tail\n");
  utf8char c1 = (utf8char) "1";
  utf8char c2 = (utf8char) "２";
  utf8char c3 = (utf8char) "3";
  utf8char c4 = (utf8char) "4";
  uint i = 10;
  while (i--) {
    mutable_string_add_utf8char_to_tail(ms, c1);
    mutable_string_add_utf8char_to_tail(ms, c2);
    mutable_string_add_utf8char_to_tail(ms, c3);
    mutable_string_add_utf8char_to_tail(ms, c4);
  }
  test_mutable_string_print(ms);

  printf("---get_tail\n");
  mutable_string *tail_test = ms;
  while (tail_test) {
    utf8char tail = mutable_string_get_tail(tail_test);
    test_utf8char_print(tail);
    tail_test = tail_test->next;
  }

  printf("---insert\n");
  mutable_string_insert(ms);
  mutable_string_insert(ms);
  mutable_string_insert(ms);
  mutable_string_insert(ms);
  test_mutable_string_print(ms);

  printf("---select_postion_x\n");
  mutable_string_calculate_width(ms, NULL, NULL);
  uint byte;
  mutable_string *select = NULL;
  select = mutable_string_select_position_x(ms, 39, &byte);
  printf("pos -> byte %u\n", byte);
  test_mutable_string_print(select);
  select = mutable_string_select_position_x(ms, 40, &byte);
  printf("pos -> byte %u\n", byte);
  test_mutable_string_print(select);
  select = mutable_string_select_position_x(ms, 41, &byte);
  printf("pos -> byte %u\n", byte);
  if (!select) {
    printf("should null\n");
  }

  utf8char c = (utf8char) "$";
  byte = 1;
  select = mutable_string_select_position_x(ms, 39, &byte);
  printf("---insert_utf8char\n");
  insert_utf8char(select, byte, c);
  insert_utf8char(select, byte, c);
  insert_utf8char(select, byte, c);
  test_mutable_string_print(select);
  printf("---delete_utf8char\n");
  delete_utf8char(select, byte);
  delete_utf8char(select, byte);
  delete_utf8char(select, byte);
  test_mutable_string_print(select);

  mutable_string_all_free(ms);
}

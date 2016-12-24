#include "../consts_for_test.h"  // PUBLIC
#include "../common_includes.h"  // PUBLIC
#include "./test_utf8char.gen.h" // PUBLIC

void print(mutable_string *ms) // PUBLIC;
{
  printf("print start\n");
  while (ms) {
    uint i = 0;
    printf(" - byte_count: %d\n", ms->byte_count);
    printf(" - string: ");
    while (i < ms->byte_count) {
      if (is_mutable_string_break(&ms->string[i])) {
        printf("<BR>");
      } else {
        printf("%c", ms->string[i]);
      }
      i++;
    }
    ms = ms->next;
    printf("\n - next\n");
  }
  printf("print end\n");
}

void test_mutable_string(void) // PUBLIC;
{
  mutable_string *ms = mutable_string_malloc();
  print(ms);

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
  print(ms);

  mutable_string *tail_test = ms;
  while (tail_test) {
    utf8char tail = mutable_string_get_tail(tail_test);
    test_utf8char_print(tail);
    tail_test = tail_test->next;
  }

  mutable_string_insert(ms);
  mutable_string_insert(ms);
  mutable_string_insert(ms);
  mutable_string_insert(ms);
  print(ms);

  mutable_string_free(ms);

  // mutable_string *mutable_string_select_position_x(mutable_string *head, unum position_x, uint *byte)  ;
  // void insert_utf8char(mutable_string *mutable_string, uint byte, utf8char c)  ;
  // void delete_utf8char(mutable_string *mutable_string, uint byte)  ;
}

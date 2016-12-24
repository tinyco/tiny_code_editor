#include "../consts_for_test.h"        // PUBLIC
#include "../common_includes.h"        // PUBLIC
#include "./test_mutable_string.gen.h" // PUBLIC

void test_lines_print(lines *ls) // PUBLIC;
{
  lines *i = ls;
  while (i) {
    printf(" - %llu width, %llu position_count", i->width_count, i->position_count);
    test_mutable_string_print(ls->mutable_string);
    i = i->next;
    printf(" - move next lines");
  }
  printf("\n");
}

void test_lines(void) // PUBLIC;
{
  lines *ls = lines_malloc();
  test_lines_print(ls);

  // utf8char c1 = (utf8char) "東";
  // utf8char c2 = (utf8char) "西";
  // utf8char c3 = (utf8char) "南";
  // utf8char c4 = (utf8char) "北";
  // uint i = 10;
  // while (i--) {
  //   mutable_string_add_utf8char_to_tail(ms, c1);
  //   mutable_string_add_utf8char_to_tail(ms, c2);
  //   mutable_string_add_utf8char_to_tail(ms, c3);
  //   mutable_string_add_utf8char_to_tail(ms, c4);
  // }

// lines *lines_insert(lines *current)  ;
// void lines_free(lines *t)  ;
// void lines_combine_next(lines *current)  ;
// void lines_divide(lines *current_lines, mutable_string *current, uint byte, utf8char divide_char)  ;
// lines *lines_select_position_y(lines *head, unum position_y)  ;
// void lines_calculate_width(lines *head)  ;
}

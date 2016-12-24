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
  printf("---malloc\n");
  lines *ls = lines_malloc();
  lines_calculate_width(ls);
  test_lines_print(ls);

  printf("---add_char\n");
  mutable_string_add_utf8char_to_tail(ls->mutable_string, (utf8char) "東");
  mutable_string_add_utf8char_to_tail(ls->mutable_string, (utf8char) "西");
  mutable_string_add_utf8char_to_tail(ls->mutable_string, (utf8char) "南");
  mutable_string_add_utf8char_to_tail(ls->mutable_string, (utf8char) "北");
  lines_calculate_width(ls);
  test_lines_print(ls);

  printf("---insert TODO\n");
  lines *insert = lines_insert(ls);
  mutable_string_add_utf8char_to_tail(ls->mutable_string, (utf8char) "\n");
  mutable_string_add_utf8char_to_tail(insert->mutable_string, (utf8char) "白");
  mutable_string_add_utf8char_to_tail(insert->mutable_string, (utf8char) "発");
  mutable_string_add_utf8char_to_tail(insert->mutable_string, (utf8char) "中");
  lines_calculate_width(ls);
  test_lines_print(ls);
  printf("---inserted\n");
  test_lines_print(insert);

  printf("---combine\n");
  lines_combine_next(ls);
  lines_calculate_width(ls);
  test_lines_print(ls);

  printf("---divide TODO\n");
  lines_divide(ls, ls->mutable_string, safed_utf8char_size((utf8char) "東"), (utf8char) "\n");
  lines_calculate_width(ls);
  test_lines_print(ls);
  // lines *lines_select_position_y(lines *head, unum position_y)  ;
  // lines_free(ls);
}

#include "../test.gen.h" // PUBLIC
#include "test_mutable_string.gen.h"

void test_lines_print(lines *ls) // PUBLIC;
{
  lines *i = ls;
  while (i) {
    printf(" - %llu width, %llu position_count", i->width_count, i->position_count);
    test_mutable_string_print(i->mutable_string);
    i = i->next;
    printf(" - move next lines\n");
  }
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

  printf("---insert\n");
  lines *insert = lines_insert(ls);
  lines_calculate_width(ls);
  test_lines_print(ls);

  printf("---inserted_all\n");
  mutable_string_add_utf8char_to_tail(ls->mutable_string, (utf8char) "\n");
  mutable_string_add_utf8char_to_tail(insert->mutable_string, (utf8char) "白");
  mutable_string_add_utf8char_to_tail(insert->mutable_string, (utf8char) "発");
  mutable_string_add_utf8char_to_tail(insert->mutable_string, (utf8char) "中");
  lines_calculate_width(ls);
  test_lines_print(ls);
  printf("---inserted_line\n");
  test_lines_print(insert);

  printf("---combine\n");
  lines_combine_next(ls);
  lines_calculate_width(ls);
  test_lines_print(ls);

  printf("---divide\n");
  lines_divide(ls, ls->mutable_string, safed_utf8char_size((utf8char) "東"), (utf8char) "\n");
  lines_calculate_width(ls);
  test_lines_print(ls);

  printf("---select\n");
  lines *select = NULL;
  select = lines_select_position_y(ls, 2);
  test_lines_print(select);
  select = lines_select_position_y(ls, 3);
  if (!select) {
    printf("should null\n");
  };

  select = lines_select_position_y(ls, 1);
  lines_free(select);
  select = lines_select_position_y(ls, 2);
  lines_free(select);
}

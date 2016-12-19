#include "../main.gen.h"

lines *file_read(const char *filepath) // PUBLIC;
{
  FILE *fp;
  if ((fp = fopen(filepath, "r")) == NULL) {
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
  }

  lines *head = lines_malloc();

  lines *current_lines = head;
  mutable_string *current_mutable_string = head->mutable_string;

  multibyte_char buf = multibyte_char_malloc();
  uchar c;
  mbcher_zero_clear(buf);
  uint len = 0;
  int mbsize;
  while (1) {
    c = (uchar)fgetc(fp);
    if (feof(fp))
      break;
    buf[len] = c;
    mbsize = multibyte_char_size(buf, len + 1);
    if (mbsize > 0) {
      mutable_string_add_char(current_mutable_string, buf);
      if (is_mutable_string_break(buf)) {
        current_lines = lines_insert(current_lines);
        current_mutable_string = current_lines->mutable_string;
      }
      mbcher_zero_clear(buf);
      len = 0;
    } else if (mbsize == MULTIBYTE_CHAR_NOT_FILL) {
      len++;
    } else {
      mbcher_zero_clear(buf);
    }
  }

  multibyte_char_free(buf);
  fclose(fp);
  return head;
}

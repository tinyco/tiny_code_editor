#include "../main.gen.h"

text *file_read(const char *filepath) // PUBLIC;
{
  FILE *fp;
  if ((fp = fopen(filepath, "r")) == NULL) {
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
  }

  text *head = text_malloc();

  text *current_text = head;
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
        current_text = text_insert(current_text);
        current_mutable_string = current_text->mutable_string;
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

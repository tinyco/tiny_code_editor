#include "../main.gen.h"

void file_write(const char *filepath, lines *head) // PUBLIC;
{
  FILE *fp;
  if ((fp = fopen(filepath, "w")) == NULL) {
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
  }

  lines *current_lines = head;
  mutable_string *current_mutable_string = head->mutable_string;

  while (current_lines) {
    current_mutable_string = current_lines->mutable_string;
    while (current_mutable_string) {
      fwrite(current_mutable_string->string, sizeof current_mutable_string->string[0], current_mutable_string->byte_count, fp);
      current_mutable_string = current_mutable_string->next;
    }
    current_lines = current_lines->next;
  }
  fclose(fp);
}

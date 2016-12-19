#include "../main.gen.h"

void file_write(const char *filepath, text *head) // PUBLIC;
{
  FILE *fp;
  if ((fp = fopen(filepath, "w")) == NULL) {
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
  }

  text *current_text = head;
  mutable_string *current_mutable_string = head->mutable_string;

  while (current_text) {
    current_mutable_string = current_text->mutable_string;
    while (current_mutable_string) {
      fwrite(current_mutable_string->string, sizeof current_mutable_string->string[0], current_mutable_string->byte_count, fp);
      current_mutable_string = current_mutable_string->next;
    }
    current_text = current_text->next;
  }
  fclose(fp);
}

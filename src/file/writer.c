#include "../main.gen.h"

void file_write(const char* filepath, text* head)//PUBLIC;
{
  FILE *fp;
  if((fp = fopen(filepath, "w")) == NULL)
  {
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
  }

  text* current_text = head;
  line* current_line = head->line;

  while(current_text)
  {
    current_line = current_text->line;
    while(current_line)
    {
      fwrite(
        current_line->string,
        sizeof current_line->string[0],
        current_line->byte_count,
        fp);
      current_line = current_line->next;
    }
    current_text = current_text->next;
  }
  fclose(fp);
}
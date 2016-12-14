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

  unum i;
	while(current_text)
  {
    current_line = current_text->line;
  	while(current_line)
    {
      i = 0;
      while(i < current_line->byte_count)
      {
        fputc(current_line->string[i], fp);
        i++;
      }
	  	current_line = current_line->next;
    }
		current_text = current_text->next;
	}
	fclose(fp);
}
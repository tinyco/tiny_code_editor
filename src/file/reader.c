#include "../main.gen.h"

text* file_read(char* filepath)//PUBLIC;
{
	FILE *fp;
	if((fp = fopen(filepath, "r")) == NULL)
	{
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
	}

	text* head = text_malloc();

	text* current_text = head;
	line* current_line = head->line;

	mbchar buf = mbchar_malloc();
	char c;
	mbcher_zero_clear(buf);
	int len = 0;
	int mbsize;
	while(1)
	{
		c = (char)fgetc(fp);
		if (feof(fp))	break;
		buf[len] = c;
		mbsize = mbchar_size(buf, len + 1);
		if(mbsize > 0) {
			line_add_char(current_line, buf);
			if(isLineBreak(buf)) {
				current_text = text_insert(current_text);
				current_line = current_text->line;
			}
			mbcher_zero_clear(buf);
			len = 0;
		} else if (mbsize == MBCHAR_NOT_FILL) {
			len++;
		} else {
			mbcher_zero_clear(buf);
		}
	}

	mbchar_free(buf);
	fclose(fp);
	return head;
}
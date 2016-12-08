#include "../main.gen.h"

/*EXPORT

typedef struct _line {
	int byte_count;
	uchar string[BUFFER_SIZE];
	struct _line *next;
} line;

typedef struct _text {
	int height;
	line* line;
	struct _text *prev;
	struct _text *next;
} text;

*/

line* line_insert(line* current)//PUBLIC;
{
	line* i = malloc(sizeof(line));
	i->next = current->next;
	current->next = i;
	i->byte_count = 0;
	return i;
}

void line_add_char(line* head, mbchar c)//PUBLIC;
{
	line* current = head;
	while(current->byte_count >= BUFFER_SIZE - safed_mbchar_size(c)){
		if(current->next)
		{
			current = current->next;
		}
		else
		{
			current = line_insert(current);
		}
	}
	int offset = 0;
	while(offset < safed_mbchar_size(c)){
		current->string[current->byte_count] = c[offset];
		current->byte_count++;
		offset++;
	}
}

text* text_insert(text* current)//PUBLIC;
{
	text* i = malloc(sizeof(text));
  if (current->next)
	{
    current->next->prev = i;
    i->next = current->next;
  } else {
    i->next = NULL;
  }
	if(current)
	{
  	current->next = i;
	}
	i->prev = current;
	i->line = malloc(sizeof(line));
	i->line->next = NULL;
	i->line->byte_count = 0;
	return i;
}

text* text_malloc(void)//PUBLIC;
{
	text* head = malloc(sizeof(text));
	head->prev = NULL;
	head->next = NULL;
	head->line = malloc(sizeof(line));
	head->line->next = NULL;
	head->line->byte_count = 0;
	return head;
}

void calculatotion_height(text* head, int max_width)//PUBLIC;
{
	text* current_text = head;
	line* current_line = head->line;

  int i;
	while(current_text)
  {
		int total_width = 0;
    current_line = current_text->line;
  	while(current_line)
    {
      i = 0;
      while(i < current_line->byte_count)
      {
				total_width += mbchar_width(&current_line->string[i]);
		    i += safed_mbchar_size(&current_line->string[i]);
      }
	  	current_line = current_line->next;
    }
		current_text->height = total_width / max_width + 1;
		current_text = current_text->next;
	}
}
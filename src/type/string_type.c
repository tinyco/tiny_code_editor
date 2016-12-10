#include "../main.gen.h"

/*EXPORT

typedef struct _line {
	uint byte_count;
	uint position_count;
	uchar string[BUFFER_SIZE];
	struct _line *next;
} line;

typedef struct _text {
	unum width_count;
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
	uint offset = 0;
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

line* getLineHeadFromPositionY(text* head, unum position_y)//PUBLIC;
{
	unum i = position_y-1;
	text* current_text = head;
	while(i-- > 0 && current_text)
	{
		current_text = current_text->next;
	}
	if(current_text) return current_text->line;
	return NULL;
}

line* getLineAndByteFromPositionX(line* head, unum position_x, uint* byte)//PUBLIC;
{
	unum i = position_x;
	line* current_line = head;
	while(current_line && i > current_line->position_count)
	{
		i -= current_line->position_count;
		current_line = current_line->next;
	}
	if(current_line)
	{
		*byte = 0;
		while(i--) {
			*byte += safed_mbchar_size(&current_line->string[*byte]);
		}
		return current_line;
	}
	return NULL;
}


void calculatotion_width(text* head, uint max_width)//PUBLIC;
{
	static uint prev_width = 0;
	if(prev_width == max_width) return;//cache hit
	prev_width = max_width;
	text* current_text = head;
	line* current_line = head->line;

  uint i;
	while(current_text)
  {
		unum total_width = 0;
    current_line = current_text->line;
  	while(current_line)
    {
			int line_postition = 0;
      i = 0;
      while(i < current_line->byte_count)
      {
				int w = mbchar_width(&current_line->string[i]);
				total_width += w;
				line_postition++;
		    i += safed_mbchar_size(&current_line->string[i]);
      }
			current_line->position_count = line_postition;
	  	current_line = current_line->next;
    }
		current_text->width_count = total_width;
		current_text = current_text->next;
	}
}
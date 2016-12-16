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
	unum position_count;
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
	i->prev = current;
	i->next = NULL;
	if (current) {
		if (current->next) {
			current->next->prev = i;
			i->next = current->next;
		}
		current->next = i;
	}
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

void text_free(text* t)//PUBLIC;
{
	text* p = t->prev;
	text* n = t->next;
	p->next = n;
	n->prev = p;
	free(t);
}

mbchar get_tail(line* line);
void text_combine_next(text* current)//PUBLIC;
{
	line* tail = current->line;
	while(tail->next)
	{
		tail = tail->next;
	}
	delete_mbchar(tail, tail->byte_count - safed_mbchar_size(get_tail(tail)));
	tail->next = current->next->line;
	text_free(current->next);
}

void text_divide(text* current_text, line* current, uint byte, mbchar divide_char)//PUBLIC;
{
	text_insert(current_text);
	if (current->next)
	{
		free(current_text->next->line);
		current_text->next->line = current->next;
		current->next = NULL;
	}
	while(current->byte_count > byte)
	{
		mbchar tail = get_tail(current);
		current->byte_count -= safed_mbchar_size(tail);
		insert_mbchar(current_text->next->line, 0, tail);
	}
	line_add_char(current, divide_char);
}

text* getTextFromPositionY(text* head, unum position_y)//PUBLIC;
{
	unum i = position_y-1;
	text* current_text = head;
	while(i-- > 0 && current_text)
	{
		current_text = current_text->next;
	}
	if(current_text) return current_text;
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
		while(i-- > 1) // insert before cursor
		{
			*byte += safed_mbchar_size(&current_line->string[*byte]);
		}
		return current_line;
	}
	return NULL;
}

mbchar get_tail(line* line)
{
	uint i = 0;
	while(line->byte_count > i + safed_mbchar_size(&line->string[i]))
	{
		i+= safed_mbchar_size(&line->string[i]);
	}
	return &line->string[i];
}

void insert_mbchar(line* line, uint byte, mbchar c)//PUBLIC;
{
	uint s = safed_mbchar_size(c);
	if(line->byte_count + UTF8_MAX_BYTE >= BUFFER_SIZE)
	{
		line_insert(line);
	}
	while(BUFFER_SIZE <= line->byte_count + s)
	{
		mbchar tail = get_tail(line);
		line->byte_count -= safed_mbchar_size(tail);
		insert_mbchar(line->next, 0, tail);
	}
	uint move = line->byte_count;
	while(move > byte)
	{
		move--;
		line->string[move+s] = line->string[move];
	}

	uint offset = 0;
	while(offset < s){
		line->string[byte+offset] = c[offset];
		line->byte_count++;
		offset++;
	}
}


void delete_mbchar(line* line, uint byte)//PUBLIC;
{
	uint s = safed_mbchar_size(&line->string[byte]);
	uint move = byte;
	while(move < line->byte_count)
	{
		line->string[move] = line->string[move+s];
		move++;
	}
	line->byte_count-=s;
}

void calculatotion_width(text* head, uint max_width)//PUBLIC;
{
	static uint prev_width = 0;
	// if(prev_width == max_width) return;//cache hit TODO
	prev_width = max_width;
	text* current_text = head;
	line* current_line = head->line;

  uint i;
	while(current_text)
  {
		unum total_width = 0;
		unum total_position = 0;
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
			total_position += line_postition;
	  	current_line = current_line->next;
    }
		current_text->width_count = total_width;
		current_text->position_count = total_position;
		current_text = current_text->next;
	}
}

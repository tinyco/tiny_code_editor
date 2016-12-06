#include "../main.gen.h"

/*EXPORT

typedef struct _line {
	int count;
	char string[BUFFER_SIZE];
	struct _line *next;
} line;

typedef struct _text {
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
	i->count = 0;
	return i;
}

void line_set_string(line* head, char* string)//PUBLIC;
{
	int read = 0;
	line* current = head;
	while(string[read]) {
		current->string[current->count] = string[read];
		read++;
		current->count++;
		if(current->count == BUFFER_SIZE-1) {
			if(current->next)
			{
				current = current->next;
			}
			else
			{
				current = line_insert(current);
			}
		}
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
	i->line->count = 0;
	return i;
}

text* text_malloc(void)//PUBLIC;
{
	text* head = malloc(sizeof(text));
	head->prev = NULL;
	head->next = NULL;
	head->line = malloc(sizeof(line));
	head->line->count = 0;
	return head;
}
#include "../main.gen.h"

/*EXPORT
typedef struct _string {
	char str[BUFFER_SIZE];
	struct _string *prev;
	struct _string *next;
} string;
*/

string* insert(string *current)//PUBLIC;
{
	string* i = malloc(sizeof(string));
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
	return i;
}

#include "../main.gen.h"

string* file_read(char* filepath)//PUBLIC;
{
	FILE *fp;
	char buf[BUFFER_SIZE];
	double max, min;

	if((fp = fopen(filepath, "r")) == NULL)
	{
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
	}

	string* head = malloc(sizeof(string));
	head->prev = NULL;
	head->next = NULL;

	string* current = head;
	while(fgets(buf, sizeof(buf), fp))
	{
		strcpy(current->str, buf);
		insert(current);
		current = current->next;
	}
	fclose(fp);
	return head;
}

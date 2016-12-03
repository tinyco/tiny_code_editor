#include "../main.gen.h"
#include "string.gen.h"//PUBLIC

#include <stdio.h>
#include <string.h>

void file_read(char* filename, string* head)//PUBLIC;
{
  FILE *fp;
  char buf[BUFFER_SIZE];

  if((fp = fopen(filename, "r")) == NULL) {
    printf("[error]can't open\n");
    return;
  }
  string* current = head;
  while(fgets(buf, sizeof(buf), fp)) {
    strcpy(current->str, buf);
    insert(current);
    current = current->next;
  }
  fclose(fp);
}

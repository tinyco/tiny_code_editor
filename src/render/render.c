#include "../main.gen.h"
#include "../console/color.gen.h"
#include "../console/screen.gen.h"

void render(context context)//PUBLIC;
{
  console_clear();
  string* current = context.filestr;
	while(current){
		printf("%s", current->str);
		current = current->next;
	}
	printf("\n");
}
#include "../main.gen.h"
#include "../console/color.gen.h"
#include "../console/screen.gen.h"
#include "render_header.gen.h"

void render(context context)//PUBLIC;
{

  view_size view_size;
  view_size = console_size();
  context_header context_header;
  context_header.message = context.filename;
  context_header.view_size = view_size;

  console_clear();
  render_header(context_header);

  string* current = context.filestr;
	while(current){
		printf("%s", current->str);
		current = current->next;
	}
	printf("\n");
}

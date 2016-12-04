//This file is auto-generated from ./console/screen.c
#ifndef __CONSOLE_SCREEN_C
#define __CONSOLE_SCREEN_C
//EXPORT
typedef struct _view_size {
  int width;
  int height;
} view_size;

//PUBLIC
void console_clear(void);
view_size console_size(void);
#endif


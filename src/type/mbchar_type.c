#include "../main.gen.h"

typedef char* mbchar;//PUBLIC

mbchar mbchar_malloc(void)//PUBLIC;
{
  return malloc(sizeof(char) * MB_CUR_MAX);
}

void mbchar_free(mbchar mbchar)//PUBLIC;
{
  return free(mbchar);
}

mbchar mbcher_zero_clear(mbchar mbchar)//PUBLIC;
{
  int i = MB_CUR_MAX;
  while(i--)
  {
    mbchar[i] = '\0';
  }
  return mbchar;
}

int mbchar_size(mbchar mbchar)//PUBLIC;
{
  return mblen(mbchar, MB_CUR_MAX);
}

int isLineBreak(mbchar mbchar)//PUBLIC;
{
  return (mbchar[0] == '\n' ? 1 : 0);
}
#include "../main.gen.h"

uchar get_single_byte_key()
{
  static int is_init = 0;
  static struct termios cooked_mode;
  static struct termios raw_mode;
  if(!is_init)
  {
    tcgetattr(STDIN_FILENO, &cooked_mode);
    raw_mode = cooked_mode;
    cfmakeraw(&raw_mode);
  }
  uchar key;
  tcsetattr(STDIN_FILENO, 0, &raw_mode);
  key = getchar();
  tcsetattr(STDIN_FILENO, 0, &cooked_mode);
  return key;
}

mbchar keyboard_scan(mbchar *out)//PUBLIC;
{
  mbcher_zero_clear(*out);
  uint i = 0;
  while(mbchar_size(*out, i) < 0)
  {
    *out[i] = get_single_byte_key();
    i++;
    if(mbchar_size(*out, i) == MBCHAR_ILLIEGAL)
    {
      mbcher_zero_clear(*out);
    }
  }
  return *out;
}
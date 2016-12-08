#include "../main.gen.h"
#include "command.gen.h"

command command_parse(mbchar key)//PUBLIC;
{
  command cmd;
  cmd.command_key = NONE;
  cmd.command_value = key;
  char up[3]    = {0x1B,0x5B,0x41};
  char down[3]  = {0x1B,0x5B,0x42};
  char right[3] = {0x1B,0x5B,0x43};
  char left[3]  = {0x1B,0x5B,0x44};
  if(!strcmp((const char*)key, "u"))cmd.command_key = UP;
  if(!strcmp((const char*)key, "d"))cmd.command_key = DOWN;
  if(!strcmp((const char*)key, "r"))cmd.command_key = RIGHT;
  if(!strcmp((const char*)key, "l"))cmd.command_key = LEFT;
  if(!strcmp((const char*)key, "e"))cmd.command_key = EXIT;

  return cmd;
}

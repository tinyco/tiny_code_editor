#include "../main.gen.h"
#include "command.gen.h"

enum ControlKeyFlag {NOT_CTRL, ALLOW_1, ALLOW_2};

command command_parse(mbchar key)//PUBLIC;
{
  static enum ControlKeyFlag flag = 0;
  command cmd;
  cmd.command_key = NONE;
  if(key[0] == 0x1B && flag == NOT_CTRL) {
    flag = ALLOW_1;
  } else if (key[0] == 0x5B && flag == ALLOW_1) {
    flag = ALLOW_2;
  } else if (flag == ALLOW_2) {
    if(key[0] == 0x41) cmd.command_key = UP;
    if(key[0] == 0x42) cmd.command_key = DOWN;
    if(key[0] == 0x43) cmd.command_key = RIGHT;
    if(key[0] == 0x44) cmd.command_key = LEFT;
    flag = NOT_CTRL;
  } else {
    flag = NOT_CTRL;
    cmd.command_key = INSERT;
  }
  cmd.command_value = key;
  if(!strcmp((const char*)key, "e"))cmd.command_key = EXIT;

  return cmd;
}

#include "../main.gen.h"
#include "command.gen.h"

enum ControlKeyFlag {NOT_CTRL, ALLOW_1, ALLOW_2};

command command_parse(mbchar key)//PUBLIC;
{
  static enum ControlKeyFlag flag = 0;
  command cmd;
  cmd.command_key = NONE;
  cmd.command_value = key;
  if(key[0] == 0x1B && flag == NOT_CTRL) {
    flag = ALLOW_1;
  } else if (key[0] == 0x5B && flag == ALLOW_1) {
    flag = ALLOW_2;
  } else if (flag == ALLOW_2) {
    /**/ if(key[0] == 0x41) {cmd.command_key = UP;}
    else if(key[0] == 0x42) {cmd.command_key = DOWN;}
    else if(key[0] == 0x43) {cmd.command_key = RIGHT;}
    else if(key[0] == 0x44) {cmd.command_key = LEFT;}
    flag = NOT_CTRL;
  } else {
    /**/ if(key[0] == 0x11) {cmd.command_key = EXIT;}
    else if(key[0] == 0x7F) {cmd.command_key = DELETE;}
    else if(key[0] == 0x0D) {cmd.command_key = ENTER; cmd.command_value=(mbchar)"\n";}
    else if(key[0] == 0x20) {cmd.command_key = INSERT; cmd.command_value=(mbchar)" ";}
    /**/ else               {cmd.command_key = INSERT;}
    flag = NOT_CTRL;
  }
  return cmd;
}

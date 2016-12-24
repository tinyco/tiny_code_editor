#include "../main.gen.h"
#include "command.gen.h"

enum ControlKeyFlag { NORMAL_KEY, ESC_KEY, ARROW_KEY };

command command_parse(utf8char key) // PUBLIC;
{
  static enum ControlKeyFlag flag = 0;
  command cmd;
  cmd.command_key = NONE;
  cmd.command_value = key;
  if (key[0] == 0x1B && flag == NORMAL_KEY) {
    flag = ESC_KEY;
  } else if (key[0] == 0x5B && flag == ESC_KEY) {
    flag = ARROW_KEY;
  } else if (flag == ARROW_KEY) {
    if (key[0] == 0x41) {
      cmd.command_key = UP;
    } else if (key[0] == 0x42) {
      cmd.command_key = DOWN;
    } else if (key[0] == 0x43) {
      cmd.command_key = RIGHT;
    } else if (key[0] == 0x44) {
      cmd.command_key = LEFT;
    }
    flag = NORMAL_KEY;
  } else {
    if (key[0] == 0x11) {
      cmd.command_key = EXIT;
    } else if (key[0] == 0x7F) {
      cmd.command_key = DELETE;
    } else if (key[0] == 0x0D) {
      cmd.command_key = ENTER;
      cmd.command_value = (utf8char) "\n";
    } else if (key[0] == 0x20) {
      cmd.command_key = INSERT;
      cmd.command_value = (utf8char) " ";
    } else if (key[0] == 0x13) {
      cmd.command_key = SAVE_OVERRIDE;
    } else {
      cmd.command_key = INSERT;
    }
    flag = NORMAL_KEY;
  }
  return cmd;
}

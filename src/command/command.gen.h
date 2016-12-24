//This file is auto-generated from ./command/command.c
#ifndef __COMMAND_COMMAND_C
#define __COMMAND_COMMAND_C
//EXPORT

enum CommandType {NONE, INSERT, DELETE, ENTER, UP, DOWN, LEFT, RIGHT, SAVE_OVERRIDE, EXIT};

typedef struct _command {
  enum CommandType command_key;
  utf8char command_value;
} command;


//PUBLIC
#endif

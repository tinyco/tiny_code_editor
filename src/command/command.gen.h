//This file is auto-generated from ./command/command.c
#ifndef __COMMAND_COMMAND_C
#define __COMMAND_COMMAND_C
//EXPORT

enum CommandType {NONE, UP, DOWN, LEFT, RIGHT, EXIT};

typedef struct _command {
  enum CommandType command_key;
  mbchar command_value;
} command;

#include "command_parse.gen.h"


//PUBLIC
#endif

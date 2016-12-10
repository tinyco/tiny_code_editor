#include "../main.gen.h"
#include "command.gen.h"

// void vailidate_cursor_position(context *context)
// {
// TODO
// }


void command_perform(command command, context *context)//PUBLIC;
{
  switch(command.command_key)
  {
    case UP:
      (*context).cursor.position_y -= 1;
      break;
    case DOWN:
      (*context).cursor.position_y += 1;
      break;
    case RIGHT:
      (*context).cursor.position_x -= 1;
      break;
    case LEFT:
      (*context).cursor.position_x += 1;
      break;
    case EXIT:
      exit(EXIT_SUCCESS);
      break;
    case INSERT:
      context->filename = (char*)(command.command_value); // TODO
      context->filename = (char*)(getLineHeadFromPositionY(context->text, context->cursor.position_y)->string); // TODO
      line* head = getLineHeadFromPositionY(context->text, context->cursor.position_y);
      context->filename = (char*)(head->string); // TODO
      uint byte;
      line* line = getLineAndByteFromPositionX(head, context->cursor.position_x, &byte);
      context->filename = (char*)&line->string[byte]; // TODO

      break;
    case NONE:
      break;
  }
}

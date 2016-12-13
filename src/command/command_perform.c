#include "../main.gen.h"
#include "command.gen.h"

void vailidate_cursor_position(context *context)
{

  if((*context).cursor.position_x < 1)
  {
    (*context).cursor.position_x = 1;
  }

  if((*context).cursor.position_y < 1)
  {
    (*context).cursor.position_y = 1;
  }

  while(!getTextFromPositionY((*context).text,(*context).cursor.position_y))
  {
    (*context).cursor.position_y -= 1;
  }

  unum max_x = getTextFromPositionY((*context).text,(*context).cursor.position_y)->position_count - 1;
  if((*context).cursor.position_x > max_x)
  {
    (*context).cursor.position_x = max_x;
  }

}


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
      {
        uint byte;
        text* head = getTextFromPositionY(context->text, context->cursor.position_y);
        line* line = getLineAndByteFromPositionX(head->line, context->cursor.position_x, &byte);
        insert_mbchar(line, byte, command.command_value);
        (*context).cursor.position_x += 1;
      }
      break;
    case NONE:
      break;
  }
  vailidate_cursor_position(context);
}
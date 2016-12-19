#include "../main.gen.h"
#include "command.gen.h"

void vailidate_cursor_position(context *context) {

  if ((*context).cursor.position_x < 1) {
    (*context).cursor.position_x = 1;
  }

  if ((*context).cursor.position_y < 1) {
    (*context).cursor.position_y = 1;
  }

  while (!getTextFromPositionY((*context).lines, (*context).cursor.position_y)) {
    (*context).cursor.position_y -= 1;
  }

  unum max_x = getTextFromPositionY((*context).lines, (*context).cursor.position_y)->position_count - 1;
  if ((*context).cursor.position_x > max_x) {
    (*context).cursor.position_x = max_x;
  }
}

void command_perform(command command, context *context) // PUBLIC;
{
  calculatotion_width((*context).lines, (*context).view_size.width);
  switch (command.command_key) {
  case UP:
    (*context).cursor.position_y -= 1;
    break;
  case DOWN:
    (*context).cursor.position_y += 1;
    break;
  case RIGHT:
    (*context).cursor.position_x += 1;
    break;
  case LEFT:
    (*context).cursor.position_x -= 1;
    break;
  case EXIT:
    exit(EXIT_SUCCESS);
    break;
  case INSERT: {
    uint byte;
    lines *head = getTextFromPositionY(context->lines, context->cursor.position_y);
    mutable_string *mutable_string = getLineAndByteFromPositionX(head->mutable_string, context->cursor.position_x, &byte);
    insert_multibyte_char(mutable_string, byte, command.command_value);
    (*context).cursor.position_x += 1;
  } break;
  case DELETE: {
    if (context->cursor.position_x > 1) {
      uint byte;
      lines *head = getTextFromPositionY(context->lines, context->cursor.position_y);
      mutable_string *mutable_string = getLineAndByteFromPositionX(head->mutable_string, context->cursor.position_x - 1, &byte);
      delete_multibyte_char(mutable_string, byte);
      (*context).cursor.position_x -= 1;
    } else if (context->cursor.position_y > 1) {
      lines *head = getTextFromPositionY(context->lines, context->cursor.position_y - 1);
      lines_combine_next(head);
      (*context).cursor.position_x = getTextFromPositionY((*context).lines, (*context).cursor.position_y - 1)->position_count;
      (*context).cursor.position_y -= 1;
    }
  } break;
  case ENTER: {
    uint byte;
    lines *head = getTextFromPositionY(context->lines, context->cursor.position_y);
    mutable_string *mutable_string = getLineAndByteFromPositionX(head->mutable_string, context->cursor.position_x, &byte);
    lines_divide(head, mutable_string, byte, command.command_value);
    (*context).cursor.position_x = 1;
    (*context).cursor.position_y += 1;
  } break;
  case SAVE_OVERRIDE:
    context_write_override_file(context);
    break;
  case NONE:
    break;
  }
  calculatotion_width((*context).lines, (*context).view_size.width);
  vailidate_cursor_position(context);
}

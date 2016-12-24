#include "../main.gen.h"
#include "command.gen.h"

void validate_cursor_position(context *context) {

  if ((*context).cursor.position_x < 1) {
    (*context).cursor.position_x = 1;
  }

  if ((*context).cursor.position_y < 1) {
    (*context).cursor.position_y = 1;
  }

  while (!lines_select_position_y((*context).lines, (*context).cursor.position_y+1)) {
    (*context).cursor.position_y -= 1;
  }

  unum max_x = lines_select_position_y((*context).lines, (*context).cursor.position_y)->position_count - 1;
  if ((*context).cursor.position_x > max_x) {
    (*context).cursor.position_x = max_x;
  }
}

void command_perform(command command, context *context) // PUBLIC;
{
  lines_calculate_width((*context).lines);
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
    lines *head = lines_select_position_y(context->lines, context->cursor.position_y);
    mutable_string *mutable_string = mutable_string_select_position_x(head->mutable_string, context->cursor.position_x, &byte);
    insert_utf8char(mutable_string, byte, command.command_value);
    (*context).cursor.position_x += 1;
  } break;
  case DELETE: {
    if (context->cursor.position_x > 1) {
      uint byte;
      lines *head = lines_select_position_y(context->lines, context->cursor.position_y);
      mutable_string *mutable_string = mutable_string_select_position_x(head->mutable_string, context->cursor.position_x - 1, &byte);
      delete_utf8char(mutable_string, byte);
      (*context).cursor.position_x -= 1;
    } else if (context->cursor.position_y > 1) {
      lines *head = lines_select_position_y(context->lines, context->cursor.position_y - 1);
      lines_combine_next(head);
      (*context).cursor.position_x = lines_select_position_y((*context).lines, (*context).cursor.position_y - 1)->position_count;
      (*context).cursor.position_y -= 1;
    }
  } break;
  case ENTER: {
    uint byte;
    lines *head = lines_select_position_y(context->lines, context->cursor.position_y);
    mutable_string *mutable_string = mutable_string_select_position_x(head->mutable_string, context->cursor.position_x, &byte);
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
  lines_calculate_width((*context).lines);
  validate_cursor_position(context);
}

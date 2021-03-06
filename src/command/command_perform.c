#include "../console/screen.gen.h"
#include "../main.gen.h"           //PUBLIC
#include "../render/context.gen.h" //PUBLIC
#include "command.gen.h"

void validate_cursor_position(context *context, unum *x, unum *y) {
  if (*x < 1) {
    *x = 1;
  }
  if (*y < 1) {
    *y = 1;
  }
  while (!lines_select_position_y((*context).lines, *y)) {
    *y -= 1;
  }
  unum max_x = lines_select_position_y((*context).lines, *y)->position_count - 1;
  if (*x > max_x + 1) {
    *x = max_x + 1;
  }
}

void validate_cursor(context *context) {
  validate_cursor_position(context, &(*context).cursor.start_position_x, &(*context).cursor.start_position_y);
  validate_cursor_position(context, &(*context).cursor.end_position_x, &(*context).cursor.end_position_y);
}

void cursor_set_one_width(context *context) {
  (*context).cursor.end_position_x = (*context).cursor.start_position_x;
  (*context).cursor.end_position_y = (*context).cursor.start_position_y;
}

void command_perform(command command, context *context) // PUBLIC;
{
  lines_calculate_width((*context).lines);
  switch (command.command_key) {
  case UP:
    (*context).cursor.start_position_y -= 1;
    cursor_set_one_width(context);
    break;
  case DOWN:
    (*context).cursor.start_position_y += 1;
    cursor_set_one_width(context);
    break;
  case RIGHT:
    (*context).cursor.start_position_x += 1;
    cursor_set_one_width(context);
    break;
  case LEFT:
    (*context).cursor.start_position_x -= 1;
    cursor_set_one_width(context);
    break;
  // case SELECT_UP:
  // case SELECT_DOWN:
  case SELECT_RIGHT:
    (*context).cursor.end_position_x += 1;
    break;
  case SELECT_LEFT:
    (*context).cursor.end_position_x -= 1;
    break;
  case EXIT:
    console_clear();
    exit(EXIT_SUCCESS);
    break;
  case INSERT: {
    if (cursor_is_range(context->cursor)) {
      cursor_delete_range(context);
    }
    cursor_set_one_width(context);
    uint byte;
    lines *head = lines_select_position_y(context->lines, context->cursor.start_position_y);
    mutable_string *mutable_string =
        mutable_string_select_position_x(head->mutable_string, context->cursor.start_position_x, &byte);
    insert_utf8char(mutable_string, byte, command.command_value);
    (*context).cursor.start_position_x += 1;
    cursor_set_one_width(context);
  } break;
  case CUT:
    if (cursor_is_range(context->cursor)) {
      cursor_copy_range(context);
      cursor_delete_range(context);
    }
    cursor_set_one_width(context);
    break;
  case COPY:
    cursor_copy_range(context);
    break;
  case PASTE:
    if (cursor_is_range(context->cursor)) {
      cursor_delete_range(context);
    }
    cursor_set_one_width(context);
    cursor_paste_range(context);
    break;
  case DELETE: {
    if (cursor_is_range(context->cursor)) {
      cursor_delete_range(context);
    } else {
      cursor_delete_one(context);
    }
    cursor_set_one_width(context);
  } break;
  case ENTER: {
    if (cursor_is_range(context->cursor)) {
      cursor_delete_range(context);
    }
    cursor_set_one_width(context);
    uint byte;
    lines *head = lines_select_position_y(context->lines, context->cursor.start_position_y);
    mutable_string *mutable_string =
        mutable_string_select_position_x(head->mutable_string, context->cursor.start_position_x, &byte);
    lines_divide(head, mutable_string, byte, command.command_value);
    (*context).cursor.start_position_x = 1;
    (*context).cursor.start_position_y += 1;
    cursor_set_one_width(context);
  } break;
  case SAVE_OVERRIDE:
    context_write_override_file(context);
    break;
  case NONE:
    break;
  }
  lines_calculate_width((*context).lines);
  validate_cursor(context);
}

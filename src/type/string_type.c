#include "../main.gen.h"

/*EXPORT

typedef struct _mutable_string {
  uint byte_count;
  uint position_count;
  uchar string[BUFFER_SIZE];
  struct _mutable_string *next;
} mutable_string;

typedef struct _lines {
  unum width_count;
  unum position_count;
  mutable_string* mutable_string;
  struct _lines *prev;
  struct _lines *next;
} lines;

*/

mutable_string *mutable_string_insert(mutable_string *current) // PUBLIC;
{
  mutable_string *i = malloc(sizeof(mutable_string));
  i->next = current->next;
  current->next = i;
  i->byte_count = 0;
  return i;
}

void mutable_string_add_char(mutable_string *head, multibyte_char c) // PUBLIC;
{
  mutable_string *current = head;
  while (current->byte_count >= BUFFER_SIZE - safed_multibyte_char_size(c)) {
    if (current->next) {
      current = current->next;
    } else {
      current = mutable_string_insert(current);
    }
  }
  uint offset = 0;
  while (offset < safed_multibyte_char_size(c)) {
    current->string[current->byte_count] = c[offset];
    current->byte_count++;
    offset++;
  }
}

lines *lines_insert(lines *current) // PUBLIC;
{
  lines *i = malloc(sizeof(lines));
  i->prev = current;
  i->next = NULL;
  if (current) {
    if (current->next) {
      current->next->prev = i;
      i->next = current->next;
    }
    current->next = i;
  }
  i->mutable_string = malloc(sizeof(mutable_string));
  i->mutable_string->next = NULL;
  i->mutable_string->byte_count = 0;
  return i;
}

lines *lines_malloc(void) // PUBLIC;
{
  lines *head = malloc(sizeof(lines));
  head->prev = NULL;
  head->next = NULL;
  head->mutable_string = malloc(sizeof(mutable_string));
  head->mutable_string->next = NULL;
  head->mutable_string->byte_count = 0;
  return head;
}

void lines_free(lines *t) // PUBLIC;
{
  lines *p = t->prev;
  lines *n = t->next;
  p->next = n;
  n->prev = p;
  free(t);
}

multibyte_char get_tail(mutable_string *mutable_string);
void lines_combine_next(lines *current) // PUBLIC;
{
  mutable_string *tail = current->mutable_string;
  while (tail->next) {
    tail = tail->next;
  }
  delete_multibyte_char(tail, tail->byte_count - safed_multibyte_char_size(get_tail(tail)));
  tail->next = current->next->mutable_string;
  lines_free(current->next);
}

void lines_divide(lines *current_lines, mutable_string *current, uint byte, multibyte_char divide_char) // PUBLIC;
{
  lines_insert(current_lines);
  if (current->next) {
    free(current_lines->next->mutable_string);
    current_lines->next->mutable_string = current->next;
    current->next = NULL;
  }
  while (current->byte_count > byte) {
    multibyte_char tail = get_tail(current);
    current->byte_count -= safed_multibyte_char_size(tail);
    insert_multibyte_char(current_lines->next->mutable_string, 0, tail);
  }
  mutable_string_add_char(current, divide_char);
}

lines *getTextFromPositionY(lines *head, unum position_y) // PUBLIC;
{
  unum i = position_y - 1;
  lines *current_lines = head;
  while (i-- > 0 && current_lines) {
    current_lines = current_lines->next;
  }
  if (current_lines)
    return current_lines;
  return NULL;
}

mutable_string *getLineAndByteFromPositionX(mutable_string *head, unum position_x, uint *byte) // PUBLIC;
{
  unum i = position_x;
  mutable_string *current_mutable_string = head;
  while (current_mutable_string && i > current_mutable_string->position_count) {
    i -= current_mutable_string->position_count;
    current_mutable_string = current_mutable_string->next;
  }
  if (current_mutable_string) {
    *byte = 0;
    while (i-- > 1) // insert before cursor
    {
      *byte += safed_multibyte_char_size(&current_mutable_string->string[*byte]);
    }
    return current_mutable_string;
  }
  return NULL;
}

multibyte_char get_tail(mutable_string *mutable_string) {
  uint i = 0;
  while (mutable_string->byte_count > i + safed_multibyte_char_size(&mutable_string->string[i])) {
    i += safed_multibyte_char_size(&mutable_string->string[i]);
  }
  return &mutable_string->string[i];
}

void insert_multibyte_char(mutable_string *mutable_string, uint byte, multibyte_char c) // PUBLIC;
{
  uint s = safed_multibyte_char_size(c);
  if (mutable_string->byte_count + UTF8_MAX_BYTE >= BUFFER_SIZE) {
    mutable_string_insert(mutable_string);
  }
  while (BUFFER_SIZE <= mutable_string->byte_count + s) {
    multibyte_char tail = get_tail(mutable_string);
    mutable_string->byte_count -= safed_multibyte_char_size(tail);
    insert_multibyte_char(mutable_string->next, 0, tail);
  }
  uint move = mutable_string->byte_count;
  while (move > byte) {
    move--;
    mutable_string->string[move + s] = mutable_string->string[move];
  }

  uint offset = 0;
  while (offset < s) {
    mutable_string->string[byte + offset] = c[offset];
    mutable_string->byte_count++;
    offset++;
  }
}

void delete_multibyte_char(mutable_string *mutable_string, uint byte) // PUBLIC;
{
  uint s = safed_multibyte_char_size(&mutable_string->string[byte]);
  uint move = byte;
  while (move < mutable_string->byte_count) {
    mutable_string->string[move] = mutable_string->string[move + s];
    move++;
  }
  mutable_string->byte_count -= s;
}

void calculatotion_width(lines *head, uint max_width) // PUBLIC;
{
  static uint prev_width = 0;
  // if(prev_width == max_width) return;//cache hit TODO
  prev_width = max_width;
  lines *current_lines = head;
  mutable_string *current_mutable_string = head->mutable_string;

  uint i;
  while (current_lines) {
    unum total_width = 0;
    unum total_position = 0;
    current_mutable_string = current_lines->mutable_string;
    while (current_mutable_string) {
      int mutable_string_postition = 0;
      i = 0;
      while (i < current_mutable_string->byte_count) {
        int w = multibyte_char_width(&current_mutable_string->string[i]);
        total_width += w;
        mutable_string_postition++;
        i += safed_multibyte_char_size(&current_mutable_string->string[i]);
      }
      current_mutable_string->position_count = mutable_string_postition;
      total_position += mutable_string_postition;
      current_mutable_string = current_mutable_string->next;
    }
    current_lines->width_count = total_width;
    current_lines->position_count = total_position;
    current_lines = current_lines->next;
  }
}

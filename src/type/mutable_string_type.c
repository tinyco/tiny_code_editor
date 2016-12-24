#include "../main.gen.h"

/*EXPORT

#include "./unsigned_type.gen.h"

typedef struct _mutable_string {
  uint byte_count;
  uint position_count;
  uchar string[BUFFER_SIZE];
  struct _mutable_string *next;
} mutable_string;

*/

mutable_string *mutable_string_malloc() // PUBLIC;
{
  mutable_string *i = malloc(sizeof(mutable_string));
  i->next = NULL;
  i->byte_count = 0;
  return i;
}

int mutable_string_free_tail(mutable_string *i) {
  if (!i) {
    return 0;
  }
  if (!i->next) {
    free(i);
    return 0;
  }

  while (i->next->next) {
    i = i->next;
  }
  free(i->next);
  i->next = NULL;
  return 1;
}

void mutable_string_free(mutable_string *i) // PUBLIC;
{
  if (!i) {
    return;
  }
  while (mutable_string_free_tail(i))
    ;
}

mutable_string *mutable_string_insert(mutable_string *current) // PUBLIC;
{
  mutable_string *i = malloc(sizeof(mutable_string));
  i->next = NULL;
  if (current) {
    i->next = current->next;
    current->next = i;
  }
  i->byte_count = 0;
  return i;
}

void mutable_string_add_utf8char_to_tail(mutable_string *head, utf8char c) // PUBLIC;
{
  if (!head) {
    return;
  }
  while (head->next) {
    head = head->next;
  }
  mutable_string *current = head;
  while (current->byte_count >= BUFFER_SIZE - safed_utf8char_size(c)) {
    if (current->next) {
      current = current->next;
    } else {
      current = mutable_string_insert(current);
    }
  }
  uint offset = 0;
  while (offset < safed_utf8char_size(c)) {
    current->string[current->byte_count] = c[offset];
    current->byte_count++;
    offset++;
  }
}

mutable_string *mutable_string_select_position_x(mutable_string *head, unum position_x, uint *byte) // PUBLIC;
{
  unum i = position_x;
  mutable_string *current = head;
  while (current && i > current->position_count) {
    i -= current->position_count;
    current = current->next;
  }
  if (current) {
    *byte = 0;
    while (i-- > 1) // insert before cursor
    {
      *byte += safed_utf8char_size(&current->string[*byte]);
    }
    return current;
  }
  *byte = 0;
  return NULL;
}

utf8char mutable_string_get_tail(mutable_string *mutable_string) // PUBLIC;
{
  uint i = 0;
  while (mutable_string->byte_count > i + safed_utf8char_size(&mutable_string->string[i])) {
    i += safed_utf8char_size(&mutable_string->string[i]);
  }
  return &mutable_string->string[i];
}

void insert_utf8char(mutable_string *mutable_string, uint byte, utf8char c) // PUBLIC;
{
  uint s = safed_utf8char_size(c);
  if (mutable_string->byte_count + UTF8_MAX_BYTE >= BUFFER_SIZE) {
    mutable_string_insert(mutable_string);
  }
  while (BUFFER_SIZE <= mutable_string->byte_count + s) {
    utf8char tail = mutable_string_get_tail(mutable_string);
    mutable_string->byte_count -= safed_utf8char_size(tail);
    insert_utf8char(mutable_string->next, 0, tail);
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

void delete_utf8char(mutable_string *mutable_string, uint byte) // PUBLIC;
{
  uint s = safed_utf8char_size(&mutable_string->string[byte]);
  uint move = byte;
  while (move < mutable_string->byte_count) {
    mutable_string->string[move] = mutable_string->string[move + s];
    move++;
  }
  mutable_string->byte_count -= s;
}

void mutable_string_calculate_width(mutable_string *ms, unum *width_count, unum *position_count) // PUBLIC;
{
  unum total_width = 0;
  unum total_position = 0;

  while (ms) {
    int mutable_string_postition = 0;
    uint i = 0;
    while (i < ms->byte_count) {
      int w = utf8char_width(&ms->string[i]);
      total_width += w;
      mutable_string_postition++;
      i += safed_utf8char_size(&ms->string[i]);
    }
    ms->position_count = mutable_string_postition;
    total_position += mutable_string_postition;
    ms = ms->next;
  }
  if (width_count) {
    *width_count = total_width;
  }
  if (position_count) {
    *position_count = total_position;
  }
}

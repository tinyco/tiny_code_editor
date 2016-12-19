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

multibyte_char get_tail(mutable_string *mutable_string) // PUBLIC;
{
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

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
  mutable_string *ms = malloc(sizeof(mutable_string));
  ms->next = NULL;
  ms->byte_count = 0;
  return ms;
}

int mutable_string_free_tail(mutable_string *ms) {
  if (!ms) {
    return 0;
  }
  if (!ms->next) {
    free(ms);
    return 0;
  }

  while (ms->next->next) {
    ms = ms->next;
  }
  free(ms->next);
  ms->next = NULL;
  return 1;
}

void mutable_string_all_free(mutable_string *head) // PUBLIC;
{
  if (!head) {
    return;
  }
  while (mutable_string_free_tail(head)) {
    ; // only loop
  }
}

void mutable_string_free(mutable_string *ms) // PUBLIC;
{
  free(ms);
}
mutable_string *mutable_string_insert(mutable_string *target) // PUBLIC;
{
  mutable_string *ms = malloc(sizeof(mutable_string));
  ms->next = NULL;
  if (target) {
    ms->next = target->next;
    target->next = ms;
  }
  ms->byte_count = 0;
  return ms;
}

void mutable_string_add_utf8char_to_tail(mutable_string *head, utf8char c) // PUBLIC;
{
  if (!head) {
    return;
  }
  while (head->next) {
    head = head->next;
  }
  mutable_string *ms = head;
  while (ms->byte_count >= BUFFER_SIZE - safed_utf8char_size(c)) {
    if (ms->next) {
      ms = ms->next;
    } else {
      ms = mutable_string_insert(ms);
    }
  }
  uint offset = 0;
  while (offset < safed_utf8char_size(c)) {
    ms->string[ms->byte_count] = c[offset];
    ms->byte_count++;
    offset++;
  }
}

mutable_string *mutable_string_select_position_x(mutable_string *head, unum position_x, uint *byte) // PUBLIC;
{
  unum x = position_x;
  mutable_string *ms = head;
  while (ms && x > ms->position_count) {
    x -= ms->position_count;
    ms = ms->next;
  }
  if (ms) {
    *byte = 0;
    while (x-- > 1) // insert before cursor
    {
      *byte += safed_utf8char_size(&ms->string[*byte]);
    }
    return ms;
  }
  *byte = 0;
  return NULL;
}

utf8char mutable_string_get_tail(mutable_string *ms) // PUBLIC;
{
  uint b = 0;
  while (ms->byte_count > b + safed_utf8char_size(&ms->string[b])) {
    b += safed_utf8char_size(&ms->string[b]);
  }
  return &ms->string[b];
}

void insert_utf8char(mutable_string *ms, uint byte, utf8char c) // PUBLIC;
{
  uint s = safed_utf8char_size(c);
  if (ms->byte_count + UTF8_MAX_BYTE >= BUFFER_SIZE) {
    mutable_string_insert(ms);
  }
  while (BUFFER_SIZE <= ms->byte_count + s) {
    utf8char tail = mutable_string_get_tail(ms);
    ms->byte_count -= safed_utf8char_size(tail);
    insert_utf8char(ms->next, 0, tail);
  }
  uint move = ms->byte_count;
  while (move > byte) {
    move--;
    ms->string[move + s] = ms->string[move];
  }

  uint offset = 0;
  while (offset < s) {
    ms->string[byte + offset] = c[offset];
    ms->byte_count++;
    offset++;
  }
}

void delete_utf8char(mutable_string *ms, uint byte) // PUBLIC;
{
  uint s = safed_utf8char_size(&ms->string[byte]);
  uint move = byte;
  while (move < ms->byte_count) {
    ms->string[move] = ms->string[move + s];
    move++;
  }
  ms->byte_count -= s;
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

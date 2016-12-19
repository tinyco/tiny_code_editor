#include "../main.gen.h"

#define UTF8_MAX_BYTE 6 // PUBLIC

#define UTF8_NULL 0      // PUBLIC
#define UTF8_NOT_FILL -1 // PUBLIC
#define UTF8_ILLIEGAL -2 // PUBLIC

typedef uchar *utf8char; // PUBLIC

utf8char utf8char_malloc(void) // PUBLIC;
{
  return malloc(sizeof(uchar) * UTF8_MAX_BYTE);
}

void utf8char_free(utf8char utf8char) // PUBLIC;
{
  free(utf8char);
  utf8char = NULL;
}

utf8char mbcher_zero_clear(utf8char utf8char) // PUBLIC;
{
  uint i = UTF8_MAX_BYTE;
  while (i--) {
    utf8char[i] = '\0';
  }
  return utf8char;
}

int utf8char_size(utf8char utf8char, uint byte) // PUBLIC;
{
  if (byte < 1 || UTF8_MAX_BYTE < byte)
    return UTF8_ILLIEGAL;

  if (byte == 1 && utf8char[0] == 0x00)
    return UTF8_NULL;

  uint head_one_bits = 0;
  while (head_one_bits < 8) {
    if ((utf8char[0] >> (7 - head_one_bits) & 0x01) == 1) {
      head_one_bits++;
    } else {
      break;
    }
  }
  if (head_one_bits == 0)
    head_one_bits++;

  if (head_one_bits > byte)
    return UTF8_NOT_FILL;

  if (head_one_bits == byte) {
    switch (byte) {
    case 1:
      return byte;
      break;
    case 2:
      if (utf8char[0] & 0x1e)
        return byte;
      break;
    case 3:
      if (utf8char[0] & 0x0f || utf8char[1] & 0x20)
        return byte;
      break;
    case 4:
      if (utf8char[0] & 0x07 || utf8char[1] & 0x30)
        return byte;
      break;
    case 5:
      if (utf8char[0] & 0x03 || utf8char[1] & 0x38)
        return byte;
      break;
    case 6:
      if (utf8char[0] & 0x01 || utf8char[1] & 0x3c)
        return byte;
      break;
    }
  }

  return UTF8_ILLIEGAL;
}

uint safed_utf8char_size(utf8char utf8char) // PUBLIC;
{
  uint head_one_bits = 0;
  while (head_one_bits < 8) {
    if ((utf8char[0] >> (7 - head_one_bits) & 0x01) == 1) {
      head_one_bits++;
    } else {
      break;
    }
  }
  if (head_one_bits == 0)
    head_one_bits++;
  return head_one_bits;
}

int is_mutable_string_break(utf8char utf8char) // PUBLIC;
{
  return (utf8char[0] == '\n' ? 1 : 0);
}

uint utf8char_width(utf8char utf8char) // PUBLIC;
{
  if (safed_utf8char_size(utf8char) > 1) {
    return 2;
  } else {
    return 1;
  }
}

unum string_width(uchar *message) // PUBLIC;
{
  uint width = 0;
  long max_byte = strlen((char *)message);
  uint i = 0;
  while (i < max_byte) {
    width += utf8char_width(&message[i]);
    i += safed_utf8char_size(&message[i]);
  }
  return width;
}

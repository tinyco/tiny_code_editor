#include "../main.gen.h"

#define UTF8_MAX_BYTE 6 // PUBLIC

#define MULTIBYTE_CHAR_NULL 0      // PUBLIC
#define MULTIBYTE_CHAR_NOT_FILL -1 // PUBLIC
#define MULTIBYTE_CHAR_ILLIEGAL -2 // PUBLIC

typedef uchar *multibyte_char; // PUBLIC

multibyte_char multibyte_char_malloc(void) // PUBLIC;
{
  return malloc(sizeof(uchar) * UTF8_MAX_BYTE);
}

void multibyte_char_free(multibyte_char multibyte_char) // PUBLIC;
{
  free(multibyte_char);
  multibyte_char = NULL;
}

multibyte_char mbcher_zero_clear(multibyte_char multibyte_char) // PUBLIC;
{
  uint i = UTF8_MAX_BYTE;
  while (i--) {
    multibyte_char[i] = '\0';
  }
  return multibyte_char;
}

int multibyte_char_size(multibyte_char multibyte_char, uint byte) // PUBLIC;
{
  if (byte < 1 || UTF8_MAX_BYTE < byte)
    return MULTIBYTE_CHAR_ILLIEGAL;

  if (byte == 1 && multibyte_char[0] == 0x00)
    return MULTIBYTE_CHAR_NULL;

  uint head_one_bits = 0;
  while (head_one_bits < 8) {
    if ((multibyte_char[0] >> (7 - head_one_bits) & 0x01) == 1) {
      head_one_bits++;
    } else {
      break;
    }
  }
  if (head_one_bits == 0)
    head_one_bits++;

  if (head_one_bits > byte)
    return MULTIBYTE_CHAR_NOT_FILL;

  if (head_one_bits == byte) {
    switch (byte) {
    case 1:
      return byte;
      break;
    case 2:
      if (multibyte_char[0] & 0x1e)
        return byte;
      break;
    case 3:
      if (multibyte_char[0] & 0x0f || multibyte_char[1] & 0x20)
        return byte;
      break;
    case 4:
      if (multibyte_char[0] & 0x07 || multibyte_char[1] & 0x30)
        return byte;
      break;
    case 5:
      if (multibyte_char[0] & 0x03 || multibyte_char[1] & 0x38)
        return byte;
      break;
    case 6:
      if (multibyte_char[0] & 0x01 || multibyte_char[1] & 0x3c)
        return byte;
      break;
    }
  }

  return MULTIBYTE_CHAR_ILLIEGAL;
}

uint safed_multibyte_char_size(multibyte_char multibyte_char) // PUBLIC;
{
  uint head_one_bits = 0;
  while (head_one_bits < 8) {
    if ((multibyte_char[0] >> (7 - head_one_bits) & 0x01) == 1) {
      head_one_bits++;
    } else {
      break;
    }
  }
  if (head_one_bits == 0)
    head_one_bits++;
  return head_one_bits;
}

int is_line_break(multibyte_char multibyte_char) // PUBLIC;
{
  return (multibyte_char[0] == '\n' ? 1 : 0);
}

uint multibyte_char_width(multibyte_char multibyte_char) // PUBLIC;
{
  if (safed_multibyte_char_size(multibyte_char) > 1) {
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
    width += multibyte_char_width(&message[i]);
    i += safed_multibyte_char_size(&message[i]);
  }
  return width;
}

#include "../main.gen.h"

#define UTF8_MAX_BYTE 6 // PUBLIC

#define MBCHAR_NULL 0      // PUBLIC
#define MBCHAR_NOT_FILL -1 // PUBLIC
#define MBCHAR_ILLIEGAL -2 // PUBLIC

typedef uchar *mbchar; // PUBLIC

mbchar mbchar_malloc(void) // PUBLIC;
{
  return malloc(sizeof(uchar) * UTF8_MAX_BYTE);
}

void mbchar_free(mbchar mbchar) // PUBLIC;
{
  free(mbchar);
  mbchar = NULL;
}

mbchar mbcher_zero_clear(mbchar mbchar) // PUBLIC;
{
  uint i = UTF8_MAX_BYTE;
  while (i--) {
    mbchar[i] = '\0';
  }
  return mbchar;
}

int mbchar_size(mbchar mbchar, uint byte) // PUBLIC;
{
  if (byte < 1 || UTF8_MAX_BYTE < byte)
    return MBCHAR_ILLIEGAL;

  if (byte == 1 && mbchar[0] == 0x00)
    return MBCHAR_NULL;

  uint head_one_bits = 0;
  while (head_one_bits < 8) {
    if ((mbchar[0] >> (7 - head_one_bits) & 0x01) == 1) {
      head_one_bits++;
    } else {
      break;
    }
  }
  if (head_one_bits == 0)
    head_one_bits++;

  if (head_one_bits > byte)
    return MBCHAR_NOT_FILL;

  if (head_one_bits == byte) {
    switch (byte) {
    case 1:
      return byte;
      break;
    case 2:
      if (mbchar[0] & 0x1e)
        return byte;
      break;
    case 3:
      if (mbchar[0] & 0x0f || mbchar[1] & 0x20)
        return byte;
      break;
    case 4:
      if (mbchar[0] & 0x07 || mbchar[1] & 0x30)
        return byte;
      break;
    case 5:
      if (mbchar[0] & 0x03 || mbchar[1] & 0x38)
        return byte;
      break;
    case 6:
      if (mbchar[0] & 0x01 || mbchar[1] & 0x3c)
        return byte;
      break;
    }
  }

  return MBCHAR_ILLIEGAL;
}

uint safed_mbchar_size(mbchar mbchar) // PUBLIC;
{
  uint head_one_bits = 0;
  while (head_one_bits < 8) {
    if ((mbchar[0] >> (7 - head_one_bits) & 0x01) == 1) {
      head_one_bits++;
    } else {
      break;
    }
  }
  if (head_one_bits == 0)
    head_one_bits++;
  return head_one_bits;
}

int is_line_break(mbchar mbchar) // PUBLIC;
{
  return (mbchar[0] == '\n' ? 1 : 0);
}

uint mbchar_width(mbchar mbchar) // PUBLIC;
{
  if (safed_mbchar_size(mbchar) > 1) {
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
    width += mbchar_width(&message[i]);
    i += safed_mbchar_size(&message[i]);
  }
  return width;
}

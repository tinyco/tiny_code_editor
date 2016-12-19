//This file is auto-generated from ./type/multibyte_char_type.c
#ifndef __TYPE_MULTIBYTE_CHAR_TYPE_C
#define __TYPE_MULTIBYTE_CHAR_TYPE_C
//EXPORT
//PUBLIC
#define UTF8_MAX_BYTE 6  
#define MULTIBYTE_CHAR_NULL 0       
#define MULTIBYTE_CHAR_NOT_FILL -1  
#define MULTIBYTE_CHAR_ILLIEGAL -2  
typedef uchar *multibyte_char;  
multibyte_char multibyte_char_malloc(void)  ;
void multibyte_char_free(multibyte_char multibyte_char)  ;
multibyte_char mbcher_zero_clear(multibyte_char multibyte_char)  ;
int multibyte_char_size(multibyte_char multibyte_char, uint byte)  ;
uint safed_multibyte_char_size(multibyte_char multibyte_char)  ;
int is_line_break(multibyte_char multibyte_char)  ;
uint multibyte_char_width(multibyte_char multibyte_char)  ;
unum string_width(uchar *message)  ;
#endif

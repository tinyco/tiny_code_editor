//This file is auto-generated from ./type/utf8char_type.c
#ifndef __TYPE_UTF8CHAR_TYPE_C
#define __TYPE_UTF8CHAR_TYPE_C
//EXPORT
//PUBLIC
#include "../main.gen.h" 
#define UTF8_MAX_BYTE 6  
#define UTF8_NULL 0       
#define UTF8_NOT_FILL -1  
#define UTF8_ILLIEGAL -2  
typedef uchar *utf8char;  
utf8char utf8char_malloc(void)  ;
void utf8char_free(utf8char uc)  ;
utf8char utf8char_zero_clear(utf8char uc)  ;
int utf8char_size(utf8char uc, uint byte)  ;
uint safed_utf8char_size(utf8char uc)  ;
int is_break(utf8char uc)  ;
uint utf8char_width(utf8char uc)  ;
unum string_width(uchar *str)  ;
#endif

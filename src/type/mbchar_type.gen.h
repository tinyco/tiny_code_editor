//This file is auto-generated from ./type/mbchar_type.c
#ifndef __TYPE_MBCHAR_TYPE_C
#define __TYPE_MBCHAR_TYPE_C
//EXPORT
//PUBLIC
#define MBCHAR_NULL 0 
#define MBCHAR_NOT_FILL -1 
#define MBCHAR_ILLIEGAL -2 
typedef uchar* mbchar;
mbchar mbchar_malloc(void);
void mbchar_free(mbchar mbchar);
mbchar mbcher_zero_clear(mbchar mbchar);
int mbchar_size(mbchar mbchar, int byte);
int safed_mbchar_size(mbchar mbchar);
int isLineBreak(mbchar mbchar);
int mbchar_width(mbchar mbchar) ;
int string_width(uchar* message) ;
#endif

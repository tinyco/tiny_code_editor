//This file is auto-generated from ./type/string_type.c
#ifndef __TYPE_STRING_TYPE_C
#define __TYPE_STRING_TYPE_C
//EXPORT

typedef struct _mutable_string {
  uint byte_count;
  uint position_count;
  uchar string[BUFFER_SIZE];
  struct _mutable_string *next;
} mutable_string;

typedef struct _text {
  unum width_count;
  unum position_count;
  mutable_string* mutable_string;
  struct _text *prev;
  struct _text *next;
} text;


//PUBLIC
mutable_string *mutable_string_insert(mutable_string *current)  ;
void mutable_string_add_char(mutable_string *head, multibyte_char c)  ;
text *text_insert(text *current)  ;
text *text_malloc(void)  ;
void text_free(text *t)  ;
void text_combine_next(text *current)  ;
void text_divide(text *current_text, mutable_string *current, uint byte, multibyte_char divide_char)  ;
text *getTextFromPositionY(text *head, unum position_y)  ;
mutable_string *getLineAndByteFromPositionX(mutable_string *head, unum position_x, uint *byte)  ;
void insert_multibyte_char(mutable_string *mutable_string, uint byte, multibyte_char c)  ;
void delete_multibyte_char(mutable_string *mutable_string, uint byte)  ;
void calculatotion_width(text *head, uint max_width)  ;
#endif

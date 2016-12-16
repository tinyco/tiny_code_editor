//This file is auto-generated from ./type/string_type.c
#ifndef __TYPE_STRING_TYPE_C
#define __TYPE_STRING_TYPE_C
//EXPORT

typedef struct _line {
	uint byte_count;
	uint position_count;
	uchar string[BUFFER_SIZE];
	struct _line *next;
} line;

typedef struct _text {
	unum width_count;
	unum position_count;
	line* line;
	struct _text *prev;
	struct _text *next;
} text;


//PUBLIC
line* line_insert(line* current);
void line_add_char(line* head, mbchar c);
text* text_insert(text* current);
text* text_malloc(void);
void text_free(text* t);
void text_combine_next(text* current);
void text_divide(text* current_text, line* current, uint byte, mbchar divide_char);
text* getTextFromPositionY(text* head, unum position_y);
line* getLineAndByteFromPositionX(line* head, unum position_x, uint* byte);
void insert_mbchar(line* line, uint byte, mbchar c);
void delete_mbchar(line* line, uint byte);
void calculatotion_width(text* head, uint max_width);
#endif

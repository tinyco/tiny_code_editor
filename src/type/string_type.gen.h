//This file is auto-generated from ./type/string_type.c
#ifndef __TYPE_STRING_TYPE_C
#define __TYPE_STRING_TYPE_C
//EXPORT

typedef struct _line {
	unum byte_count;
	uchar string[BUFFER_SIZE];
	struct _line *next;
} line;

typedef struct _text {
	unum height;
	line* line;
	struct _text *prev;
	struct _text *next;
} text;


//PUBLIC
line* line_insert(line* current);
void line_add_char(line* head, mbchar c);
text* text_insert(text* current);
text* text_malloc(void);
line* getLineHeadFromPositionY(text* head, unum position_y);
void calculatotion_height(text* head, uint max_width);
#endif

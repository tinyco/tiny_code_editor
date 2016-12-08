//This file is auto-generated from ./type/string_type.c
#ifndef __TYPE_STRING_TYPE_C
#define __TYPE_STRING_TYPE_C
//EXPORT

typedef struct _line {
	int byte_count;
	char string[BUFFER_SIZE];
	struct _line *next;
} line;

typedef struct _text {
	int height;
	line* line;
	struct _text *prev;
	struct _text *next;
} text;


//PUBLIC
line* line_insert(line* current);
void line_add_char(line* head, mbchar c);
text* text_insert(text* current);
text* text_malloc(void);
void calculatotion_height(text* head, int max_width);
#endif

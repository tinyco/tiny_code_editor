本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の7日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

このアドベントカレンダーは、コンソール上で動くエディタを25日間で1から自作することを目標としています。

# 文字列の持ち方を変える

```string_type.c
typedef struct _line {
	int byte_count;//実際にstringに入ってる文字のバイト数
	char string[BUFFER_SIZE];
	struct _line *next;
} line;

typedef struct _text {
	line* line;
	struct _text *prev;
	struct _text *next;
} text;
```

改行するまでをline(単方向リスト)として、そのlineを双方向リストにつないだものをtextとします。
意図として、カーソル移動するたびに改行位置を探すのは、後で実装するときに困りそうだという点と、
1行に最大でもxx文字しかこない、という閾値を見積れなかった点を考慮しました。
合わせて、ファイルからリードするところも変更します。

```mbchar.c
typedef char* mbchar;//PUBLIC

mbchar mbchar_malloc(void)//PUBLIC;
{
  return malloc(sizeof(char) * MB_CUR_MAX);
}

void mbchar_free(mbchar mbchar)//PUBLIC;
{
  return free(mbchar);
}

mbchar mbcher_zero_clear(mbchar mbchar)//PUBLIC;
{
  int i = MB_CUR_MAX;
  while(i--)
  {
    mbchar[i] = '\0';
  }
  return mbchar;
}

int mbchar_size(mbchar mbchar)//PUBLIC;
{
  return mblen(mbchar, MB_CUR_MAX);
}

int isLineBreak(mbchar mbchar)//PUBLIC;
{
  return (mbchar[0] == '\n' ? 1 : 0);
}
```

```file_read.c
text* file_read(char* filepath)//PUBLIC;
{
	FILE *fp;
	if((fp = fopen(filepath, "r")) == NULL)
	{
    printf("[error]can't open file\n");
    exit(EXIT_FAILURE);
	}

	text* head = text_malloc();

	text* current_text = head;
	line* current_line = head->line;

	mbchar buf = mbchar_malloc();
	char c;
	mbcher_zero_clear(buf);
	int len = 0;
	while(1)
	{
		c = (char)fgetc(fp);
		if (feof(fp))	break;
		buf[len] = c;
		if(mbchar_size(buf) > 0) {
			line_add_char(current_line, buf);
			if(isLineBreak(buf)) {
				current_text = text_insert(current_text);
				current_line = current_text->line;
			}
			mbcher_zero_clear(buf);
			len = 0;
		} else if (mbchar_size(buf) <= 0) {
			len++;
		}
	}

	mbchar_free(buf);
	fclose(fp);
	return head;
}
```

これで、マルチバイト文字がリストの途中で分断されないようにリストに分割される・・・はず。
と思ったのですが、分断されてしまいました。

# 原因を調べる

「私」の文字を対象にprintfデバッグをしていきます。
ネットで調べると、私はE7・A7・81の3バイト文字のようです。

MB_CUR_MAXを出力すると、どうやら1になってしまっているらしい。
`setlocale( LC_CTYPE， 'jpn' );`
を追加しても1のままです。

そのため、mblenで長さを正しく取ることができず、1バイトの長さと扱われて、
mbcharに格納されるときにE7・A7・81の3文字として扱われてしまっているようです。

ハマったところとしては
`printf("[%x]\n",mbchar[0]/*私*/);` -> [ffffffe7]
`printf("[%d]\n",mbchar[0]/*私*/);` -> [-25]
が出力されるため、頭の中で上手く`E7`と結びつけられず、余計なバイト(私の前に入ってる文字？)が
入っていると疑ってしまって時間がかかりました。

printfのときは0埋めされないんですね。ただ、
`printf("[%d]\n",mbchar[0] == (char)0xe7 ? 1 : 0);` -> [1] をみると、正しく`E7`が入っているとわかります。

# 今日のまとめ

- カーソル移動に備えて、文字列を2次元リストとして持つことにした
  - あわせてファイル読み込み部分も実装

- カーソル移動に備えて文字のバイト数を取得しようとしたが上手くいかなかった

明日は、[ここ](http://qiita.com/masakielastic/items/abf8e54344034f9b089c)を参考に、mblenを使わずに
「後続文字があるかどうかわかる」「後続文字がないときに、何バイトの文字かわかる」関数を自作してmblenと差し替えていきます。
それが済んだら、カーソル移動まで進めるようになるはず٩(๑òωó๑)۶


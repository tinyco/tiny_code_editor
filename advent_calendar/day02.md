本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の2日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

このアドベントカレンダーは、コンソール上で動くエディタを25日間で1から自作することを目標としています。

# ファイルを読む

調べたところ、以下のようにしてファイルを読むようです。要点としては

1. ファイルディスクリプタを用意する(File \*fpの部分)
1. ファイルを開く(fopen)
1. str[10]を超えないように指定したサイズ分ずつ読む(fgets)
1. ファイルディスクリプタを解放する(fclose(fp))

といった流れのようです。

ファイルディスクリプタはOSが用意する、同時に何個までファイルを開いてもいいかを管理する連番IDのようですね。
(Riak等をインストールするときに増やせって言われるやつですね)

```main.c
#include <stdio.h>

void file_print(char* filename) {
  FILE *fp;
  char str[10];
  if((fp = fopen(filename, "r")) == NULL) {
    printf("[error]can't open\n");
    return;
  }
  while(fgets(str, sizeof(str), fp)) {
    printf("%s", str);
  }
  fclose(fp);
}

int main(int argc, char *argv[]) {
  printf("\e[2J\e[1;1H");//clear
  if(argc != 2) {
    printf("[error]illegal args\n");
  } else {
    file_print(argv[1]);
  }
}
```

`gcc main.c; ./a.out main.c`とかで試してください

気づいたこととしては、以下です。

- filenameの部分は、絶対パスじゃなくて相対パスでもいいようです。
今回はコンソールのコマンドを目指しているので相対パスのほうが楽そうですね。


- C言語に可変長文字列はないのですが、固定長文字配列の長さをmalloc/freeを使ってランタイムに決めることはできるようです。
ただ、エディタを作りたいために配列だと都合が悪いですね(1文字打つごとに配列全体を作り直さないといけない)。
双方向リスト型がほしいですが、自作するしかなさそうです。今回は、その場でprintfしました。


- 将来的に新規ファイルを作ったり、複数のファイルを開いたりしたいですが、現時点ではargsを1個だけ指定することに決め打ちします。


- 昨日の投稿時点では気づいてなかったですが、argcは0始まりではなくて1始まりなんですね。

- 関数の戻り値がない関数(行先頭の戻り値のところがvoid)はreturnが不要だが、書いてもいいようです。関数の戻り値がint型のものはreturn(n)をつけないとwarningがでますが、main関数は書いてないのに怒られないですね。不思議です。いいのかな？

- [ここ](http://stackoverflow.com/questions/9549342/should-i-return-0-or-1-for-successful-function)によるとmain関数については、成功が0/失敗が1を返すのが通例のようです。それぞれ、EXIT_SUCCESS/EXIT_FAILUREっていうエイリアスがついていますが、エイリアスを使うにはstdlib.hをincludeする必要があるようです。

- よくわかってないですが、マルチバイト文字もいい感じに出力されます。str[10]ごとの区切りがマルチバイト文字を分断してしまっても、結果的に連続してprintfされていればいいのでしょうか？

ファイル内容が出力できると、一気に進んだ感がありますねヽ(•̀ω•́ )ゝ✧

# 双方向リストを作る

調べたところ双方向リストを作るには

1. 構造体をつくる
1. リストをつなげるときに、つなげる元の構造体につなげる先の構造体へのポインタを置いておく
1. 先頭、末尾についてはポインタをNULLにしてわかるようにしておく
1. その際に、挿入はmallocを、削除はfreeを忘れないようにしないとメモリリークする

という流れのようです。結構めんどくさいですね・・・。

ライブラリを探してみたのですが、検索ワードが悪いのか上手く見つけられませんでした。
構造体を上手くみんなが使いやすい形で定義するのが難しいからないのかも、ジェネリクスは偉大ですね。

つくってみました

```main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2

typedef struct _string {
  char str[BUFFER_SIZE];
  struct _string *prev;
  struct _string *next;
} string;

string* insert(string *from) {
  string* to = malloc(sizeof(string));
  if (from->next) {
    from->next->prev = to;
    to->next = from->next;
  } else {
    to->next = NULL;
  }
  if(from){
    from->next = to;
  }
  to->prev = from;
  return to;
}

void file_read(char* filename, string* head) {
  FILE *fp;
  char buf[BUFFER_SIZE];

  if((fp = fopen(filename, "r")) == NULL) {
    printf("[error]can't open\n");
    return;
  }
  string* current = head;
  while(fgets(buf, sizeof(buf), fp)) {
    strcpy(current->str, buf);
    insert(current);
    current = current->next;
  }
  fclose(fp);
}

int main(int argc, char *argv[]) {
  printf("\e[2J\e[1;1H");//clear
  string* head = malloc(sizeof(string));
  head->prev = NULL;
  head->next = NULL;
  if(argc != 2) {
    printf("[error]illegal args\n");
  } else {
    file_read(argv[1], head);
    string* current = head;

    // normal
    while(current->next) {
      printf("%s", current->str);
      current = current->next;
    }
    printf("%s", current->str);

    // reverse
    printf("\e[31m\e[47m");//color
    while(current->prev) {
      printf("%c", current->str[1]);
      printf("%c", current->str[0]);
      current = current->prev;
    }
    printf("%c", current->str[1]);
    printf("%c", current->str[0]);
    printf("\e[39m\e[49m");//reset

  }
  return EXIT_SUCCESS;
}
```

全部貼り付けるには、長くなってきました。そろそろファイル分けたりしないとですね。
出来るだけ細かく過程を貼りたかったのですが、けっこう差分が多くなってしまいました。。。

`gcc main.c; ./a.out main.c`とかで試してください。

abcdeっていう文字列があったら、最初に文字を順方向に表示して(abcde)、次に逆方向に文字を表示する(edcba)サンプルです。可変長の文字列を双方向リストを使って扱う練習です。

気づいたこととしては、以下です。

- stdlib.hでmallocを、string.hで文字列のコピー(strcpy)を使えるようにします。

- freeを忘れないようにしないと、と言った直後ですがfreeをしていないです。実行終了時にメモリの解放自体はされるっぽいですね。freeする必要があるのは、アプリケーションが長時間利用されるはず、という前提がありそうです。忘れないようにしたつもりでも忘れそうな感じがします。

- ポインタを作る(型に\*をつける) → mallocしたメモリ領域を代入しておく → 使うときは`dot`ではなく`->`でアクセスする がC言語でポインタを使うときの基本的な流れのようです。

- structで構造体を宣言できますが、その時にtypedefをつけて末尾に型名を書くと便利です。typedefをつけずに struct hoge {...};で宣言すると、使うときに毎回 struct hogeと書く必要がありますが、typedef struct \_hoge {...} hoge;で宣言すると hoge (又はstruct \_hoge)と書いて使えます。

- insertするときは、ポインタをちまちま繋ぎ変える必要があります。紙に書いて頭の中を整理すればなんとか書けましたが、この辺のコードを読み書きするのが大変ですね。

- 間違えてmallocした領域を代入していないポインタで`->`を使うと`Bus error: 10`がでます。


- 今回のものでマルチバイト文字のファイルを開くと、順方向へ表示するときは綺麗に表示されますが、逆方向にprintしていくときは文字化けします。マルチバイト文字の1文字を必ずしも同時にprintfする必要はないんですね(順方向)。当たり前ですがマルチバイト文字の1文字の中身の順番を入れ替えると文字化けします(逆方向)。試した範囲では、文字化けするだけでエラーで落ちたりはしなかったです。意外とマルチバイト文字対応できるんじゃないか？って気がしてきました(しれっと対象外にするつもりでした)。

# 今日のまとめ

- ファイルを読めるようになった
- 可変長の文字列を扱えるようになった

そろそろゲームマーケットですね。楽しみです。

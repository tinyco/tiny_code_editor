本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の3日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

このアドベントカレンダーは、コンソール上で動くエディタを25日間で1から自作することを目標としています。

> 前回までで、いったん`catコマンド`っぽいところまでできました。
> 今日は`lessコマンド`っぽいところを目指します。

# キーボードで操作したい

まずは調べるところから。どうやら、キーボードの入力を受け取るにはgetchar/getch/scanf/sscanf/gets/fgets...等、色々な方法がありそうです。

検索結果によると、gets/fgets/scanf/sscanfは入力を行で受け取るので、ユーザが[a,b,c,enter]と入力した場合に、エンターを押した瞬間にabcを取得する方針(`以下エンター待ちと呼ぶ、造語です`)のようで、エディタを作りたい目的に合致しなさそうです。1文字ずつ処理できるらしいgetcharかgetchを使うのが良さそうな気がします。

よくわからないまま、getcharとgetchを比べてみます。[ここ](http://www.geocities.co.jp/SiliconValley-Bay/7437/c/c7.htm)の使用例3のサンプルをコピペして動かしてみました。すると`'getch' is invalid in C99`というエラーが出たため、とりあえずコメントアウトしてgetcharだけ動かすと、エンター待ちになりました。こまった・・・。キーの入力があるかをしらべるkbhitも`'kbhit' is invalid in C99`になります。

getchは、なんでコンパイル通らないんだろうと思って無意味にincludeを足したり、上記の文章を読み直したりするうちに、エコーバックっていう概念がわかってない気がしてきました。

さらに調べたところ、getchにはconio.hが必要なのですが、macだとトリッキーな方法(無理やりヘッダを足す？)を用いないと使えないようです、たぶん。また、エコーバックについて調べるうちに、たまたま[ここ](http://okwave.jp/qa/q3121681.html)の回答にたどり着きました。コード例を動かすとエンター待ちが起きません！(isprintはわからないのでコメントアウトしました)。
ありがとうnoocyteさんヾ(｡>﹏<｡)ﾉﾞ✧\*。

欲しい動作(エンター待ちなし)をするコード例ですが、理解できないのでさらに調べます。cookedモードとrawモードを検索ワードにすると、あっさり知りたかったことが出てきました。著者の理解の範囲では下記の動作です。

  - rawモード: エンター待ちしない。コンソールはなにもせずに、1文字ずつC言語側にイベントを渡す

  - cookedモード: エンター待ちする。コンソール側で入力を貯めておいて、エンターキーを押すと行ごとC言語側に渡す


rawモードのままプログラムを終了させてしまうと、そのコンソールは反応しなくなります。C言語側・コンソール側ではcookedモードに復帰してくれません。異常終了しないプログラムを書け！っていう重圧を感じます・・・。

別途気になることが2点出てきます。

  - 筆者はどうしても十字キーでカーソル移動したいのですが、十字キーってマルチバイト文字(3バイト)なのか・・・、つらい。少なくとも2バイト前まで覚えていないといけさなそうです。マルチバイト簡単説(2日目より)が揺らぎ始めます。。。

  - 筆者はviとかemacsの独特なキーバインドが覚えられないので、cmd + s(Macの場合)でファイル保存を目論んでいたのですが、押してもC言語側にイベントがこなさそうです(ターミナルの出力を保存、の旨のMac側のメニューが開く)。こっちはどうやっても無理なんでしょうか？そんな気配がしますね。そういう操作を受け付けるコマンドをみたことない気がしてきました。

なんか勘違いをしていた気がするので、rawモードでfgetsを試してみました。思った通りには動かない(値が取れない)ですが、それは行をバッファリングすることを期待する関数をrawモードで呼ぶからでしょうか？。自信はないのですがとりあえず、fgetsは目的と違うものということにしておきます。(コピペを繋ぎ合わせて動作確認したのですが、出典元をメモしてなかったのでコードは秘密です)

結論としては、**rawモードでgetcharをつかい、終わったら`絶対に`cookedモードに戻して終了する**のがよさそうです。

# 今度こそキーボード操作したい

思ったペースで進まないのがプログラマの運命ですが、いちおうの結論が出たのでキーボード操作に再挑戦します。こんなに調査に時間がかかるとは・・・。

2日目のコードにくっつけてみます。
（補足: 2日目にBUFFER_SIZEを2にしていたのは、逆順にしやすい・マルチバイト文字が分断されたときの動作が知りたかったからでした。今日は100にしてみます）

```main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 100

struct termios CookedMode;
struct termios RawMode;

typedef struct _string {
//昨日と同じ
} string;

string* insert(string *from) {
//昨日と同じ
}

void file_read(char* filename, string* head) {
//昨日と同じ
}

int main(int argc, char *argv[]) {
  string* head = malloc(sizeof(string));
  head->prev = NULL;
  head->next = NULL;
  if(argc != 2) {
    printf("[error]illegal args\n");
  } else {
    file_read(argv[1], head);
    string* current = head;

    int input_key;
    tcgetattr(STDIN_FILENO, &CookedMode);
    cfmakeraw(&RawMode);

    tcsetattr(STDIN_FILENO, 0, &RawMode);
    while(current) {
      printf("\e[2J\e[H");//clear
      printf("%s", current->str);

      input_key = getchar();
      if(input_key == 110/*110 is n*/) {
        current = current->next;
      }
      if(input_key == 112/*110 is p*/) {
        current = current->prev;
      }
      if(input_key == 101/*101 is e*/) {
        printf("\e[2J\e[H");//clear
        break;
      }
    }
    tcsetattr(STDIN_FILENO, 0, &CookedMode);

  }
  return EXIT_SUCCESS;
}

```

試す場合は、`gcc main.c; ./a.out main.c`して、nで次のページ、pで前のページ、eで終了です。

補足: 1日目の投稿のコメントで教えてもらいましたが、`\e[1;1H`は両方1のときは省略記法として`\e[H`と書けます。

**おお！すごい！lessコマンドっぽい！！！(筆者談)**


# 今日のまとめ

- コンソールにはcookedモードとrawモードがある。エンターキーを毎回押さずに操作できるのはrawモード。
- 2日目とあわせてlessっぽいものができた


実を言うと、main.cに継ぎ足し続けるのはもうだめです。
突然こんなこと言ってごめんね。でも本当です。
明日はソースコードをファイルを分けて書けるようにしたい！
(書き溜めてないのと、諸事情あり職場が炎上しているので遅れるかもしれません)

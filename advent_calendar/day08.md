本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の8日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

このアドベントカレンダーは、コンソール上で動くエディタを25日間で1から自作することを目標としています。

# 前日分からの続き、マルチバイト文字について考える

wikipediaの[ここ](https://ja.wikipedia.org/wiki/UTF-8)から引用すると

> 1バイト目の先頭の連続するビット "1"（その後にビット "0" が1つ付く）の個数で、その文字のバイト数がわかるようになっている。
> また、2バイト目以降はビットパターン "10" で始まり、1バイト目と2バイト目以降では値の範囲が重ならないので、文字境界を確実に判定できる。
> すなわち、任意のバイトの先頭ビットが "0" なら1バイト文字、"10" なら2バイト以上の文字の2番目以降のバイト、"110" なら2バイト文字の先頭バイト、"1110" なら3バイト文字の先頭バイト、"11110" なら4バイト文字の先頭バイトであると判定できる。

つまり、

- [前回調べた](http://qiita.com/masakielastic/items/abf8e54344034f9b089c)ように、先頭バイト等ををunsigned intとして見た時に、ある値からある値の境界に収まっているかチェックする方法と、
- 上記引用のように、先頭のbitを数える方法

があるようです。

また、やはりwikipediaから引用すると

> 最短ではない符号やサロゲートペアなど、UTF-8の規格外だがチェックを行わないプログラムでは一見正常に扱われるバイト列が存在する。これらのバイト列を入力として受け入れてしまうと、プログラムが予期しない範囲のデータを生成するため、セキュリティ上の脅威となりうる。

これは、簡単そうなのでチェックしてみます。

またまた、やはりwikipediaから引用すると

> ただし、Unicodeでは一部の文字を合成によって表現することもできるから（例：「ぱ」は、U+3071のほかにもU+306F U+309Aでも表現できる）

ここも心配ですね。ただ、合成させる場合のルールがまだよくわからないです。

最近、エディタをつくる内容からマルチバイト文字について悩む内容に変わってしまっているので

- とりあえずUTF8のみ対応。ただし、合成文字は後で考える
- 他の文字コードも後で考える
- 符号が最短かは調べる。危ない文字は読み飛ばす

いったんそういうことにして、話を進めていきます。

- 符号が最短かは調べる。危ない文字は読み飛ばす

は実装してみると難しくて、ゴリ押し感がにじみ出てるコードになってしまいました。余裕があれば直したいところ。

```snippet.c
int mbchar_size(mbchar mbchar, int byte)//PUBLIC;
{
  if(byte < 1 || UTF8_MAX_BYTE < byte) return MBCHAR_ILLIEGAL;

  if(byte == 1 && mbchar[0] == 0x00) return MBCHAR_NULL;

  int head_one_bits = 0;
  while(head_one_bits < 8)
  {
    if((mbchar[0]>>(7-head_one_bits) & 0x01) == 1)
    {
      head_one_bits++;
    } else {
      break;
    }
  }
  if(head_one_bits == 0) head_one_bits++;

  if(head_one_bits > byte) return MBCHAR_NOT_FILL;

  if(head_one_bits == byte)
  {
    switch(byte)
    {
      case 1:
        return byte;
        break;
      case 2:
        if (mbchar[0] & 0x1e) return byte;
        break;
      case 3:
        if (mbchar[0] & 0x0f || mbchar[1] & 0x20) return byte;
        break;
      case 4:
        if (mbchar[0] & 0x07 || mbchar[1] & 0x30) return byte;
        break;
      case 5:
        if (mbchar[0] & 0x03 || mbchar[1] & 0x38) return byte;
        break;
      case 6:
        if (mbchar[0] & 0x01 || mbchar[1] & 0x3c) return byte;
        break;
    }
  }

  return MBCHAR_ILLIEGAL;
}
```

こんな感じになりました。

# debugしやすくする

上記の修正をする際に色々失敗があったのですが、
そろそろプログラムが長くなってきたので、`Segmentation fault: 11`がつらいです。
これのつらいところは

```snippet.c
printf("start");
bad_func(x);//ここでSegmentation fault
printf("end");
```

というプログラムがあったときに、(タイミングによっては？)startも出力されないところです。

[ここ](http://d.hatena.ne.jp/JiaLu/20120113/1330396897) を参考に、Makefileを修正してCoreDumpできるようにしました。

エラーメッセージが`Segmentation fault: 11 (core dumped)`になりました。
gdbは入ってなかったので`brew install gdb`して、
コアファイルはMacだとルートディレクトリの/coresに入るようです。

`gdb ./main /cores/core.68090` して・・・

```
(gdb) bt
No stack.
```

あれ。

どうやらMacのgccは、linuxのgccとフォーマットが違うらしく、`lldb --core /cores/core.68090`して`(lldb) bt all`で上手くいくようです。[参考](https://github.com/Homebrew/legacy-homebrew/issues/44089)

やってみると

```
* thread #1: tid = 0x0000, 0x0000000103353756 main`debug_print_text + 86, stop reason = signal SIGSTOP
  * frame #0: 0x0000000103353756 main`debug_print_text + 86
    frame #1: 0x00000001033536f0 main`render + 80
    frame #2: 0x000000010335346b main`main + 91
    frame #3: 0x00007fff8e8665ad libdyld.dylib`start + 1
    frame #4: 0x00007fff8e8665ad libdyld.dylib`start + 1
```

こんな感じのバックトレースがでてきました。+の後は行数ではない何かのようです。なんだろう？
とにかく、落ちてる場所はわかるようになりました。
`(lldb) frame variable`は動かなかった(変数が何も表示されない)ですが、なぜかは不明のままでした。

ついでに、コンパイルオプションに`-Wall -Wextra`をつけてみました。
今まで無視されていた警告もでるようですが、現時点では効果が実感できません。
でも怪しいところにwarningがでれば嬉しいですね。

# debugのために、試しに動かしてみる

このサイトが結構便利です。

https://repl.it/languages/c

僕の環境に限った話として、main.cに引数を渡してないときの挙動をまだ実装していないので、
そこで色々試すのも便利です。

# 今日のまとめ

- ついにマルチバイト文字がハンドリングできるようになった
- lldbで落ちる場所を探せるようになった。

明日はカーソルを作るぞ！

---

なんだか炎上してしまっているアドベントカレンダーがあって、悲しい気持ちになります。
僕はフリーランスではないのですが、あまり出来る人ではないので、そういう人にも優しい世の中であってほしいものです。
最終日には何か書いてほしいなぁ。
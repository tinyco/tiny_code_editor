本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の1日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

# はじめに

このアドベントカレンダーは、コンソール上で動くエディタを２５日間で１から自作することを目標としています。

基本的には1人で連続で書きます。

目標としては、普通に編集できて保存できるところまで行くといいなぁ、といったところです。

なんとなくコンソールと相性が良さそうなイメージがあるC言語で書こうと思っていますが、仕事で使ったことはないので初歩的なことがミスってるかもしれません。もし参考にする場合はその点に注意してください。

レポジトリは、 https://github.com/tinyco/tiny_code_editor で公開しています。

macの標準的なbashで動作確認しています。

# hello,world.

- gccを入れます。brew install gccか、xcodeを入れるか、xcode-select --installで入るようです。

- ソースコードを書きます。特に解説するところがないですね。

```main.c
#include <stdio.h>

int main(void) {
  printf("hello,world.\n");
}
```

- \$ gcc main.c でコンパイルします。

- \$ ./a.out で実行します。hello,worldが表示されれば成功です。

- \$ gcc main.c -o hello を実行すると、出力される実行ファイルがa.outからhelloになり、./helloで実行できます。

- さらにパスを通すと./が不要になり、helloで実行できます。ちょっとコマンドっぽいですね。(パスは\$ export PATH=\$PATH:/ダウンロード先/tiny_code_editor/src等で通せます)

# 引数を取得する

- \$ vi sample.txt のように、エディタが開く対象を指定できるようにしたいです。そのために、実行したときに右につけた文字列を取得したいですね。

- `int main(void)`を`int main(int argc, char *argv[])`に変えると、引数の数と文字列が取得できるようです。argcの数だけargvに文字列が入っていて、それを超える添字にアクセスするとランタイムエラーが起きます。

```main.c
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("hello,world.\n");
  for(int i=0; i<argc; i++) {
    printf("receive > %s \n",argv[i]);
  }
  printf("don't receive > %s \n",argv[100]);
}
```

を実行すると

```実行結果
$ gcc main.c
$ ./a.out sample1.txt sample2.txt
hello,world.
receive > ./a.out
receive > sample1.txt
receive > sample2.txt
Segmentation fault: 11
```
になりました。

- 変数をprintfするときには、文字列のときは%sを、整数のときには%dを入れたところに展開されます。

- argvにはコマンド自身(今回は./a.out)も入りました。argcが0の場合は考慮しなくて良さそうですね。

- Segmentation faultは、確保してない変数へのアクセスで起きるランタイムエラーっぽいです。11はなんですかね？エラー番号？？

- argv.sizeとかargv.length的なものは無さそうです。Segmentation faultが起きる位置を把握しているならあっても良さそうな気がするんですが。アクセスした瞬間に怒られるのでif(argv[100] != NULL)とかもできなさそうですね。

# コンソールの左上から表示したい

- viにしてもemacsにしても、起動した瞬間に左上から文字が書かれていると思うのですが、上のmain.cはlsやcatコマンドのように今までのコンソールの続きから出力されます(伝わりますかね？)。コンソールの、見た目の左上の位置にprintfしたいです。

- 調べたところ、system("cls")を使う方法もあるようですが、自分の環境では`sh: cls: command not found`でした。Windows環境のみですかね。

- [ここ](https://www.grapecity.com/tools/support/powernews/column/clang/047/page02.htm)を参考に、特殊な文字`\e[2J`と`\e[1;1H`とを出力すると、前者で画面をクリアして後者で左上の位置にカーソルを移動できました。画面のクリアだけをしてもカーソル位置が上に行くわけではなさそうです。


```main.c
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("\e[2J\e[1;1H");//clear
  printf("\e[31m\e[47m");//color
  printf("hello,world.\n");
  printf("\e[39m\e[49m");//reset
  for(int i=0; i<argc; i++) {
    printf("receive > %s \n",argv[i]);
  }
}
```

- 色も同様の方法で付けれました(๑•̀ㅂ•́)و✧

- この特殊な文字はC言語に用意されたものではなさそうです。試しにnode.jsで似たコードを書いても動作しました。(zsh等では試せていません。)

# 今日のまとめ

- 環境構築できた
- 引数が取得できた
- 画面をクリアできた
- 文字に色がつけれた

初日・初心者なので、初歩的な内容で申し訳ないです。

先は長そうですね・・・、25日までにエディタになるんだろうか！？

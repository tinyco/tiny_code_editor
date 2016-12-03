本稿は[自作エディタを作る Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の4日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

このアドベントカレンダーは、コンソール上で動くエディタを25日間で1から自作することを目標としています。

> 前回まで、ひたすらmain.cに継ぎ足し続けていました。
> 今日はソースコードのファイルを分割してビルドできる環境を整えていきます

# とりあえずファイルを分ける

昨日までのmain.cを、何も考えずに以下のように3つに分けてみます。

```main.c

typedef struct _string {} string;
// lib/string.cへ

string* insert(string *from) {}
// lib/string.cへ

void file_read(char* filename, string* head) {}
// lib/file_read.cへ

int main(int argc, char *argv[]) {}
// main.cのまま
```

当然ですが、移動した関数はmain.cから無くなるため、このままではコンパイルが通りません。

そこで、移動先のファイルを読み込みます。

```main.c
#include "lib/string.c"
#include "lib/file_read.c"
```

無事コンパイルが通りました。`<>`ではなく`""`で囲むと、そのファイルからの相対パスが指定できるようです。

正直言ってこのままでも良い気もするのですが、世間的には良くない方法のようです。

検索した感じによると、よくないとされる理由は

- 毎回全てのファイルをコンパイルするのは効率的ではない
- 全ての関数をpublicとして扱うのはよくない

の2点が主な理由のようです。他に、自分が気になった点としては、

```main.c
#include "lib/file_read.c"
#include "lib/string.c"
```

と書くと、file_readにstringを使っているためエラーになってしまいます。

- 依存先を、includeを書く順番ではない方法で指定できない

のはよくないですね。

逆にメリットとしては、少ない手順で気軽にファイルを分けれる点でしょうか。コンパイル時間等のデメリットも、今の規模であれば全然気にならないので作り始めの時は有効な作戦かもしれないなぁ、と思いました。

現時点で上記の方法にそこまでのデメリットを感じられないのですが、せっかくなので世間で勧められている方法にチャレンジしていきます。

# ヘッダファイルを作る

世間的に、上記のデメリットを克服するために、ヘッダファイルを作成するようです。

ヘッダファイルに書く内容としては、主に関数のプロトタイプ宣言です。

プロトタイプ宣言は、

```snippet.c

//プロトタイプ宣言
int hoge(int arg);

//プロトタイプ宣言すると、
//関数の中身を定義する前でも使えるようになる
hoge(3);

//関数の中身を定義
int hoge(int arg) {
  arg += 10;
  return (arg);
}
```

のようにすることで、関数の中身の定義の場所をファイルの中で自由に動かすためのテクニックです。

今までは、たまたまmain関数を最後に書いていたので大丈夫だったみたいですね。

このプロトタイプ宣言があると、関数の中身の定義がなくてもコンパイルが通るようになります。ただし、関数の中身が無いとコンパイルの後のリンクと呼ばれる手順でエラーになります。

ビルド手順を、コンパイルとリンクにわけて、コンパイルの時はプロトタイプ宣言で関数を使用できる状態にするためのファイルがヘッダファイルのようです。

とりあえず、以下の手順でヘッダファイルを作りました

- 依存先の多い#define BUFFER_SIZE 100 をconst.hに移す
- 関数のプロトタイプ宣言をヘッダファイルに書く
- 構造体の型宣言は書く場所がわからなかったが、とりあえずヘッダに移す
- $gcc -c 該当ファイル を実行したとに怒られないように#include "xxx.h"を足していく

このときに、string.hはmain.c/main.cが読むfile_read.hの2つから参照されるために、重複して定義している旨のエラーが出ます。それを防ぐために、

```
#ifndef フラグ名
#define フラグ名

//ここにプロトタイプ宣言等を書く

#endif
```

というマクロを使います。これをインクルードガードと呼ぶようです。

また、"string.h" というファイル名にしておくと、見つからなかった時に<string.h>を探しにいきます。
これは標準ライブラリとして見つかるために、混乱の原因となりました。
標準ライブラリと同じファイル名にするのはよくないですね。

無事にヘッダファイルができたので、以下の手順でビルドできます。

```
$gcc -c main.c
$gcc -c lib/file_read.c
$gcc -c lib/string.c
$gcc main.o file_read.o string.o -o main
$./main main.c
```

gccの動作としては、

- $gcc main.c はコンパイルもリンクもする
- $gcc -c main.c はコンパイルだけする
- $gcc main.o はリンクだけする。-o オプションで出力ファイル名を決めれる

毎回これをするのはめんどくさいですね。

# Makefileを作る

どうやらMakefileを作ると、自動で

```
$gcc -c 色々なファイル
```

の手順をやってくれるようです。Makefileの書き方は

```
作りたいファイル名: 作るのに必要なもの
[タブ文字(空白文字不可)]作るためのコマンド
```

です。出来たのが、以下です。

```Makefile
main: main.o file_read.o string.o
	gcc main.o file_read.o string.o -o main
main.o:
	gcc -c main.c
file_read.o:
	gcc -c lib/file_read.c
string.o:
	gcc -c lib/string.c

clean:
	rm -rf *\.o main
```

`make`（make mainでも可）でビルドできて、`make clean`で作ったファイルを消すことができます。

ちょっとファイル構成いじるたびに、Makefileを書き直すのがすごく辛そうです。

# よりよいMakefileを作る

[ここ](http://blog.wagavulin.jp/entry/20120405/1333629926)と、[ここ](http://d-rissoku.net/2013/06/)を参考に、
毎回ファイル名を指定しなくても上手く動くMakefileを目指していきます。
Makefileには色々なテクニックがあるため、正直全部を理解できなかったのですが、自分なりに頑張ってできたのが以下です。

```
PROG := main
SRCS := $(wildcard ./*.c) $(wildcard ./**/*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(PROG)

$(PROG): $(OBJS)
	gcc -o $@ $^

.o:
	gcc -c $@

clean:
	rm -rf $(PROG) $(OBJS)
```

ポイントしては、

- PROG := main のような書き方で、定数をつくれる
- $(wildcard)をつかうと、findコマンド相当の指定ができる
- OBJSは、makeしようとした瞬間にはまだ存在しないため、wildcardでは指定できない。
  - SRCSをsed s/\.c/\.o/g相当のことをするコマンドを用いて作るのが定番
- このMakefileでは、ヘッダファイルを変更するたびにmake cleanする必要がある


そもそも、毎回makeコマンドを打つのが面倒な気がしてきました。

# 自動でmakeする

あまりC言語において一般的とは言えない方法ですが、gulpを使ってファイルを保存するたびにコンパイルが通るか確認するようにします。

そのための手順は

1. npm init コマンドでpackage.jsonを作成
1. sudo npm install gulp -g
1. npm install gulp --save-dev
1. 以下のようにgulpfile.jsを作成

です。

```gulpfile.js
let gulp = require("gulp")
let child_process = require("child_process")
let exe = (cmd, callback) => {
  let red   = '\u001b[31m'
  let green = '\u001b[32m'
  let reverse = '\u001b[7m'
  let reset = '\u001b[0m'
  child_process.exec(cmd, (error, stdout, stderr, callback) => {
    console.log(reverse + '> ' + cmd + reset)
    if(stdout){
      console.log(green + '[ok]  ' + reset + stdout)
    }
    if(stderr){
      console.log(red + '[ng]  ' + reset + stderr)
    }
    if (error !== null) {
      console.log(red + '[err] ' + reset  + error)
    }
    callback && callback()
  })
}

gulp.task("h", () => {
  gulp.watch("\./**/*\.h", event => {
    exe("make clean",exe("make"))
  })
})

gulp.task("c", () => {
  gulp.watch("\./**/*\.c", event => {
    exe("make")
  })
})

gulp.task("default", ["h","c"])
```

これで、`gulp`が起動している間は、ファイルを保存するたびにmakeしてくれるようになりました。

> このMakefileでは、ヘッダファイルを変更するたびにmake cleanする必要がある

この問題も、自動でmake cleanしてくれるので解決！？ですね。(いいのか・・・)

# 自動でheaderを作る。

この方法は、まったく一般的でない方法(オレオレフレームワーク)です。
そもそも、ヘッダファイルを手で書くのは二重管理になってつらい気がしてきました。
Objective-Cのときは特に疑問に思わなかったけれど、Swift書くと戻れない、みたいな感じです。
ヘッダファイルも無理やり自動で作ることにします。

作るものの仕様としては以下4点です。

1. `#include "string.h"//PUBLIC`のように、`//PUBLIC`という特殊なコメントがある行を、`#include "string.h"`のように出力する。
1. `int hoge(int arg)//PUBLIC;`のように、`//PUBLIC;`という特殊なコメントがある行を、`int hoge(int arg);`のようにセミコロンをつけて出力する。
1. `/*EXPORT ヘッダに書く内容*/`のコメントから`ヘッダに書く内容`をヘッダにコピーする
1. ファイル名を使って自動でインクルードガードをつける

とりあえず、javascriptで作ってみました。レポジトリを分けて[ここ](https://github.com/tinyco/clang_gen_header)に置いておきます。

```generator.js

const fs = require('fs')
let error = i => i && console.log(JSON.stringify(i, null, 4))
let init = filename => fs.writeFile (filename, '' , err => error(err))
let write = (filename,str) => fs.appendFile(filename, str ? str + '\n' : '' , err => error(err))

if (process.argv.length < 3) {
  console.log('Error: illegal argument.')
} else {
  let args = process.argv.filter(i => i.endsWith('.c'))
  args.map(c_file => {
    fs.readFile(c_file, 'utf8', (err, sorce_code) => {
      if(err) {
        error(err)
      } else {
        let h_file = c_file.replace(/.c$/,'.gen.h')
        init(h_file)
        const flag = c_file.toUpperCase().replace(/[\/\.]/g,'_')
        write(h_file, '//This file is auto-generated from ' + c_file)
        write(h_file, '#ifndef ' + flag)
        write(h_file, '#define ' + flag)

        write(h_file, '//EXPORT')
        sorce_code
          .split('/*')
          .map(i => i.split('*/')[0])
          .filter(i => i.indexOf('EXPORT') === 0)
          .map(i => i.replace('EXPORT\n',''))
          .map(i => write(h_file, i))

        write(h_file, '//PUBLIC')
        sorce_code
          .split('\n')
          .filter(i => i.indexOf('//PUBLIC') !== -1)
          .map(i => i.replace('//PUBLIC',''))
          .map(i => write(h_file, i))

        write(h_file, '#endif')
      }
    })
  })
}
```

こうして自動できた*.gen.hですが、nodeが入っていない環境でもビルドできるように、gitに含めることにしました。
これで、ヘッダファイルを書かなくても大丈夫ですね！

# 今日のまとめ

- ソースコードのファイル分割をするために
  - headerを書いた
  - makefileを書いた
  - gulpでmakeするようにした
  - headerのgeneratorをつくった

makeファイルは奥が深いですが、C言語専用の道具ではなく、他の言語でも使えそうなのでもう少し勉強したいですね。

足場は揃ってきた気がするので、そろそろ設計を考えたいです。

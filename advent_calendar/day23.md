本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の23日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

# プルリクエストを貰いました

またequal-l2氏から頂きました。

`*str = malloc(strlen(argv[1]));`

は、文字列末端のヌル文字をコピーできていないために良くなくて、

`*str = malloc(strlen(argv[1])+1);`

が正しいようです。strlenのドキュメントに明記されてました。

ちなみに、strlenを使っているもう片方の箇所はヌル文字をコピーせずに使いたい箇所なので問題ありませんでした。
つまり、もっと注意力を持って関数を呼ばないとダメですね。
C言語はmalloc/freeと配列の何番目かがとにかく大変だと思います。
これは慣れの問題ではないような・・・、つらい。
jsのようにアクセスされたところまで確保領域が伸びる配列のようにつかえるリストを作ったら流行るのでは？って思いました。

# 静的解析ツール(infer/clang-check)

tenmyo氏と19日目のコメントと、equal-l2氏の21日目のコメントをもとにいじっていると、静的解析ツールがいくつか動くようになりました。

ポイントは静的解析ツールに

`export C_INCLUDE_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include:$C_INCLUDE_PATH`

でstdio.h等の位置を教えてあげることで、

- facebook infer
- clang-check

は自分の環境では動くようになりました。

ネットを見るに、`usr/include`や`usr/local/include`にstdio.h等がある状況もあるかもしれない。
自分はなかったです。

ちなみにMacOSの10.12.2です。

```
find . | grep '\.c$' | xargs infer -- clang -c
(infer-outフォルダのbugs.txtに結果があります)

find . | grep '\.c$' | xargs clang-check -analyze 2> clang_check.txt
```

たぶんこんな感じで動きます。infer-outとplistをgitignoreすると良さそうです

19日目のcppcheckと合わせて、今回のコードに使った感想だと、指摘がほとんど重複してないです。

- cppcheckは、表面的な文法の誤りを探す雰囲気
- clang_checkは、どことなくiOSアプリを作る時のXcodeのwarningみたいな内容
- inferは、コードをよく解析して挙動自体の誤りを探してる雰囲気

がしました。cppcheckは指摘する種類が少ない気がしますね、なんとなくですが。

facebook inferはAndroidアプリ、iOSアプリの指摘もしてくれるので、そちらも使ってみました。
けっこう良い感じの指摘があったので、その用途もオススメです。

また、Macではgccがclangに置き換わっているので、
`-fno-omit-frame-pointer -fsanitize=address`をつけるとセグフォ(及びその予備軍)を教えてくれます。
色々メッセージが出て詳細なことを教えてくれますが、
とりあえずメッセージ上部ぐらいしか意味がわからず。それでも便利です。

# 余談

C言語は歴史が長いからか、コンパイラのエラーメッセージが結構親切な気がします。
74行目にセミコロンがないよ、その場合は変数名に&をつけたほうがいいよ・・・とか。
なので、そのエラーメッセージを受け取って、
自明そうなエラーであれば勝手にセミコロンをつけたり&をつけたりするツールを作ったら
便利かもって思いました。

もし変な改変されても、たぶんgitで戻せるし良いのでは？

# まとめて削除

前回、カーソルだけは範囲指定できるようになったので、
その状態で文字を打つかbackspaceを押した場合はその範囲を削除するようにしました。

文字を打つ時も、範囲指定状態であれば削除してから挿入するようにします

# 今日のまとめ

- 静的解析ツールが動くようになりました
- 選択範囲を削除できるようになりました

静的解析ツールやコードフォーマットツールはやはり便利ですね。
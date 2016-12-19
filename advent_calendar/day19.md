本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の19日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)


自分の中で読みづらいと思っているのは、主にstring_type.cです。
今日はそこを綺麗にしつつ、バグを取っていきます。

# コードを読みやすくする

いままで、

- utf8の1文字をmbchar型
- 行先頭から末尾までをline型
- ファイル先頭からファイル末尾までをtext型と名付けていましたが

それぞれutf8char型、mutable_string型、lines型と呼ぶことにします。

名付けたときの思いに反して、line型が示す内容が直感的ではない気がしました。
text型がlineっぽい名前であるべきかなと。

あとは、より説明的な名前にしました。

mbcharはmulti-byte charのつもりだったのですが、
現時点ではutf8しか対応できてないのでutf8charにしました。

あわせて、メソッド名もわかりにくいものは順次なおしています。

引数がわかりにくい似たメソッド(X座標とY座標)を、メソッド名に引数はX座標だよ、
みたいな命名したところだけがキャメルケースになっていました。(他はスネークケース)
書いたとき、Objective-Cを連想したんでしょうか。
・・・Cって普通スネークケースなのかな。cursesはスネークケースだったけど。

# nullチェックと静的解析

16日目を参考に、地道にnullチェックを追加していきます。
地道すぎる、静的解析ツールほしい！と思って`cppcheck`を試して見ます。

`$brew install cppcheck`
`$cppcheck --enable=all . 2>err.txt`

すると

```
[file/reader.c:17]: (style) The scope of the variable 'c' can be reduced.
[file/reader.c:20]: (style) The scope of the variable 'mbsize' can be reduced.
[type/lines_type.c:97]: (style) Variable 'prev_width' is assigned a value that is never used.
(information) Cppcheck cannot find all the include files (use --check-config for details)
```

3行しか怒られなかった。nullチェック漏れは見つからず。

4行目の --check-config をつけると
`[main.gen.h:7]: (information) Include file: <stdio.h> not found. Please note: Cppcheck does not need standard library headers to get proper results.`
見つからなくても大丈夫らしいので、気にしないことにしました。
いちおう指摘をなおします。

[facebook-infer](http://fbinfer.com/)は使い方がわからず。

```
$ infer -- clang -c main.c
Capturing in make/cc mode...
In file included from main.c:1:
./main.gen.h:7:10: fatal error: 'stdio.h' file not found
$infer -- gcc -c main.cもだめでした。
```

こちらはstdio.hがあるフォルダにシンボリックリンク貼ったりしたけど、結局諦めました。

そして地道にnullチェックを続けます。

mutable_string(ポインタ)を別のラインにつけかえて、
それを所持していたlineをfreeするときに、
間違えて再利用したはずのmutable_stringまでfreeしていた箇所を直すと、
だいぶ動作が安定してきました。

・・・静的解析は教えてくれなかったけど。

# 余談: 可変長配列の実装

**全体的に、合ってるかの自信は全くないです**


### swift

世の中の可変長文字列はどうなってるんだろう、可変長配列になってるのかな、と思ってswiftを調べてみました。


https://github.com/apple/swift/blob/master/stdlib/public/core/String.swift#L670

bufferなるものを足していくらしい、たぶんmallocみたいなやつなのかな

https://github.com/apple/swift/blob/master/stdlib/public/core/ArrayBuffer.swift#L127

capacityを指定してmallocするらしい、たぶん

つまり、必要に迫られたときに必要に応じて確保して後ろにつなぐ。
サイズがマチマチなメモリ領域をリストでつなぐ構造・・・なのかな？

https://github.com/apple/swift/blob/b4d30b7c1a0189290410aa2df983d618abaca3db/stdlib/public/core/ArrayBuffer.swift#L269

ここがリスト構造っぽいけど、_SliceBuffer内のイテレータの動きがわからず確信が持てない。

swiftってswift自体(とgyp)で書いてるんですね。参照カウンタっぽいコードが多い。

### PHP

PHPを調べると

https://github.com/php/php-src/blob/master/Zend/zend_hash.c#L174

hashになってるのかな。プリミティブ型としてarrayっぽいものがない雰囲気。

いや、配列型がないわけないような・・・。見つけられず。

### Ruby

Rubyは

https://github.com/ruby/ruby/blob/trunk/array.c#L210

配列を伸ばすときに、おっきい領域を確保しなおして、そこにコピーかな？たぶん。

Rubyって標準ライブラリいっぱいあるなって思いました。


# 今日のまとめ

- 徐々に読みやすいコードを目指しています
  - メソッド名・型名を他の人が見やすくした、つもり
  - フォルダわけも少し手直し
  - 1ファイル100行以内を努力目標にして、だいたい130行以内くらいに収まりつつあります
- バグ修正して、だいぶ落ちなくなってきました(当社比)
- 静的解析ツールを試してみました、微妙かも

明日は、後回しにしていた自動テストとincludeの見直しをしたい！

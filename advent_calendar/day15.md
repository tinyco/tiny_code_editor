本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の15日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

# ver0.0.1をつくるぞ！

単体テスト・ncurses対応・制御文字と文字コード対応・カーソルの仕様の微妙さ・・・。

やりたいことは色々ありますが、ロマンのひとつである「自作エディタを使ってエディタを作る」ところを今日は目指します。

それができたら、とりあえずはver0.0.1ということにします(๑•̀ㅁ•́๑)✧


# ファイルを上書き保存する

キーバインドは悩んでますが(13日目参考)、とりあえず`Ctrl + S (0x13)`を上書き保存にします。

いつも通り、まずは内部DSLの構造体の定義から作っていきます。

ここもenumとはいえ混沌の気配が生まれつつあります。十字キーが予想よりやっかいなやつでしたね。

いったん目をそらしながら内部DSLを継ぎ足して、2日目のファイル読み込み部分を参考に書き込みも作っていきます。

読むときはマルチバイト文字に苦しみましたが、書くときは文字コードを気にせず1バイトずつ書いていけばいいので楽です。

```write.c
while(current_text)
{
  current_line = current_text->line;
  while(current_line)
  {
    i = 0;
    while(i < current_line->byte_count)
    {
      fputc(current_line->string[i], fp);
      i++;
    }
    current_line = current_line->next;
  }
  current_text = current_text->next;
}
```

なんか、あっさり書き込めました！

# 本文を、真面目に表示する

いままで、エディタとかカッコつけたことをいっても多機能なprintfでしかなかったので、
画面に収まらない、テキストが長いファイルを開くと困ってしまいます。
そこで、カーソルを中心に画面に収まるように本文を表示するようにします。
ヘッダー部分(画面上部)にファイル名を表示しているのですが、
本文をどこまで書いたかわかりやすいように、画面の下部にもヘッダーと同じものを出すようにながら進めました。
フッターと呼ぶことにします。

カーソルが画面下端から飛び出ると表示開始行を足していき、上端から出た場合は表示開始行を減らすようにしました。

なんか画面がチラつくのですが、いちおう表示できるようになりました。

文字列の幅とかを結構な頻度で再計算するのでチラつくのかもしれません。

# せっかくなので、フッターになにか表示する

試しに`getcwd`をつかってファイルパスを表示してみました。

なんとなくそれっぽい感じがします

# 空行でもカーソルを表示する

ちょっとアドホックな修正ですが、カーソル位置が空行の時はカーソル色付きの空白文字をprintすることで
カーソルを空行でも表示できるようにしました。

# linuxでビルドしてみる

なんとなく、AWSのLinux上でビルドしてみました。
`‘for’ loop initial declarations are only allowed in C99 mode`
どうやら`for(int i = 0; i < bytes; i++)`があるとgccではコンパイルがすんなり通らないようです。
そのあと動かしてみましたが、ssh上では普通に動きました。
ビルドとコンソールはLinuxですが、端末エミュレータはMacだからかもしれません(たぶん)

# readmeをちょっぴり親切にする

まだ使う人がいないと思いますが、いちおうビルド方法を書いておきます。

# ver0.0.1完成！

これで、自作エディタを使ってエディタを作れますね٩(๑❛ᴗ❛๑)۶

なんとなくgithub issueをonにして、git tagを打ちました。
25日までに出来そうなことをまとめたいです。


# 今日のまとめ

- ファイルの上書き保存
- 本文の表示
- ver0.0.1できたよー！

正直なところ、このあたりの完成度で25日目を迎えるような気がしていました。

けっこう動いてて感動がありますね！画面がチラつくけど、それはそれ！
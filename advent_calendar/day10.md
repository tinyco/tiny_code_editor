本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の10日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

更新遅れました・・・。言い訳ですが、今日は私用があり殆ど進んでません。明日頑張ります！

# 色々な人のコメントについて

とてもありがたいです！

ずっと疑問に思っているのは、なぜみなさんこんなに文字コードに詳しいんだろう、ということで
オススメの本や資料等があれば教えて欲しいです。
個別のわからないことは、みなさんのご指摘で僅かながら理解が進むのですが、文字コードの全体像みたいなものが見えないなと思っています。
体系だって理解できていないというか。

それとは別に、文字コードに詳しくなるきっかけってなんだろう、とか思ったりします。仕事上で重要になる業界があるのでしょうか。出版とか？

返事ができていないコメントもあるのですが、頂いた参考リンクを読み終え次第、返事します。

# 文字の挿入(途中)

とりあえず、ずっとやりたかった文字の挿入をできるようにしたいです。
前回まででカーソルを移動できるようになっているのですが、十字キーをハンドリングできていないので(u/d/l/rキーと、終了でeキーを使用している)
それ以外の文字を全て挿入できるようにしてみます。

再掲ですが

```
enum CommandType {NONE, UP, DOWN, LEFT, RIGHT, EXIT};

typedef struct _command {
  enum CommandType command_key;
  mbchar command_value;
} command;
```

いまキーボードの文字はこういう風にラップされていて、valueにutf8として1文字の、数バイトのcharがはいっています。
CommandTypeにInsertを加えて、カーソルの位置に文字を追加してみます。


# 行の取得

```
line* getLineHeadFromPositionY(text* head, unum position_y)//PUBLIC;
{
	unum i = position_y-1;
	text* current_text = head;
	while(i-- > 0 && current_text)
	{
		current_text = current_text->next;
	}
	if(current_text) return current_text->line;
	return NULL;
}
```

こんな感じに挿入したい行の先頭を取得しますが、カーソル移動・行の取得ともに折り返し行がまだ考慮できてないです。

また、開発時にデバッグ用のおまじないとして
`context->filename = (char*)(getLineHeadFromPositionY(context->text, context->cursor.position_y)->string); // TODO`
を書いたりしています。
自分のプログラムはコンソールをクリアしてしまうので、printfデバッグしようとしても消されてしまいます。
filenameはプログラムの行頭に表示する仕様(という名の現状動作)なので、そこに突っ込んでいますが
デバッグ用の仕組みを検討する必要がそろそろありそうですね。

# 今日のまとめ

- 文字挿入用に、カーソルの先頭行のポインタを検索できるようになりました

・・・進捗ダメです。
明日もゲームマーケットに行くので全然進まない気がします。申し訳ない。

昨日分に頂いたコメント(半角・全角の判別)は積み残しているので、やらないとですね。
文字コード周りの各種判定(後続文字判定含む)は、そこだけ切り出して別レポジトリにすると需要があったりするのでしょうか？
ないかな・・・。

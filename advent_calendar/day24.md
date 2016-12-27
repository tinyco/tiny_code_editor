本稿は[自作エディタをつくる Advent Calendar 2016](http://qiita.com/advent-calendar/2016/make_editor)の24日目です、レポジトリは[こちら](https://github.com/tinyco/tiny_code_editor)

# コピーペースト

contextと呼んでいる部分に可変長文字列のclip_boardを追加し、
そこに対してコピー・カット・ペースト操作を割り当てました。

キーバインドは以下です。

- Ctrl+X - cut selected string
- Ctrl+C - copy selected string
- Ctrl+V - paste string

あわせて、readmeも更新しました。

コードとしては

```
void cursor_copy_range(context *context) // PUBLIC;
{
  mutable_string_all_free(context->clip_board);
  context->clip_board = mutable_string_malloc();
  cursor c = cursor_sort_start_end(context->cursor);
  unum s = c.end_position_x - c.start_position_x + 1;
  unum i = 0;
  while (i < s) {
    uint byte;
    lines *head = lines_select_position_y(context->lines, c.start_position_y);
    mutable_string *ms = mutable_string_select_position_x(head->mutable_string, c.start_position_x + i, &byte);
    mutable_string_add_utf8char_to_tail(context->clip_board, &ms->string[byte]);
    i++;
  }
}

void cursor_paste_range(context *context) // PUBLIC;
{
  mutable_string *ms = context->clip_board;
  while (ms) {
    uint i = 0;
    while (i < ms->byte_count) {
      uint byte;
      lines *head = lines_select_position_y(context->lines, context->cursor.start_position_y);
      mutable_string *mutable_string =
          mutable_string_select_position_x(head->mutable_string, context->cursor.start_position_x + i, &byte);
      insert_utf8char(mutable_string, byte, &ms->string[i]);
      i++;
    }
    ms = ms->next;
  }
}
```

こんな感じで、操作する文字列が開いたファイルかクリップボードかは違いますが
やることは結構似ています。
もう少し良い感じの抽象度のメソッドを作れれば、DRYを守れて綺麗なメソッドになる気がします。


# 使いやすくする

- 終了時にコンソールをクリアする
- 範囲選択状態で文字入力した場合は先に選択範囲を削除する

等の、直感的でないと自分が思った動作を直していきます。

# バグ報告

equal-l2氏から、バグ報告を貰いました。
https://github.com/tinyco/tiny_code_editor/issues/7

改行直後等にINSERTすると、対象の文字列が空文字列ではなくNULLになっているようです。
いったん、NULLであれば空文字列をmallocするようにしましたが、
本来は改行等の際にmallocしていかないとまずそうですね・・・。
もう少し調査したいところ。


# 今日のまとめ

- copy-pasteできるようになりました

コピーペーストができるようになった記念に、verを0.0.2にしました。
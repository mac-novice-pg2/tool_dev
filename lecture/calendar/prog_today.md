# プログラミング編

ボンヤリながら、作ろうとするソフトウェアのイメージが出来てきました。ここでやっとプログラミングについて考える準備が出来たと言えるでしょう。では、ここからはプログラムを書く為の作業について説明してみます。なお、ここで説明したカレンダーソフトのソースコードについては、本書のあるディレクトリの/src以下に格納されてますので、適宜内容を確認してみて下さい。

## プログラムを書くには？

ここまで

「プログラムはコンピュータに作業を指示する為のもの」

と述べましたが、ここで具体的にコンピュータにどのように作業の指示を出すのか示してみます。

プログラムには３つの構成要素があって、これら３つの構成要素で全てのプログラムを記す事が出来るそうです(**エドガー・ダイクストラらが提唱した構造化プログラミング**より)

- 順次/連接
  - 一行ずつ順番に処理を指示していく事
- 分岐/選択
  - 特定の条件に従って、処理を分岐させる事
- 反復/繰り返し
  - 特定の条件を満たすまで、一定の処理を繰り返す事

## プログラムを書いてみる

そういえば、目的のソフトウェアを作るのにどのプログラミング言語を使うのか？をここまで指定してませんでしたが、古式ゆかしきC言語を用いる事にします。

ただ、この節で説明する**最初のプログラム**はあくまで上の**３大要素を使って、どうやって処理を書くのか？**を説明する事に集中する為、日本語を用いた疑似言語で示す事にします。

「プログラミング言語に何をチョイスするのか？」

というテーマは、それだけで議論と検討に値する大事なものですが、初学者に必要なのは**プログラムがどういうものか？**を理解する事のハズですね。



では、有名なFizzBuzz問題を疑似言語によって記載してみましょう。では、FizzBuzz問題の定義から

- 1から100までの数字の内、以下の条件の時に特定の処理を行いなさい
  - 3の倍数の時には、"Fizz"を出力
  - 5の倍数の時には、"Buzz"を出力
  - 3と5の公倍数の時には、"FizzBuzz"を出力

余談ですが、世界のナベアツのネタはこれの亜種で、3xの数字の時と5xの数字の時にも何らかのネタをやるので、実は問題の条件が違ってたりします。では、早速疑似言語で書いてみましょう

```
1.(順次)繰り返し用カウンタに1をセットする
2.(分岐)繰り返し用カウンタが3で割り切れるなら、"Fizz"を出力
3.(分岐)繰り返し用カウンタが5で割り切れるなら、"Buzz"を出力
4.(分岐)繰り返し用カウンタの値に応じて、以下の処理を分岐を行う
 4-a.(反復)繰り返しカウンタが100未満：繰り返しカウンタを1つ増やして、2からやり直す
 4-b.(順次)繰り返しカウンタが100：プログラムを終了する
```

プログラミング言語はそれぞれ表現力が異なる為、上に書いた処理をもっとスッキリ書ける事がありますが、大抵の言語の場合は上の処理をそのままプログラミング言語に置き換えるだけです。

## カレンダーソフトでの処理を考えてみる

では、疑似言語を使ってカレンダーソフトの処理を考えてみましょう。

ざっくりとですが、以下のような処理の順序になるのではないでしょうか？

1. 本日の日付を調べて表示する
2. 今月のカレンダーを表示する
3. 今月のイベントを表示する

「表示する」が３つ並んでますが、実際にはその中では以下のような処理が更に行われる事になるでしょう。

- ライブラリ関数を使って、今日の年月日を調べる
- 今月のカレンダーを作る為に必要なデータを作成する
  - そのデータに応じて、今月のカレンダーを作る
- 今月の日の中で、お知らせイベントに該当するものがあれば、出力する

### Today機能を書いてみる

全体のザックリとしたプログラミングの構造が見えてきたので、最初の処理である**Today機能**を書いてみましょう。「C言語 日付」ググってみた所以下のようなコードを見つける事が出来ました。

```cpp
#include <time.h>

int main( void )
{
    time_t timer;
    time( &timer );
    struct tm *date = localtime( &timer );
    printf( "Today:%s\n", asctime( date ) );
}
// Today:Mon Dec 31 01:57:49 2018
// が出力される
```

英語表記なのが気になりますが、要件は満たせたという事で一旦OKとしておきましょう
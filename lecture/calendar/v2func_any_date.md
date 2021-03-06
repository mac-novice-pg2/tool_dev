# Ver2)対応年月日拡大

Ver1は2019年限定になっていましたが、これは以下の理由によるものです

- 月末と開始曜日のテーブルが2019年分のしかなかった

これだけを考えると、単純にテーブルを拡大すれば解決出来そうな気がしますが、いちいち作成していたのではキリがありません。という訳で、改めて**任意の月のカレンダーを作る為に必要なもの**を考えてみましょう。

### 任意月のカレンダーを作るのに必要なもの

Ver1を作る上で、ある月のカレンダーを作成する時に必要だったものは以下の二点でした

1. その月の月末が何日か？
2. その月は何曜日から始まるか？

Ver1ではこれらの情報を構造体の配列として持つ事で解決していました。しかし、実はこれらの情報は以下の２つの情報が分かれば動的に導き出せます

- その月がうるう年か？
- ツェラーの公式によって年月日から曜日を導く

では、早速これらを関数として実装してみましょう。

### うるう年判定

[Microsoftが公開しているページ](https://support.microsoft.com/ja-jp/help/214019/method-to-determine-whether-a-year-is-a-leap-year)にて、以下のように紹介されています

1. 判定する年が 4 で割り切れる場合は手順 2. に進みます。割り切れない場合は手順 5. に進みます。
2. その年が 100 で割り切れる場合は手順 3. に進みます。割り切れない場合は手順 4. に進みます。
3. その年が 400 で割り切れる場合は手順 4. に進みます。割り切れない場合は手順 5. に進みます。
4. その年はうるう年です (この年は 366 日です)。
5. その年はうるう年ではありません (この年は 365 日です)。

これをこのまま関数として実装すれば良さそうです

```cpp
bool
Is_LeapYear( int year )
{
    bool judge = false;

    if( ( year % 4 ) == 0 )
    {
        if( ( year % 100 ) != 0 )
        {
            judge = true;
        }
        else
        {
            if( ( year % 400 ) == 0 )
            {
                judge = true;
            }
            else
            {
                judge = false;
            }
        }
    }

    return judge;
} // Is_LeapYear()
```

### ツェラーの公式で年月日から曜日を知る

年月日より曜日を求める事が出来るツェラーの公式というものがあります。詳細な説明はインターネットで調べて貰うとして、関数での実装は非常にシンプルです

```cpp
eWeekday
Formula_Zeller( int year, int month, int day )
{
    if( month < 3 )
    {
        year--;
        month += 12;
    }

    int temp = ( year + year / 4 - year / 100 + year / 400 + ( 13 * month + 8 ) / 5 + day ) % 7;

    return ( eWeekday )temp;
} // Formula_Zeller()
```

## 任意年月日の月末/開始曜日を知る

上の２つの情報を以下のように組み合わせる事で、Ver1で使った月情報を得られます。ここでは、基本的な考え方を説明するので実装方法を考えてみて下さい。実装例についてはサンプルプロジェクトをみて下さい

- 当該年がうるう年か否かで２月の月末日のみを変える
- 当該年の各月1に対してツェラーの公式を使用し、開始曜日を求める
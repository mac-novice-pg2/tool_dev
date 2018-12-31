# いちねんカレンダー機能を作る

Today機能を一から自分で作るのはあまりに難しいので、Google先生に頼ってみました。現在の日時情報を知るには、コンピュータにある日付情報をどうにかして取得する必要があります。こういった処理は、通常OSから提供される機能を使う事になるので、標準ライブラリ関数を使うのが一般的です。

次に作る機能であるいちねんカレンダー機能についてはどうでしょうか？少し考えてみます

#### カレンダーを作るには？

どうやればカレンダーを用意する事が出来るでしょう？こういう時は、

「カレンダーを作る為には、何が必要か？」

を考え見るとヒントが得られます。ここでは**どういった情報があれば、カレンダーを作成する事が出来る**のかを考えてみます。少し考え見たところ、どうやらカレンダーは以下のルールで成り立っているようです

- 月は1～12月までの12進数
- 日は必ず1日から始まるが、終わりは28～31日まである
- 曜日は年月日によって変わり、例えば2018年と2019年の1/1の曜日は異なる

これらの情報から、特定月のカレンダーを作成する場合に必要なものは

- その月が何日まであるのか？
- その月の1日は何曜日から始まるのか？

の二点が決まれば作成出来そうです。

なので以下の情報をプログラミング出来れば、特定月のカレンダーを作成出来そうです

- その月が何日まであるのか？
- その月は何曜日まであるのか？

では、早速これをデータとして定義してみましょう

### カレンダー情報型を定義する

| データ名 | 設定値制限                           | 型     |
| -------- | ------------------------------------ | ------ |
| 月末     | 28～31                               | 整数型 |
| 開始曜日 | "月"、"火"、"水"、"木"、"月"、"月"、 | 列挙型 |

月ごとに上のデータがあれば良さそうです。では、C言語でこれを表現してみます。

```cpp
// 曜日を示す列挙型
typedef enum
{
    Sun, // 日曜日
    Mon, // 月曜日
    Tue, // 火曜日
    Wed, // 水曜日
    Thu; // 木曜日
    Fri; // 金曜日
    Sat; // 土曜日
}eWeekday;

// カレンダーを作るのに必要な情報を示す構造体型
typdef struct{
    int      last; // その月の最終日
    eWeekday weekday; // 曜日
}Month_Info_t;
```

Month_Info_t型の情報を使えば、例えば2018/12であれば**土曜日から始まり、31日で終わる**ので、以下のようにデータを定義する事が出来ます

```cpp
Month_Info_t month_info_2018_12;
month_info_2018_12.weekday  = Sat; // 2018/12は土曜日から始まる
month_info_2018_12.last_day = 31;  // 2018/12は31日まで
```

さらに、この型を配列にすれば2019年データを定義する事が出来ます

```cpp
Month_Info_t month_info_2019[] = {
    { 31, Tue }, // 2019/01
    { 28, Fri }, // 2019/02
    { 31, Fri }, // 2019/03
    { 30, Mon }, // 2019/04
    { 31, Wed }, // 2019/05
    { 30, Sat }, // 2019/06
    { 31, Mon }, // 2019/07
    { 31, Thu }, // 2019/08
    { 30, Sun }, // 2019/09
    { 31, Tue }, // 2019/10
    { 30, Fri }, // 2019/11
    { 31, Sun }, // 2019/12
};
```

### プログラム例

完成形のコードを示します。

```cpp
static eWeekday
next_weekday( eWeekday current )
{
    switch( current )
    {
    case eSun: return eMon;
    case eMon: return eTue;
    case eTue: return eWed;
    case eWed: return eThu;
    case eThu: return eFri;
    case eFri: return eSat;
    case eSat: return eSun;
    default:   assert( 0 ); //ここに入ってくるとバグなので強制終
    }
} // next_weekday()

static const char*
weekday2string( eWeekday weekday )
{
    switch( weekday )
    {
    case eSun: return "日";
    case eMon: return "月";
    case eTue: return "火";
    case eWed: return "水";
    case eThu: return "木";
    case eFri: return "金";
    case eSat: return "土";
    default:   assert( 0 ); //ここに入ってくるとバグなので強制終了
    }
} // weekday2string()

void
print_calener( int month )
{
    Month_Info_t *pInfo = &( month_info_2019[ month - 1 ] );

    // 月の部分を出力
    printf( "%2d月のカレンダー\n\n", month );

    // 曜日部分を出力する
    for( int idx = 0; idx < eWeekEnd; idx++ )
        printf( "%s ", weekday2string( ( eWeekday )idx ) );
    printf( "\n" );

    // 日部分の出力位置合わせ
    eWeekday cur_weekday = pInfo->weekday;
    int pos = ( int )cur_weekday;
    for( int idx = 0; idx < pos; idx++ )
        printf( "   " );

    // 日部分を出力する
	for( int today = 1; today <= pInfo->last; today++, pos++ )
    {
    	if( pos && ( ( pos % 7 ) == 0 ) )
            printf( "\n" );
        printf( "%2d ", today );
    }
	printf( "\n" );
} // print_calener()
```

３つの関数から成りますが、上の二つは使う側はprint_calener()関数に月を渡す事でカレンダーが表示されます

```cpp
// static関数で、print_calener
static eWeekday next_weekday( eWeekday current );
static const char* weekday2string( eWeekday weekday );
void print_calener( int month )
```


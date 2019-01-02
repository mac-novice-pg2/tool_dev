# イベントお知らせ機能

いちねんカレンダーが完成しましたが、いかがでしたでしょうか？色々と考えるべき内容が盛り沢山で大変だったのではないでしょうか？次のイベントお知らせ機能は、カレンダーソフトの処理の中でも複雑なものになりますが、いちねんカレンダーが理解出来ればこちらについても分かるようになると思います。

## 機能イメージ

**イベントお知らせ機能**についての機能のイメージをもう少し具体化しましょう。

- 特定の日をお知らせする機能
  - ホントはカレンダー上に表示して欲しいけど、大変なので別枠で
- 特定の日時の定義
  - 定時退社日
    - 給料日(25日)
    - プレミアムフライデー
    - 長期連休前(3連休以上)
  - 祝日
  - プライベートイベント（飲み会など）

## 処理の流れを考える

処理の流れは概ね以下のようになるでしょうか。

- 当月にある日を順番にイベント条件と照らし合わせる
  - 一致した場合、その日を一致したイベントと一緒に出力する

## イベント条件判定

次に照らし合わせるイベント条件について、詳しく考えてみましょう

- 定時退社の判定
  - 25日かつ平日。25日が祝日の場合は直前の平日とする)
  - 3連休を超える連休直前の出勤日
  - 最終金曜日(プレミアムフライデー)
- 祝日/プライベートイベント
  - 祝日/プライベートイベント一覧表に合致するエントリがある場合

これらの内容を見て、

- 1日から月末までのイベント判定する為、定時退社となる日が休日だった場合、直前の出勤日を覚えておく必要がある。すなわち、直近の出勤日は常に保存しておく必要がある
- 祝日/プライベートイベント判定用テーブルを用意する

といった内容が必要になる事に思いつけたなら、かなり理解が進んでいると言えるでしょう

## データ構造

機能のイメージと処理の大まかな流れが見えてきた所で、プログラム設計に入りましょう。プログラムで最も重要なデータ構造について考えてみます。イベントお知らせ機能においては、

- 定時退社日
- 特定のイベント

の二つの判定が必要になりますが、いずれにおいても**当月分のイベント情報が必要になりそう**です。ここでは、イベント情報を以下のように定義しました

```cpp
typedef struct
{
    int day; // イベント日
    const char *event_name; // イベント名
    bool is_holiday; // true:休日 false:平日
}EventInfo;
```

| 変数名     | イベント名称 | 入力値例 |
| ---------- | ------------ | -------- |
| day        | イベント日   | １       |
| event_name | イベント名   | 元旦     |
| is_holiday | 休日か否か   | true     |

イベント情報は二次元配列として用意され、12月までの月に対して、それぞれのイベント情報のリストを格納します。月ごとの終端は、無効データを入れておく事でイベント終端を判定しています

```cpp
EventInfo
event_info_2019[][ EVENT_ITEM_MAX ] = {
    { // 2019/1
        { 1, "元旦", true },
        { 7, "七草の日", false },
        { 14,"成人の日", true },
        { EVENT_END,nullptr, false },
    },
    // 中略
        { // 2019/12
        { 23, "天皇誕生日？", true },
        { 24, "クリスマスイブ", false },
        { 25, "クリスマス", false },
        { 31, "大晦日", false },
        { EVENT_END,nullptr, false },
    },
};
```

### 必要そうな関数を準備しておく

イベントお知らせ機能を作ってみたのですが、冗長(似たような処理が幾つもある)な処理が多く出てきたので、そういった部分を関数化してコードをスッキリさせています。具体的には以下の処理が該当します

- 指定日がイベント情報リストに含まれるかどうかを判定する
- 指定日が休日かどうかを判定する

それぞれの関数の処理をコードと共に簡単に説明しておきます

### イベント日判定( check_event_day() )

引数で渡された日がイベント日かどうかを調べます。本館数では、イベント情報リストと照合して含まれているかどうか？を元にイベント日かどうかを返します。

- 有効なEventInfo*が返ってくる
  - 指定日はイベント日なので、返り値でイベント情報(へのポインタ)を返す
- nullptr
  - イベント日ではありません。返り値は無効ポインタです

具体的なコードは以下の通りです。引数がconst TodayInfo*となっているのは、この関数内で引数で渡されたデータが書き換えられない事を保証する為のものです。(constが無ければ、**引数で渡されたデータを関数内で破壊する事が出来てしまう**為、意思表示としてconstを付けるのがセオリーです)

```cpp
static EventInfo*
check_event_day( const TodayInfo *today )
{
    EventInfo *event = nullptr;
    for( int idx = 0; idx < EVENT_ITEM_MAX; idx++ )
    {
        event = &( event_info_2019[ today->month - 1 ][ idx ] );
        // イベント終端判定( 構造体メンバ全てが無効値を終端として扱う)
        if( ( event->day == EVENT_END ) &&
            ( event->event_name == nullptr ) &&
            ( event->is_holiday == false ) )
        {
            event = nullptr; // 上位へ該当なしを示す無効ポインタを返す
            break;
        }
        else if( event->day == today->day ) // イベントリストに指定日あり
        {
            break; // 現在のイベント情報を上位へ返す
        }
    }

    return event;
} // check_event_day()

    return event;
} // check_event_day()

```

### 休日判定( check_holiday() )

引数で渡された日が休日かどうかを判定します。といっても、判定基準はいい加減なもので、以下の条件に該当すれば、休日と判定させています

- 土日
- イベント情報リストを見て、休日指定されている

この関数でもイベント情報リストを参照する必要があるので、check_event_info()を使ってイベント情報を取り出しています。

```cpp
static bool
check_holiday( const TodayInfo *today )
{
    bool judgement = false;

    // 土日判定
    if( ( today->weekday == eSat ) &&
        ( today->weekday == eSun ) )
    {
        judgement = true;
    }
   
    // 祝日判定
    EventInfo *event = check_event_info( today );
    if( event != nullptr ) // 該当イベントが見つかった？
    {
        if( event->is_holiday )
        {
            judgement = true;
        }
    }

    return judgement;
} // is_holiday()
```

### イベント日の出力

指定月を11日から順番に調べて、check_event_day()でイベントリストに該当エントリが無いかを探して、該当イベントがある場合に詳細を表示します。**check_event_day()の返り値がnullptrでなければ、イベントリストに該当イベントがある**と判定しているのがポイントです。

```cpp
static void
print_event_alert( const TodayInfo *start, int eom )
{
    EventInfo *event;
    TodayInfo today = *start;
    for( int loop_count = 0; loop_count < eom; loop_count++ )
    {
        event = check_event_day( &today );
        if( event != nullptr ) // 該当イベントが見つかった？
        {
            printf( "%2d/%2dは%sです\n", today.month, today.day, event->event_name );
        }
        step_today_info( &today, eom ); // 1日進める
    }
    printf( "\n" );
} // print_event_alert()
```

### 定時退社日判定

本関数は、このソフトで最も複雑な判定処理を行っていると思います。考え方のポイントは、

- 連続する休日は、あくまで一つの連休として扱う。
  - is_cont_holidays変数がtrueの間が連休中と判定されます。これは**連続する休暇を一つの連休として扱う為**ものです。こうしないと、例えば連続休暇数だけで判定しようとすると**連続休暇数が3なら直前の出勤日を出力する**という分かりにくい判定になるのを避ける為です
  - 25日が給料日かつ平日の場合は定時退社判定
  - １日から順番にイベント判定を行う際に、平日の金曜日をlast_fridayへ保存する
    - 月末到達時点でのlast_fridayがプレミアムフライデーとなる

では、ソースコードです

```cpp
static void
print_no_overtime( const TodayInfo *start, int eom )
{
    TodayInfo today = *start;

    int bef_bussiness_day = today.day;
    bool is_cont_holidays = false; // 連休中判定
    bool is_holiday = false; // 連休中判定
    int cont_holidays = 0; // 連続休暇数
    int last_friday = NOT_FOUND;
    printf( "★定時退社日★\n" );
    for( int loop_count = 0; loop_count < eom; loop_count++ )
    {
        is_holiday = check_holiday( &today ); // 本日の休日判定
        if( is_holiday ) // 本日は休日？
        {
            is_cont_holidays = true; // 休暇中フラグOn
        }
        else // !is_holiday( =平日 )
        {
            // 3連休判定(連休終了時に直前の出勤日を出力する)
            if( is_cont_holidays )
            {
                if( cont_holidays >= 3 )
                {
                    printf( "%2d日\n", bef_bussiness_day );
                }
            }
            // 給料日での定時退社出力
            if( today.day == SALARY_DAY )
            {
                printf( "%2d日\n", today.day );
            }
            // プレミアムフライデー判定用に最終金曜日を保存しておく
            if( today.weekday == eFri )
            {
                last_friday = today.day;
            }
            is_cont_holidays = false; // 休暇中フラグOff
            bef_bussiness_day = today.day; // 直近の営業日を更新する
        }

        step_today_info( &today, eom ); // 1日進める
    }

    // プレミアムフライデー出力
    if( last_friday != NOT_FOUND )
    {
        printf( "%2d日\n", last_friday );
    }
} // print_no_overtime()
```


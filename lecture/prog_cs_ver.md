# C#バージョンの紹介

カレンダーソフトをC#に移植してみました。元のソースコードはC/C++で書かれていますが、C#になるとどうなるか？並べて紹介してみます。

言語仕様についてはあまり説明しませんが、言語的に近いので読めると思います。C#という言語は全ての要素がクラスに含まれる必要があるのですが、オブジェクト指向の考えは全く知らなくても読めると思います。

## C#版作成に際して

C#では、以下の３つの要素を導入してみました。

- 日本語名
- コレクションクラス
- foreach文

従来配列で持っていたデータを**コレクションクラス**と呼ばれるデータ構造に置き換えています。配列よりも非常に便利な機能を多く持っている為、多くの場合配列の代わりにこういったコレクションクラスを用いる方が良いです。

foreach文とは、配列やコレクションクラスといった一連のデータを一つずつ順番に処理する場合に使用される非常に便利な構文です。(C++にも部分的に導入されています)

## ヘッダファイル

C#にはヘッダーファイルという概念がない(そんなものを使う必要性がない)ので、C/C++版のみです

### [C/C++版]calendar_type.h

```cpp
#pragma once
/*
  ------------------------------------------
   マクロ定義
  ------------------------------------------
*/
#define SALARY_DAY     ( 25 )
#define NOT_FOUND      ( 0xFFFFFFFF )
#define EVENT_ITEM_MAX ( 8 )
#define EVENT_END      ( 0xFFFFFFFFU )

/*
  ------------------------------------------
   型定義
  ------------------------------------------
*/
// 曜日を示す列挙型
typedef enum
{
    eSun = 0, eMon, eTue, eWed, eThu, eFri, eSat, eWeekEnd,
}eWeekday;

// カレンダー作成用情報
typedef struct
{
    int      eom;     // 月末( end of month )
    eWeekday start_weekday; // 開始曜日
}MonthInfo;

// 本日を示す構造体
typedef struct
{
    int year;
    int month;
    int day;
    eWeekday weekday;
}TodayInfo;

typedef struct
{
    int day; // イベント日
    const char *event_name; // イベント名
    bool is_holiday; // true:休日 false:平日
}EventInfo;
```



## カレンダーデータ部

### [C/C++版]calendar_data.cpp

```cpp
#include "calendar_type.h"

MonthInfo eom_table[] = {
    // eom, start_weekday
    {  31,  eTue }, // 2019/01
    {  28,  eFri }, // 2019/02
    {  31,  eFri }, // 2019/03
    {  30,  eMon }, // 2019/04
    {  31,  eWed }, // 2019/05
    {  30,  eSat }, // 2019/06
    {  31,  eMon }, // 2019/07
    {  31,  eThu }, // 2019/08
    {  30,  eSun }, // 2019/09
    {  31,  eTue }, // 2019/10
    {  30,  eFri }, // 2019/11
    {  31,  eSun }, // 2019/12
};

EventInfo event_info_2019[][ EVENT_ITEM_MAX ] = {
    { // 2019/1
        // { day, event_name, is_holiday}
        { 1, "元旦", true },
        { 7, "七草の日", false },
        { 14,"成人の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/2
        // { day, event_name, is_holiday}
        { 3, "節分", false },
        { 4, "立春", false },
        { 11, "建国記念日", true },
        { 14, "バレンタインデー", false },
        { EVENT_END,nullptr, false },
    },
    { // 2019/3
        // { day, event_name, is_holiday}
        { 3, "ひな祭り", false },
        { 4, "立春", false },
        { 14, "ホワイトデー", false },
        { 21, "春分の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/4
        // { day, event_name, is_holiday}
        { 1, "エイプリルフール", false },
        { 29, "昭和の日", true },
        { EVENT_END,nullptr },
    },
    { // 2019/5
        // { day, event_name, is_holiday}
        { 1, "メーデー", false },
        { 3, "憲法記念日", true },
        { 4, "みどりの日", true },
        { 5, "こどもの日", true },
        { 6, "振替休日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/6
        // { day, event_name, is_holiday}
        { 1, "衣替え", false },
        { EVENT_END,nullptr, false },
    },
    { // 2019/7
        // { day, event_name, is_holiday}
        { 7, "七夕", false },
        { 15, "海の日", true },
        { 17, "祇園祭", false },
        { 25, "天神祭", false },
        { EVENT_END,nullptr, false },
    },
    { // 2019/8
        // { day, event_name, is_holiday}
        { 11, "山の日", true },
        { 12, "振替休日", true },
        { 15, "終戦記念日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/9
        // { day, event_name, is_holiday}
        { 15, "月見", false },
        { 16, "敬老の日", true },
        { 23, "秋分の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/10
        // { day, event_name, is_holiday}
        { 1, "衣替え", false },
        { 14, "体育の日", true },
        { 23, "秋分の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/11
        // { day, event_name, is_holiday}
        { 3, "文化の日", true },
        { 15, "七五三", false },
        { 23, "勤労感謝の日", true },
        { 28, "感謝祭", false },
        { EVENT_END,nullptr, false },
    },
    { // 2019/12
        // { day, event_name, is_holiday}
        { 23, "天皇誕生日？", true },
        { 24, "クリスマスイブ", false },
        { 25, "クリスマス", false },
        { 31, "大晦日", false },
        { EVENT_END,nullptr, false },
    },
};
```

### [C#版]calendar_data.cs

```C#
using System;
using System.Collections.Generic;

namespace Calendar_cs_ver
{
    // 曜日を示す列挙型
    public enum 曜日
    {
        日, 月, 火, 水, 木, 金, 土, 週末,
    }

    public class 月情報
    {
        public int 月末;     // 月末( end of month )
        public 曜日 開始曜日; // 開始曜日
        public 月情報(int 月末, 曜日 開始曜日)
        {
            this.月末 = 月末;
            this.開始曜日 = 開始曜日;
        }
    }

    public class 本日の情報
    {
        public int 年;
        public int 月;
        public int 日;
        public 曜日 曜日;
        public 本日の情報(int 年, int 月, int 日, 曜日 曜日)
        {
            this.年 = 年;
            this.月 = 月;
            this.日 = 日;
            this.曜日 = 曜日;
        }
    }

    public class イベント情報
    {
        public uint イベント日;
        public string イベント名;
        public bool 休日判定;
        public イベント情報(uint イベント日, string イベント名, bool 休日判定)
        {
            this.イベント日 = イベント日;
            this.イベント名 = イベント名;
            this.休日判定 = 休日判定;
        }
    }

    public class カレンダー情報
    {
        public static readonly int 給料日 = 25;
        public static List<月情報> 月情報テーブル = new List<月情報>
        {
            // 月末, 開始曜日
            new 月情報( 31, 曜日.火 ), // 2019/01
            new 月情報( 28, 曜日.金 ), // 2019/02
            new 月情報( 31, 曜日.金 ), // 2019/03
            new 月情報( 30, 曜日.月 ), // 2019/04
            new 月情報( 31, 曜日.水 ), // 2019/05
            new 月情報( 30, 曜日.土 ), // 2019/06
            new 月情報( 31, 曜日.月 ), // 2019/07
            new 月情報( 31, 曜日.木 ), // 2019/08
            new 月情報( 30, 曜日.日 ), // 2019/09
            new 月情報( 31, 曜日.火 ), // 2019/10
            new 月情報( 30, 曜日.金 ), // 2019/11
            new 月情報( 31, 曜日.日 ), // 2019/12
        };
        public static List<イベント情報>[] イベントリスト2019年版 = new List<イベント情報>[]
        {
            new List<イベント情報> // 2019/1
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報( 1, "元旦", true ),
                new イベント情報(  7, "七草の日", false ),
                new イベント情報(  14,"成人の日", true ),
            },
            new List<イベント情報> // 2019/1
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  3, "節分", false ),
                new イベント情報(  4, "立春", false ),
                new イベント情報(  11, "建国記念日", true ),
                new イベント情報(  14, "バレンタインデー", false ),
            },
            new List<イベント情報> // 2019/3
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  3, "ひな祭り", false ),
                new イベント情報(  4, "立春", false ),
                new イベント情報(  14, "ホワイトデー", false ),
                new イベント情報(  21, "春分の日", true ),
            },
            new List<イベント情報> // 2019/4
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報( 1, "エイプリルフール", false ),
                new イベント情報(  29, "昭和の日", true ),
            },
            new List<イベント情報> // 2019/5
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  1, "メーデー", false ),
                new イベント情報(  3, "憲法記念日", true ),
                new イベント情報(  4, "みどりの日", true ),
                new イベント情報(  5, "こどもの日", true ),
                new イベント情報(  6, "振替休日", true ),
            },
            new List<イベント情報> // 2019/6
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  1, "衣替え", false ),
            },
            new List<イベント情報> // 2019/7
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  7, "七夕", false ),
                new イベント情報(  15, "海の日", true ),
                new イベント情報(  17, "祇園祭", false ),
                new イベント情報(  25, "天神祭", false ),
            },
            new List<イベント情報> // 2019/8
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  11, "山の日", true ),
                new イベント情報(  12, "振替休日", true ),
                new イベント情報(  15, "終戦記念日", true ),
            },
            new List<イベント情報> // 2019/9
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  15, "月見", false ),
                new イベント情報(  16, "敬老の日", true ),
                new イベント情報(  23, "秋分の日", true ),
            },
            new List<イベント情報> // 2019/10
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  1, "衣替え", false ),
                new イベント情報(  14, "体育の日", true ),
                new イベント情報(  23, "秋分の日", true ),
            },
            new List<イベント情報> // 2019/11
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  3, "文化の日", true ),
                new イベント情報(  15, "七五三", false ),
                new イベント情報(  23, "勤労感謝の日", true ),
                new イベント情報(  28, "感謝祭", false ),
            },
            new List<イベント情報> // 2019/12
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  23, "天皇誕生日？", true ),
                new イベント情報(  24, "クリスマスイブ", false ),
                new イベント情報(  25, "クリスマス", false ),
                new イベント情報(  31, "大晦日", false ),
            },
        };
    }
}
```

### カレンダー処理部

### [C/C++版]Calendar.cpp

```cpp
#include <cstdio>
#include <cassert>
#include "calendar_type.h"

extern MonthInfo eom_table[];
extern EventInfo event_info_2019[][ EVENT_ITEM_MAX ];

static eWeekday next_weekday( eWeekday current )
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
    default:
        assert( 0 );
        return eWeekEnd;
    }
} // next_weekday()

// TodayInfo型データを一日進める関数
// 引数のeomはend of monthで月末の意
static void
step_today_info( TodayInfo *today, int eom ) //step_today_infoの処理■
{
    if( today->day != eom ) // 今日は月末でない？
        ( today->day )++; // 日を1日進める
    else // 今日は月末？
    {
        // 一か月進めて、1日に戻す
        // 年末なら、1年進めて1月に戻す
        today->day = 1;
        ( today->month )++;
        if( today->month == 13 ) // 年末？
        {
            today->month = 1;
            ( today->year )++;
        }
    }

    // 曜日を進める
    // next_weekday()という関数は、現在の曜日を渡すと、次の曜日を返す
    //  ex) next_weekday( eMon ) // eMonを渡すとeTueが返ってくる
    today->weekday = next_weekday( today->weekday );
} // step_today_info()

static EventInfo*
search_event_list( const TodayInfo *today )
{
    int event_list_index = 0;
    EventInfo *event = nullptr;
    while( event_list_index < EVENT_ITEM_MAX )
    {
        event = &( event_info_2019[ today->month - 1 ][ event_list_index ] );
        // イベント終端判定( 構造体メンバ全てが無効値のものをリスト終端として扱う )
        if( ( event->day == EVENT_END ) &&
            ( event->event_name == nullptr ) &&
            ( event->is_holiday == false ) )
        {
            event = nullptr; // 該当エントリなしを示す無効ポインタを返す
            break;
        }
        else if( event->day == today->day ) // イベントリストに指定日あり
            break; // 現在のイベント情報を上位へ返す
        event_list_index++; // イベントリストのインデックスを進める
    }
    return event;
} // search_event_list()

static void
print_skip_weekday( eWeekday start_weekday )//3
{
    for( int skip = 0; skip < start_weekday; skip++ )
        printf( "   " );
} // print_skip_weekday()

static void print_calendar( int year, int month )//2
{
    printf( "%d月のカレンダー\n", month );
    printf( "日 月 火 水 木 金 土\n" );

    // 1月1日がどこから(火曜日)始まることを意味してる
    // その月が何曜日から始まるのかは、eom_table配列の[ 月 ].start_weekdayメンバに入っている
    eWeekday start_weekday = eom_table[ month - 1 ].start_weekday;

    // 曜日の位置まで空白を詰める
    print_skip_weekday( start_weekday ); // 曜日の位置まで空白を詰める

    // 1日から始めて、月末までの日を表示する
    // 月末はeom_table配列の[ 月 ].eomメンバに入っている
    TodayInfo today = { year, month, 1, start_weekday };
    int eom = eom_table[ month - 1 ].eom;
    for( int loop_cnt = 0; loop_cnt < eom; loop_cnt++ )
    {
        // [書式指定文字列]
        // 数字の出力幅は2桁(%2d)分取り、スペース1つを出力する
        printf( "%2d ", today.day );
        if( today.weekday == eSat ) // 今日は土曜日
            printf( "\n" ); // 改行を入れる
        step_today_info( &today, eom );//■
    }
    printf( "\n\n" );
} // print_calendar()

void print_event( int year, int month )
{
    printf( "%d月のイベント日\n"
        "---------------------------\n",
        month );

    // 1月1日がどこから(火曜日)始まることを意味してる
    // その月が何曜日から始まるのかは、
    // eom_table配列の[ 月 ].start_weekdayメンバに入っている
    eWeekday start_weekday = eom_table[ month - 1 ].start_weekday;

    TodayInfo today = { year, month, 1, start_weekday };
    int eom = eom_table[ month - 1 ].eom;
    while( today.day < eom )
    {
        EventInfo *event = search_event_list( &today );
        if( event != nullptr ) // 該当イベントが見つかった？
            printf( "%2d/%2dは%sです\n", today.month, today.day, event->event_name );
        step_today_info( &today, eom ); // 1日進める
    }
    printf( "\n" );
} // print_event()

int main( int argc, const char* argv[] )// 1
{
    int year = 2019;
    for( int month = 1; month <= 12; month++ )
    {
        print_calendar( year, month ); // カレンダーを表示する
        print_event( year, month ); // イベントお知らせを表示する
        printf( "=================================\n" );
    }
}
```

### [C#版]CalendarMain.cs

```C#
using System;
using System.Collections.Generic;
using System.Text;

namespace Calendar_cs_ver
{
    class Program
    {
        static 曜日 次の曜日に進める( 曜日 現在の曜日 )
        {
            switch( 現在の曜日 )
            {
            case 曜日.日: return 曜日.月;
            case 曜日.月: return 曜日.火;
            case 曜日.火: return 曜日.水;
            case 曜日.水: return 曜日.木;
            case 曜日.木: return 曜日.金;
            case 曜日.金: return 曜日.土;
            case 曜日.土: return 曜日.日;
            default:
                Debug.Assert( false );
                return 曜日.週末;
            }
        } // 次の曜日に進める()

        static void 一日進める( 本日の情報 本日, int 月末 )
        {
            if( 本日.日 != 月末 )
                本日.日++;
            else // 今日は月末？
            {
                本日.日 = 1;
                本日.月++;
                if( 本日.月 == 13 ) // 年末？
                {
                    本日.月 = 1;
                    本日.年++;
                }
            }
            本日.曜日 = 次の曜日に進める( 本日.曜日 );
        } // 一日進める()

        static イベント情報 イベントリスト検索( 本日の情報 本日 )
        {
            イベント情報 検索結果 = null;
            foreach( var イベント in カレンダー情報.イベントリスト2019年版[ 本日.月 - 1 ] )
            {
                if( イベント.イベント日 == 本日.日 )
                    検索結果 = イベント;
            }
            return 検索結果;
        } // search_event_list()

        static uint 曜日からスキップ数を算出( 曜日 開始曜日 )
        {
            switch( 開始曜日 )
            {
            case 曜日.日: return 0;
            case 曜日.月: return 1;
            case 曜日.火: return 2;
            case 曜日.水: return 3;
            case 曜日.木: return 4;
            case 曜日.金: return 5;
            case 曜日.土: return 6;
            default:
                System.Diagnostics.Debug.Assert( false );
                return 0xdeadbeaf;
            }
        }

        static void 曜日読み飛ばし( 曜日 開始曜日 )
        {
            uint スキップ数 = 曜日からスキップ数を算出( 開始曜日 );
            for( uint ステップ = 0; ステップ < スキップ数; ステップ++ )
                Console.Write( "   " );
        }

        static void カレンダー表示( int 年, int 月 )
        {
            Console.WriteLine( "{0}月のカレンダー", 月 );
            Console.WriteLine( "日 月 火 水 木 金 土" );

            // 1月1日がどこから(火曜日)始まることを意味してる
            // その月が何曜日から始まるのかは、
            // CalendarData.月情報テーブル[ 月 - 1 ].開始曜日に入っている
            曜日 開始曜日 = カレンダー情報.月情報テーブル[ 月 - 1 ].開始曜日;

            曜日読み飛ばし( 開始曜日 ); // 曜日の位置まで空白を詰める

            // 1日から始めて、月末までの日を表示する
            // 月末はCalendarData.月情報テーブル[ 月 - 1 ].月末に入っている
            本日の情報 本日 = new 本日の情報( 年, 月, 1, 開始曜日 );
            int 月末 = カレンダー情報.月情報テーブル[ 月 - 1 ].月末;
            for( int ループ数 = 0; ループ数 < 月末; ループ数++ )
            {
                // [書式指定文字列]
                // 数字の出力幅は2桁分取り、スペース1つを出力する
                Console.Write( "{0:D2} ", 本日.日 );
                if( 本日.曜日 == 曜日.土 )
                    Console.WriteLine( "" ); // 改行を入れる
                一日進める( 本日, 月末 );
            }
            Console.WriteLine( "\n" );
        }

        static void
        イベント表示( int 年, int 月 )
        {
            Console.WriteLine( "{0:D2}月のイベント日\n", 月 );
            Console.WriteLine( "---------------------------" );

            // 1月1日がどこから(火曜日)始まることを意味してる
            // その月が何曜日から始まるのかは、
            // CalendarData.月情報テーブル[ 月 - 1 ].開始曜日メンバに入っている
            曜日 開始曜日 = カレンダー情報.月情報テーブル[ 月 - 1 ].開始曜日;

            本日の情報 本日 = new 本日の情報( 年, 月, 1, 開始曜日 );
            int 月末 = カレンダー情報.月情報テーブル[ 月 - 1 ].月末;
            while( 本日.日 < 月末 )
            {
                var イベント = イベントリスト検索( 本日 );
                if( イベント != null )
                    Console.WriteLine( "{0:D2}/{1:D2}は{2}です",
                                      本日.月, 本日.日, イベント.イベント名 );
                一日進める( 本日, 月末 );
            }
            Console.WriteLine();
        }

        static void Main(string[] args)
        {
            int 年 = 2019;
            for( int 月 = 1; 月 <= 12; 月++ )
            {
                カレンダー表示( 年, 月 );
                イベント表示( 年, 月 );
                Console.WriteLine( "=================================" );
            }
        }
    }
}
```




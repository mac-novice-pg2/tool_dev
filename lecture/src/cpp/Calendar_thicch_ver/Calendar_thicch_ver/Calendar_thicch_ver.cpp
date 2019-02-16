// Calendar_thicch_ver.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

#include <cstdio>
#include <cassert>

#include "calendar_type.h"

extern EventInfo event_info_2019[][ EVENT_ITEM_MAX ];

int Get_EndOfMonth( int year, int month )
{
    const int eom[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int ret = eom[ month - 1 ];

    if( month == 2 )
    {
        if( year % 4 == 0 ) 
        {
            if( year % 100 == 0 )
            {
                if( year % 400 == 0 )
                {
                    ret = 29;
                }
            }
            else
            {
                ret = 29;
            }
        }
    }

    return ret;
}

/*
  ------------------------------------------
   static関数
  ------------------------------------------
*/
// Zeller の公式で週の何日目か調べる
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

static eWeekday
next_youbi( eWeekday current )
{
    switch( current )
    {
    case eSun:
        return eMon;
    case eMon:
        return eTue;
    case eTue:
        return eWed;
    case eWed:
        return eThu;
    case eThu:
        return eFri;
    case eFri:
        return eSat;
    case eSat:
        return eSun;
    default:
        assert( 0 );
        return eWeekEnd;
    }
} // next_youbi()

// TodayInfo型データを一日進める関数
// 引数のeomはend of monthで月末の意
static void
step_today_info( TodayInfo *today, int eom ) //step_today_infoの処理■
{
    if( today->day != eom ) // 今日は月末でない？
    {
        ( today->day )++; // 日を1日進める
    }
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
    today->weekday = next_youbi( today->weekday );
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
        {
            break; // 現在のイベント情報を上位へ返す
        }
        event_list_index++; // イベントリストのインデックスを進める
    }
    return event;
} // search_event_list()

static void
print_skip_weekday( eWeekday start_weekday )//3
{
    // 頭の読み飛ばし
    /*
     eSun = 0,
     eMon = 1
     eTue = 2
     eWed = 3
     eThu = 4
     eFri = 5
     eSat = 6
    */

    // 2019年1月1日は火曜日
    // 従って日曜と月曜は不要なので、空白を入れてスキップする。
    // （1月1日の場合）0 < 2やから0と1の2セット分の空白3個分を入れる
    // 半角スペース3個 x 2セット = 合計6個のスペースが入る )
    for( int skip = 0; skip < start_weekday; skip++ )
    {
        printf( "   " );
    }
} // print_skip_weekday()

static void
print_calendar( int year, int month )//2
{
    printf( "%d月のカレンダー\n", month );
    printf( "日 月 火 水 木 金 土\n" );

    // 1月1日がどこから(火曜日)始まることを意味してる
    // その月が何曜日から始まるのかは、ツェラーの公式で取得する
    eWeekday start_weekday = Formula_Zeller( year,month,1 );

    // 曜日の位置まで空白を詰める
    print_skip_weekday( start_weekday ); // 曜日の位置まで空白を詰める

    // 1日から始めて、月末までの日を表示する
    // 月末はGet_EndOfMonth()関数で取得出来る
    TodayInfo today;
    today.year = year;
    today.month = month;
    today.day = 1;
    today.weekday = start_weekday;
    int eom = Get_EndOfMonth( year, month );
    for( int loop_cnt = 0; loop_cnt < eom; loop_cnt++ )
    {
        // [書式指定文字列]
        // 数字の出力幅は2桁(%2d)分取り、スペース1つを出力する
        printf( "%2d ", today.day );

        if( today.weekday == eSat ) // 今日は土曜日
        {
            printf( "\n" ); // 改行を入れる
        }
        step_today_info( &today, eom );//■
    }
    printf( "\n\n" );
} // print_calendar()

void
print_event( int year, int month )
{
    printf( "%d月のイベント日\n"
        "---------------------------\n",
        month );

    // 1月1日がどこから(火曜日)始まることを意味してる
    // その月が何曜日から始まるのかは、ツェラーの公式で取得する
    eWeekday start_weekday = Formula_Zeller( year, month, 1 );

    TodayInfo today;
    today.year = year;
    today.month = month;
    today.day = 1;
    today.weekday = start_weekday;
    int eom = Get_EndOfMonth(year,month);
    while( today.day < eom )
    {
        EventInfo *event = search_event_list( &today );
        if( event != nullptr ) // 該当イベントが見つかった？
        {
            printf( "%2d/%2dは%sです\n", today.month, today.day, event->event_name );
        }
        step_today_info( &today, eom ); // 1日進める
    }
    printf( "\n" );
} // print_event()

int
main( int argc, const char* argv[] )// 1
{
    int year = 2020;
    for( int month = 1; month <= 12; month++ )
    {
        // カレンダーを表示する
        print_calendar( year, month );

        // イベントお知らせを表示する
//        print_event( year, month );

        printf( "=================================\n" );
    }

    // getchar()はEnterキーを押すまで待つ
    printf( "Enterキーを押すと終了します" );
    getchar();
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

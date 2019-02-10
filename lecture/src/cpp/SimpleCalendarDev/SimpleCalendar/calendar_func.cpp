﻿#include "pch.h"

#include <cstdio>
#include <ctime>
#include <cassert>

#include <set> // std::set

#include "calendar.h"
#include "CalendarPrinter.h"
#include "CHolidayManager.h"

/*
  ------------------------------------------
   static関数
  ------------------------------------------
*/
static eWeekday
next_weekday( eWeekday current )
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
} // next_weekday()

// TodayInfo型データを一日進める関数
// 引数のeomはend of monthで月末の意
static void
step_today_info(
    DateInfo *today,
    int eom
)
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
            ( today->year )++;
            today->month = 1;
        }
    }

    // 曜日を進める
    today->weekday = next_weekday( today->weekday );
} // step_today_info()

static bool
check_holiday(
    const DateInfo *today,
    CHolidayManager *holiday )
{
    bool judgement = false;

    // 土日判定
    if( ( today->weekday == eSat ) ||
        ( today->weekday == eSun ) )
    {
        judgement = true;
    }
    else // 祝日判定
    {
        if( holiday->Search( today ) != nullptr )
        {
            judgement = true;
        }
    }
    return judgement;
} // check_holiday()

static void
print_no_overtime(
    const DateInfo *start,
    int eom,
    CalendarPrinter *printer,
    CHolidayManager *holiday
)
{
    DateInfo today = *start;
    std::set< int > result_days;

    int bef_bussiness_day = today.day;
    int cont_holidays = 0; // 連続休暇数
    int last_friday = NOT_FOUND;
    fprintf( printer->Output(), "★定時退社日★\n" );
    while( today.day < eom )
    {
        if( check_holiday( &today, holiday ) ) // 本日は休日？
        {
            //printf( "!!! debug !!![%2d] は休み\n",today.day );
            cont_holidays++; // 連続休日数をインクリメント
        }
        else // !is_holiday( = 平日 )
        {
            // 3連休判定(連休終了時に直前の出勤日を出力する)
            if( cont_holidays >= 3 ) // 3連休以上だった？
            {
                result_days.insert( bef_bussiness_day );
            }
            cont_holidays = 0; // 連続休日数カウントをクリア
            // 給料日の定時退社判定
            if( today.day == SALARY_DAY )
            {
                result_days.insert( today.day );
            }
            // 後のプレミアムフライデー判定用に最終金曜日を保存
            if( today.weekday == eFri )
            {
                last_friday = today.day;
            }
            bef_bussiness_day = today.day; // 直近の営業日を更新する
        }
        step_today_info( &today, eom ); // 1日進める
    }

    // プレミアムフライデー判定
    if( last_friday != NOT_FOUND )
    {
        result_days.insert( last_friday );
    }

    for( auto day : result_days )
    {
        fprintf( printer->Output(), "%2d日\n", day );
    }
} // print_no_overtime()

static void
print_holiday(
    const DateInfo *start,
    int eom,
    CalendarPrinter *printer,
    CHolidayManager *holiday )
{
    DateInfo today = *start; // 引数をローカル変数にコピー
    while( today.day < eom )
    {
        const CHolidayInfo *e = holiday->Search( &today );
        if( e != nullptr ) // 該当イベントが見つかった？
        {
            fprintf( printer->Output(), "%2d/%2dは%sです\n",
                today.month, today.day, e->event_name_.c_str() );
        }
        step_today_info( &today, eom ); // 1日進める
    }
    fprintf( printer->Output(), "\n" );
} // print_event_alert()

/*
  ------------------------------------------
   API関数
  ------------------------------------------
*/
void
PrintToday( CalendarPrinter *printer )
{
    time_t timer;

    time( &timer );
    struct tm *date = localtime( &timer );
    fprintf( printer->Output(), "Today:%s\n", asctime( date ) );
} // PrintToday()

void
PrintCalendar(
    int year,
    int month,
    CalendarPrinter *printer,
    MonthInfo month_info[]
)
{
    fprintf( printer->Output(),
        "%4d年%2d月のカレンダー\n"
        "\n"
        "日 月 火 水 木 金 土\n",
        year, month );
    
    // カレンダー情報テーブルから、指定年月のテーブルを引く
    MonthInfo *pInfo = &( month_info[ month - 1 ] );

    // 日部分の出力位置合わせ
    for( int skip = 0; skip < ( int )pInfo->start_weekday; skip++ )
    {
        fprintf( printer->Output(), "   " );
    }

    // 日部分を出力する
    DateInfo today = { year, month, 1, pInfo->start_weekday };
    for( int day = 0; day < pInfo->eom; day++ )
    {
        fprintf( printer->Output(), "%2d ", today.day );
        // 土曜日まで出力したら、改行して折り返す
        if( today.weekday == eSat )
        {
            fprintf( printer->Output(), "\n" );
        }
        step_today_info( &today, pInfo->eom ); // 1日進める
    }
    fprintf( printer->Output(), "\n" );
} // PrintCalendar()

void
PrintEventAlert(
    int year,
    int month,
    CalendarPrinter *printer,
    MonthInfo month_info[],
    CHolidayManager *holiday
)
{
    fprintf( printer->Output(), 
        "\n"
        "=======================================\n"
        " イベント情報 \n"
        "=======================================\n"
    );

    // カレンダー/イベント情報初期化
    MonthInfo *pInfo = &( month_info[ month - 1 ] );
    DateInfo today = { year, month, 1, pInfo->start_weekday };

    // 祝日出力
    print_holiday( &today, pInfo->eom, printer, holiday );

    // 定時退社日チェック
    print_no_overtime( &today, pInfo->eom, printer, holiday );
} // EventAlert()

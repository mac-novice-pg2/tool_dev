#include "pch.h"

#include <cstdio>
#include <ctime>
#include <cassert>

#include "calendar.h"

// 外部参照
extern MonthInfo month_info_2019[];
extern EventInfo event_info_2019[][ 8 ];

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
    }
} // next_weekday()

static const char*
weekday2string( eWeekday weekday )
{
    switch( weekday )
    {
    case eSun:
        return "日";
    case eMon:
        return "月";
    case eTue:
        return "火";
    case eWed:
        return "水";
    case eThu:
        return "木";
    case eFri:
        return "金";
    case eSat:
        return "土";
    default:
        assert( 0 );
    }
} // weekday2string()

// TodayInfo型データを一日進める関数
// 引数のeomはend of monthで月末の意
static void
StepTodayInfo( TodayInfo *today_info, int eom )
{
    if( today_info->day != eom ) // 月末の場合、月を一つ進める
    {
        ( today_info->day )++;
    }
    else
    {
        today_info->day = 1; // 1日に戻す
        ( today_info->month )++; // 月を1つ進める
        if( today_info->month == 13 ) // 年末の場合は更に年を進める
        {
            ( today_info->year )++;
            today_info->month = 1; // 1月に戻す
        }
    }

    // 曜日を進める
    today_info->weekday = next_weekday( today_info->weekday );
} // StepTodayInfo()
static void
judge_event( int month, int today )
{
    EventInfo *event;
    for( int idx = 0; idx < EVENT_ITEM_MAX; idx++ )
    {
        event = &( event_info_2019[ month - 1 ][ idx ] );
        if( ( event->day == EVENT_END ) &&
            ( event->event_name == nullptr ) )
        {
            break;
        }
        if( event->day == today )
        {
            printf( "%2d月%2dは%sです\n", month, today, event->event_name );
        }
    }
} // judge_event()

/*
  ------------------------------------------
   API関数
  ------------------------------------------
*/
void
PrintToday( void )
{
    time_t timer;

    time( &timer );
    struct tm *date = localtime( &timer );
    printf( "Today:%s\n", asctime( date ) );
} // print_today()

void
PrintCalendar( int month )
{
    MonthInfo *pInfo = &( month_info_2019[ month - 1 ] );

    // 月の部分を出力
    printf( "%2d月のカレンダー\n\n", month );

    // 曜日部分を出力する
    for( int idx = 0; idx < eWeekEnd; idx++ )
    {
        printf( "%s ", weekday2string( ( eWeekday )idx ) );
    }
    printf( "\n" );

    // 日部分の出力位置合わせ
    for( int idx = 0; idx < ( int )pInfo->weekday; idx++ )
    {
        printf( "   " );
    }

    // 日部分を出力する
    TodayInfo today;
    today.year = 2019;
    today.month = month;
    today.day = 1;
    today.weekday = pInfo->weekday;
    for( int loop_count = 0; loop_count < pInfo->last; loop_count++ )
    {
        printf( "%2d ", today.day );
        // 土曜日まで出力したら、改行して折り返す
        if( today.weekday == eSat )
        {
            printf( "\n" );
        }
        StepTodayInfo( &today, pInfo->last ); // 1日進める
    }
	printf( "\n" );
} // PrintCalendar()

void
EventAlert( int month )
{
    MonthInfo *pInfo = &( month_info_2019[ month - 1 ] );

    printf( "\n"
        "=======================================\n"
        " イベント情報 \n"
        "=======================================\n"
    );
    // 日部分を出力する
    int bef_workday;
    eWeekday cur_weekday = pInfo->weekday;
    for( int today = 1; today <= pInfo->last; today++ )
    {
        judge_event( month, today );
        cur_weekday = next_weekday( cur_weekday );
    }
    printf( "\n" );
} // EventAlert()

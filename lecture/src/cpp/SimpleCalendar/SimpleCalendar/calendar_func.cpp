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
    eWeekday cur_weekday = pInfo->weekday;
    int pos = ( int )cur_weekday;
    for( int idx = 0; idx < pos; idx++ )
    {
        printf( "   " );
    }

    // 日部分を出力する
	for( int today = 1; today <= pInfo->last; today++, pos++ )
    {
    	if( pos && ( ( pos % 7 ) == 0 ) )
        {
            printf( "\n" );
        }
        printf( "%2d ", today );
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

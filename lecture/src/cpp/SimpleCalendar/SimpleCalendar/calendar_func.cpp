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
        return eWeekEnd;
    }
} // next_weekday()

static const char*
weekday_to_string( eWeekday weekday )
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
step_today_info( TodayInfo *today_info, int eom )
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

static EventInfo*
check_event_info( const TodayInfo *today )
{
    EventInfo *event = nullptr;
    for( int idx = 0; idx < EVENT_ITEM_MAX; idx++ )
    {
        event = &( event_info_2019[ today->month - 1 ][ idx ] );
        if( ( event->day == EVENT_END ) &&
            ( event->event_name == nullptr ) )
        {
            event = nullptr;
            break;
        }
        else if( event->day == today->day )
        {
            break;
        }
    }

    return event;
} // check_event()

static bool
is_holiday( const TodayInfo *today )
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
        if( event->holiday )
        {
            judgement = true;
        }
    }

    return judgement;
} // is_holiday()

static void
print_no_overtime( const TodayInfo *start, int eom )
{
    TodayInfo today = *start;

    int bef_bussiness_day = today.day;
    bool isHolidays = false; // 連休中判定
    int cont_holidays = 0; // 連続休暇数
    int last_friday = NOT_FOUND;
    printf( "★定時退社日★\n" );
    for( int loop_count = 0; loop_count < eom; loop_count++ )
    {
        if( is_holiday( &today ) ) // 本日は休日？
        {
            isHolidays = true; // 休暇中フラグOn
            if( today.day == SALARY_DAY ) // 給料日？
            {
                printf( "%2d日\n", bef_bussiness_day );
            }
        }
        else // 平日
        {
            // 3連休判定(連休終了時に直前の出勤日を出力する)
            if( isHolidays )
            {
                if( cont_holidays >= 3 )
                {
                    printf( "%2d日\n", bef_bussiness_day );
                }
            }
            if( today.day == SALARY_DAY )
            {
                printf( "%2d日\n", today.day );
            }
            // プレミアムフライデー判定
            if( today.weekday == eFri )
            {
                last_friday = today.day;
            }
            isHolidays = false; // 休暇中フラグOff
            bef_bussiness_day = today.day; // 直近の営業日を更新する
        }
        step_today_info( &today, eom ); // 1日進める
    }
    if( last_friday != NOT_FOUND )
    {
        printf( "%2d日\n", last_friday );
    }
} // print_no_overtime()

static void
print_event_alert( const TodayInfo *start, int eom )
{
    EventInfo *event;
    TodayInfo today = *start;
    for( int loop_count = 0; loop_count < eom; loop_count++ )
    {
        event = check_event_info( &today );
        if( event != nullptr ) // 該当イベントが見つかった？
        {
            printf( "%2d/%2dは%sです\n", today.month, today.day, event->event_name );
        }
        step_today_info( &today, eom ); // 1日進める
    }
    printf( "\n" );
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
PrintCalendar( int year, int month )
{
    printf(
        "%4d年%2d月のカレンダー\n"
        "\n"
        "日 月 火 水 木 金 土\n",
        year, month );
    
    // カレンダー情報テーブルから、指定年月のテーブルを引く
    MonthInfo *pInfo = &( month_info_2019[ month - 1 ] );

    // 日部分の出力位置合わせ
    for( int idx = 0; idx < ( int )pInfo->weekday; idx++ )
    {
        printf( "   " );
    }

    // 日部分を出力する
    TodayInfo today = { year, month, 1, pInfo->weekday };
    for( int loop_count = 0; loop_count < pInfo->eom; loop_count++ )
    {
        printf( "%2d ", today.day );
        // 土曜日まで出力したら、改行して折り返す
        if( today.weekday == eSat )
        {
            printf( "\n" );
        }
        step_today_info( &today, pInfo->eom ); // 1日進める
    }
	printf( "\n" );
} // PrintCalendar()

void
PrintEventAlert( int year, int month )
{
    printf( "\n"
        "=======================================\n"
        " イベント情報 \n"
        "=======================================\n"
    );

    // カレンダー/イベント情報初期化
    MonthInfo *pInfo = &( month_info_2019[ month - 1 ] );
    TodayInfo today = { year, month, 1, pInfo->weekday };

    // 定時退社日チェック
    print_no_overtime( &today, pInfo->eom );

    // イベントお知らせ出力
    print_event_alert( &today, pInfo->eom );
} // EventAlert()

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
check_event_day( const TodayInfo *today )
{
    EventInfo *event = nullptr;
    for( int idx = 0; idx < EVENT_ITEM_MAX; idx++ )
    {
        event = &( event_info_2019[ today->month - 1 ][ idx ] );
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
    }
    return event;
} // check_event_day()

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
    EventInfo *event = check_event_day( today );
    if( event != nullptr ) // 該当イベントが見つかった？
    {
        if( event->is_holiday ) // 該当日は祝日？
        {
            judgement = true;
        }
    }
    return judgement;
} // check_holiday()

static void
print_no_overtime( const TodayInfo *start, int eom )
{
    TodayInfo today = *start;

    int bef_bussiness_day = today.day;
    bool is_cont_holidays = false; // 連休中判定
    bool is_holiday = false; // 本日の休日判定
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
        else // !is_holiday( = 平日 )
        {
            // 3連休判定(連休終了時に直前の出勤日を出力する)
            if( is_cont_holidays )
            {
                if( cont_holidays >= 3 ) // 3連休以上だった？
                {
                    printf( "%2d日\n", bef_bussiness_day );
                }
            }
            // 給料日の定時退社判定
            if( today.day == SALARY_DAY )
            {
                printf( "%2d日\n", today.day );
            }
            // 後のプレミアムフライデー判定用に最終金曜日を保存
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

    // イベントお知らせ出力
    print_event_alert( &today, pInfo->eom );

    // 定時退社日チェック
    print_no_overtime( &today, pInfo->eom );
} // EventAlert()

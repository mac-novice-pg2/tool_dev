#include "pch.h"

#include <cstdio>
#include <ctime>
#include <cassert>
#include <set> // std::set

#include "calendar_func.h"
#include "MenuControl.h"
#include "CEventManager.h"

/*
  ------------------------------------------
   static関数
  ------------------------------------------
*/
static bool
check_holiday( DateInfo *today, CEventManager& event_manager )
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
        EvnetInfoList event_list = event_manager.GetMonthEvent( today );
        const CEventInfo e = event_manager.Search( event_list, today );
        if( e.IsValid() && e.bHoliday_ )
        {
            judgement = true;
        }
    }
    return judgement;
} // check_holiday()

static void
print_no_overtime( DateInfo *start )
{
    CEventManager holiday( "holiday.csv" );
    int eom = Cal_GetEndOfMonth( start );
    DateInfo cur = *start;
    std::set< int > result_days;

    int bef_bussiness_day = cur.day;
    int cont_holidays = 0; // 連続休暇数
    int last_friday = NOT_FOUND;
    printf( "★定時退社日★ : " );
    while( cur.day < eom )
    {
        if( check_holiday( &cur, holiday ) ) // 本日は休日？
        {
            //printf( "!!! debug !!![%2d] は休み\n",today->day );
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
            if( cur.day == SALARY_DAY )
            {
                result_days.insert( cur.day );
            }
            // 後のプレミアムフライデー判定用に最終金曜日を保存
            if( cur.weekday == eFri )
            {
                last_friday = cur.day;
            }
            bef_bussiness_day = cur.day; // 直近の営業日を更新する
        }
        Cal_StepDateInfo( &cur, eom ); // 1日進める
    }

    // プレミアムフライデー判定
    if( last_friday != NOT_FOUND )
    {
        result_days.insert( last_friday );
    }

    for( int day : result_days )
    {
        printf( "%2d日 ", day );
    }
    printf( "\n" );
} // print_no_overtime()

static void
print_holiday( DateInfo *start )
{
    CEventManager holiday( "holiday.csv" );

    int eom = Cal_GetEndOfMonth( start );
    DateInfo cur = *start; // 引数をローカル変数にコピー
    printf( "今月の祝日\n" );
    EvnetInfoList cur_month_event = holiday.GetMonthEvent( &cur );
    while( cur.day < eom )
    {
        const CEventInfo& e = holiday.Search( cur_month_event, &cur );
        if( e.IsValid() ) // 該当イベントが見つかった？
        {
            printf( "%2d/%2dは%sです\n",
                cur.month, cur.day, e.name_.c_str() );
        }
        Cal_StepDateInfo( &cur, eom ); // 1日進める
    }
    printf( "\n" );
} // print_event_alert()

static void
print_event_day( DateInfo *start )
{
    CEventManager event_manager( "event.csv" );
    printf( "今月のイベント\n" );
    EvnetInfoList event_list = event_manager.GetMonthEvent( start );
    if( !event_list.empty() )
    {
        for( CEventInfo item : event_list )
        {
            printf( "%4d/%2d/%2d %s\n",
                item.date_.year, item.date_.month, item.date_.day,
                item.name_.c_str() );
        }
    }
    printf( "\n" );
} // print_event_day()

static void
print_moon_age( int day_of_week, double moon_age )
{
    for( int idx = 0; idx < day_of_week; idx++ )
    {
        printf( "%s|", Cal_GetMoonName( moon_age ) );
        moon_age = ( int )( moon_age + 1 ) % 30;
    }
    printf( "\n-------------------------------------------------\n" );
} // print_moon_age()

static void
skip_start_position( eWeekday start_weekday )
{
    const char* skip_spaces = "      |";
    // 日部分の出力位置合わせ
    for( int skip = 0; skip < ( int )start_weekday; skip++ )
    {
        printf( skip_spaces );
    }
} // skip_start_position()

void
PrintToday( void )
{
    char str[ 256 ];
    time_t timer = time( NULL );
    strftime( str, 255, "%Y/%m/%d %a %H:%M:%S", localtime( &timer ) );
    printf( "%s\n", str );
} // PrintToday()

void
PrintCalendar( DateInfo *date )
{
    char eto_str[ 32 ];

    Cal_CalcEto( date->year, eto_str );
    printf(
        "%4d年%2d月(%s年)のカレンダー\n"
        "  日  |  月  |  火  |  水  |  木  |  金  |  土  |\n"
        "-------------------------------------------------\n"
        ,
        date->year, date->month, eto_str );

    // カレンダー情報テーブルから、指定年月のテーブルを引く
    DateInfo first_day;
    first_day = *date;
    first_day.day = 1;
    eWeekday start_weekday = Cal_FormulaZeller( &first_day );

    // 日部分の出力位置合わせ
    skip_start_position( start_weekday );

    // 日部分を出力する
    int eom = Cal_GetEndOfMonth( date );
    double cur_moon_age = Cal_CalcMoonAge( &first_day );
    DateInfo today;
    today = first_day;
    today.weekday = start_weekday;
    bool is_first_week = true;
    int days_of_week = 0;
    for( int day = 0; day < eom; day++ )
    {
        days_of_week++;
        printf( "  %2d  |", today.day );
        // 土曜日まで出力したら、改行して折り返す
        if( today.weekday == eSat )
        {
            printf( "\n" );
            // 月齢出力
            if( is_first_week ) // 初週分のスキップ処理
                skip_start_position( start_weekday );
            print_moon_age( days_of_week, cur_moon_age );
            cur_moon_age = ( int )( cur_moon_age + days_of_week ) % 30;
            days_of_week = 0;
            is_first_week = false;
        }
        Cal_StepDateInfo( &today, eom ); // 1日進める
    }
    printf( "\n" );
    print_moon_age( days_of_week, cur_moon_age );
} // PrintCalendar()

void
PrintEvent( DateInfo *date )
{
    // 定時退社日チェック
    // カレンダー/イベント情報初期化
    DateInfo first_day = *date;
    first_day.day = 1;
    print_no_overtime( &first_day );

    print_holiday( &first_day );
    print_event_day( &first_day );
} // PrintEventAlert()

int main()
{
    DateInfo date;
    Cal_InitDateInfo( &date );

    Menu_Main( &date );
    bool apl_end = false;
    while( !apl_end )
    {
        PrintCalendar( &date );
        Menu_ShowInstruction();
//        PrintEvent( &date );
        apl_end = Menu_ChangeCalendar( &date );
    }
} // main()

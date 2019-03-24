#include "pch.h"

#include <cstdio>
#include <ctime>
#include <cassert>

#include <set> // std::set

#include "calendar.h"
#include "CMonthInfo.h"
#include "CEventManager.h"

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
step_today_info( DateInfo& today, int eom )
{
    if( today.day != eom ) // 今日は月末でない？
    {
        ( today.day )++; // 日を1日進める
    }
    else // 今日は月末？
    {
        // 一か月進めて、1日に戻す
        // 年末なら、1年進めて1月に戻す
        today.day = 1;
        ( today.month )++;
        if( today.month == 13 ) // 年末？
        {
            ( today.year )++;
            today.month = 1;
        }
    }

    // 曜日を進める
    today.weekday = next_weekday( today.weekday );
} // step_today_info()

static bool
check_holiday(
    const DateInfo& today,
    CEventManager& event_manager )
{
    bool judgement = false;

    // 土日判定
    if( ( today.weekday == eSat ) ||
        ( today.weekday == eSun ) )
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
print_moon_age( int day_of_week, double moon_age )
{
    for( int idx = 0; idx < day_of_week; idx++ )
    {
        printf( "%s|", CMonthInfo::Convert_MoonName( moon_age ) );
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

/*
  ------------------------------------------
   API関数
  ------------------------------------------
*/
void
PrintToday( void )
{
    char str[ 256 ];
    time_t timer = time( NULL );
    strftime( str, 255, "%Y/%m/%d %a %H:%M:%S", localtime( &timer ) );
    printf( "%s\n", str );
} // PrintToday()

void
PrintCalendar( const DateInfo &date )
{
    char eto_str[ 32 ];

    CMonthInfo::Calc_ETO( date.year, eto_str );
    printf(
        "%4d年%2d月(%s年)のカレンダー\n"
        "  日  |  月  |  火  |  水  |  木  |  金  |  土  |\n"
        "-------------------------------------------------\n"
        ,
        date.year, date.month, eto_str );
    
    // カレンダー情報テーブルから、指定年月のテーブルを引く
    DateInfo cur = CMonthInfo::GetFirstOfTheMonth( date.year, date.month );
    int eom = CMonthInfo::GetEndOfMonth( cur );
    eWeekday start_weekday = CMonthInfo::Formula_Zeller( cur );

    // 日部分の出力位置合わせ
    skip_start_position( start_weekday );

    // 日部分を出力する
    double cur_moon_age = CMonthInfo::Calc_MoonAge( cur );
    bool is_first_week = true;
    int days_of_week = 0;
    for( int day = 0; day < eom; day++ )
    {
        days_of_week++;
        printf( "  %2d  |", cur.day );
        // 土曜日まで出力したら、改行して折り返す
        if( cur.weekday == eSat )
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
        step_today_info( cur, eom ); // 1日進める
    }
    printf( "\n" );
    print_moon_age( days_of_week, cur_moon_age );
} // PrintCalendar()

void
PrintNoOvertime( const DateInfo& date )
{
    CEventManager holiday( "holiday.csv" );
    DateInfo cur = CMonthInfo::GetFirstOfTheMonth( date.year, date.month );
    int eom = CMonthInfo::GetEndOfMonth( cur );
    std::set< int > result_days;

    int bef_bussiness_day = cur.day;
    int cont_holidays = 0; // 連続休暇数
    int last_friday = NOT_FOUND;
    printf( "★定時退社日★ : " );
    while( cur.day < eom )
    {
        if( check_holiday( cur, holiday ) ) // 本日は休日？
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
        step_today_info( cur, eom ); // 1日進める
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
} // PrintNoOvertime()

void
PrintHoliday( const DateInfo &date )
{
    CEventManager holiday( "holiday.csv" );

    // 処理の開始点となる月初を作成
    DateInfo cur = CMonthInfo::GetFirstOfTheMonth( date.year, date.month );
    int eom = CMonthInfo::GetEndOfMonth( cur );
    printf( "今月の祝日\n" );
    EvnetInfoList cur_month_event = holiday.GetMonthEvent( cur );
    while( cur.day < eom )
    {
        const CEventInfo& e = holiday.Search( cur_month_event, cur );
        if( e.IsValid() ) // 該当イベントが見つかった？
        {
            printf( "%2d/%2dは%sです\n",
                cur.month, cur.day, e.name_.c_str() );
        }
        step_today_info( cur, eom ); // 1日進める
    }
    printf( "\n" );
} // PrintHoliday()

void
PrintEventDay( const DateInfo &date )
{
    DateInfo start( date );
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
} // PrintEventDay()


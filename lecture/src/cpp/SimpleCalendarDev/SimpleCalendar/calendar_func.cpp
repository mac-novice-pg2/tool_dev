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
step_today_info( DateInfo *today, int eom )
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
check_holiday( const DateInfo *today, CEventManager *holiday )
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
print_no_overtime( const DateInfo *start, int eom, CEventManager *holiday )
{
    DateInfo today = *start;
    std::set< int > result_days;

    int bef_bussiness_day = today.day;
    int cont_holidays = 0; // 連続休暇数
    int last_friday = NOT_FOUND;
    printf( "★定時退社日★\n" );
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
        printf( "%2d日\n", day );
    }
} // print_no_overtime()

static void
print_holiday( const DateInfo *start, int eom, CEventManager *holiday )
{
    DateInfo today = *start; // 引数をローカル変数にコピー
    while( today.day < eom )
    {
        const CEventInfo *e = holiday->Search( &today );
        if( e != nullptr ) // 該当イベントが見つかった？
        {
            printf( "%2d/%2dは%sです\n", today.month, today.day, e->event_name_.c_str() );
        }
        step_today_info( &today, eom ); // 1日進める
    }
    printf( "\n" );
} // print_event_alert()

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
PrintCalendar( int year, int month )
{
    printf(
        "%4d年%2d月(%s年)のカレンダー\n"
        "\n"
        "  日  |  月  |  火  |  水  |  木  |  金  |  土  |\n"
        "-------------------------------------------------\n"
        ,
        year, month, CMonthInfo::Calc_ETO( year ) );
    
    // カレンダー情報テーブルから、指定年月のテーブルを引く
    eWeekday start_weekday = CMonthInfo::Formula_Zeller( year, month, 1 );

    // 日部分の出力位置合わせ
    skip_start_position( start_weekday );

    // 日部分を出力する
    int eom = CMonthInfo::GetEndOfMonth( year, month );
    double cur_moon_age = CMonthInfo::Calc_MoonAge( year, month, 1 );
    DateInfo today = { year, month, 1, start_weekday };
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
        step_today_info( &today, eom ); // 1日進める
    }
    printf( "\n" );
    print_moon_age( days_of_week, cur_moon_age );
} // PrintCalendar()

void
PrintEventAlert( int year, int month, CEventManager *event )
{
    printf( 
        "\n"
        "=======================================\n"
        " イベント情報 \n"
        "=======================================\n"
    );

    // カレンダー/イベント情報初期化
    eWeekday start_weekday = CMonthInfo::Formula_Zeller( year, month, 1 );
    DateInfo today = { year, month, 1, start_weekday };

    // 祝日出力
    int eom = CMonthInfo::GetEndOfMonth( year, month );
    print_holiday( &today, eom, event );

    // 定時退社日チェック
    print_no_overtime( &today, eom, event );
} // EventAlert()

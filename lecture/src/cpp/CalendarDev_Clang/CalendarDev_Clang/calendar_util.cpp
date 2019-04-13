#include "pch.h"

#include <string>
#include <cassert>

#include "calendar_func.h"

void
Cal_ConvJapaneseEraName( int year, char *result )
{
    int calc_year;
    EraNameType wareki_tbl[] = {
        { 2019, "令和" },
        { 1989, "平成" },
        { 1926, "昭和" },
        { 1912, "大正" },
        { 1868, "明治" },
    };

    for( int i = 0; i < ARRAY_SIZE( wareki_tbl ); i++ )
    {
        // 引数の西暦がどの和暦に該当するか調べる
        if( year >= wareki_tbl[ i ].first_year )
        {
            /*
             西暦とその西暦が含まれる和暦の元年の差分を求める
              例) 2000年(平成)の差分を算出する
               2000年 - 1989年(平成元年) = 11年(平成元年からの経過年)
             更に1が起点なので + 1する
            */
            calc_year = year - wareki_tbl[ i ].first_year + 1;
            if( calc_year == 1 ) // 和暦1年目は元年表記にする
            {
                sprintf( result, "%s元年", wareki_tbl[ i ].era_name );
            }
            else
            {
                sprintf( result, "%s%2d年", wareki_tbl[ i ].era_name, calc_year );
            }
            break;
        }
    }
} // Cal_ConvJapaneseEraName()

eWeekday
Cal_GetNextWeekday( eWeekday current )
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
} // Cal_GetNextWeekday()

eWeekday
Cal_FormulaZeller( DateInfo *date )
{
    DateInfo tmp = *date;
    if( tmp.month < 3 )
    {
        tmp.year--;
        tmp.month += 12;
    }

    int temp = ( tmp.year
        + tmp.year / 4
        - tmp.year / 100
        + tmp.year / 400
        + ( 13 * tmp.month + 8 )
        / 5 + tmp.day ) % 7;

    return ( eWeekday )temp;
} // Cal_FormulaZeller()

DateInfo
Cal_GetFirstOfMonth( int year, int month )
{
    DateInfo date;
    date.year = year;
    date.month = month;
    date.day = 1;
    date.weekday = Cal_FormulaZeller( &date );
    return date;
} // Cal_GetFirstOfMonth()

int
Cal_GetEndOfMonth( DateInfo *date )
{
    int eom;
    const int eom_table[] = { // 月末テーブル
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    eom = eom_table[ date->month - 1 ];
    if( ( Cal_IsLeapYear( date->year ) ) && // うるう年？
        ( date->month == 2 ) // 2月？
        )
    {
        eom++; // うるう年なので1日足す
    }
    return eom;
} // Cal_GetEndOfMonth()

bool
Cal_IsLeapYear( int year )
{
    bool judge = false;

    if( ( year % 4 ) == 0 )
    {
        if( ( year % 100 ) != 0 )
        {
            judge = true;
        }
        else
        {
            judge = ( year % 400 ) ? false : true;
        }
    }
    return judge;
} // Cal_IsLeapYear()

double
Cal_CalcMoonAge( DateInfo *date )
{
    double val = (
        ( ( date->year - 2009 ) % 19 ) * 11
        + date->month
        + date->day ) % 30;
    if( date->month <= 2 )
    {
        val += 2;
    }
    return val;
} // Cal_CalcMoonAge()

const char*
Cal_GetMoonName( double moon_age )
{
    int age = ( int )moon_age;

    if( age <= 1 )
        return " 新月 ";
    else if( age <= 2 )
        return " 繊月 ";
    else if( age <= 6 )
        return "三日月";
    else if( age <= 8 )
        return " 上弦 ";
    else if( age <= 12 )
        return "十日夜";
    else if( age <= 13 )
        return "十三夜";
    else if( age <= 14 )
        return "小望月";
    else if( age <= 15 )
        return " 満月 ";
    else if( age <= 16 )
        return "十六夜";
    else if( age <= 17 )
        return "立待月";
    else if( age <= 18 )
        return "居待月";
    else if( age <= 19 )
        return "寝待月";
    else if( age <= 21 )
        return "更待月";
    else if( age <= 23 )
        return " 下弦 ";
    else if( age <= 29 )
        return "有明月";
    else if( age <= 30 )
        return "三十日";
    else
        assert( 0 );
    return 0;
} // Cal_GetMoonName()

void
Cal_CalcEto( int year, char *eto_str )
{
    const char *eto_string[] = {
        "申","酉","戌","亥","子","丑","寅","卯","辰","巳","午","未"
    };
    int index = year % ARRAY_SIZE( eto_string );
    strcpy( eto_str, eto_string[ index ] );
} // Cal_CalcEto()

void
Cal_StepDateInfo( DateInfo *today, int eom )
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
    today->weekday = Cal_GetNextWeekday( today->weekday );
} // Cal_StepDateInfo()

void
Cal_InitDateInfo( DateInfo *date )
{
    date->year = 2000;
    date->month = 1;
    date->day = 1;
    date->weekday = eSun;
} // Cal_InitDateInfo()

EventInfo*
Cal_CreateEventTable( const char *holiday_filename, int *tbl_size )
{
    EventInfo *tbl;
    FILE *fp = fopen( holiday_filename, "rt" );
    if( fp == NULL )
    {
        printf( "休日ファイルがオープン出来ません" );
        exit( 1 );
    }

    // 祝日数を先にカウントしておく
    char read_buf[ 256 ];
    int event_count = 0;
    EventInfo *cur_pos;
    while( !feof( fp ) )
    {
        fgets( read_buf, sizeof( read_buf ), fp );
        event_count++;
    }
    tbl = ( EventInfo* )malloc( sizeof( EventInfo ) * event_count );

    // ファイルポインタを先頭まで巻き戻す
    rewind( fp );

    char date[ 64 ];
    char name[ 64 ];
    EventInfo *cur;
    for( int idx = 0; idx < event_count; idx++ )
    {
        cur = &( tbl[ idx ] );
        fgets( read_buf, sizeof( read_buf ), fp );
        // 日付/祝日名取り出し
        //  ファイルからの入力：1950/1/1,元日,
        //   "1950/1/1" -> date
        //   "元旦"     -> name
        sscanf( read_buf, "%[^,],%[^,],", date, cur->event_name );
        sscanf( date, "%d/%d/%d",
            &( cur->date.year ), &( cur->date.month ), &( cur->date.day ) );
        cur->is_holiday = true;
        cur->is_valid = true;
    }
    fclose( fp );

    *tbl_size = event_count;
    return tbl;
} // Cal_CreateEventTable()

EventInfo*
Cal_GetCurEventTable(
    DateInfo *date,
    EventInfo *base_tbl,
    int base_tbl_size,
    int *out_tbl_size )
{
    bool found = false;
    EventInfo *cur;
    int tbl_start;
    int tbl_end;
    *out_tbl_size = 0;
    for( int idx = 0; idx < base_tbl_size; idx++ )
    {
        cur = &( base_tbl[ idx ] );
        if( !found )
        {
            if( ( date->year == cur->date.year ) &&
                ( date->month == cur->date.month ) )
            {
                tbl_start = idx;
                found = true;
            }
        }
        else
        {
            if( ( date->year != cur->date.year ) ||
                ( date->month != cur->date.month ) ) // 月替わりを検出？
            {
                *out_tbl_size = idx - tbl_start;
                break;
            }
        }
    }

    if( !found )
        return NULL;
    return &( base_tbl[ tbl_start ] );
} // Cal_GetCurEventTable()

int
Cal_SearchEventTable(
    DateInfo *date,
    EventInfo *tbl,
    int tbl_size
)
{
    EventInfo *cur;
    for( int idx = 0; idx < tbl_size; idx++ )
    {
        if( ( date->year == tbl[ idx ].date.year ) &&
            ( date->month == tbl[ idx ].date.month ) &&
            ( date->day == tbl[ idx ].date.day ) )
        {
            return idx;
        }
    }
    return 0xFFFFFFFF; // 見つからなかった場合の値
} // Cal_SearchEventTable()
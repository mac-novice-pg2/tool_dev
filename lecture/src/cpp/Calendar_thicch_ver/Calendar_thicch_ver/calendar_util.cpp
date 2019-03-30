#include "pch.h"

#include <cassert>

#include "calendar_def.h"
#include "calendar_func.h"

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

int Cal_GetEndOfMonth( DateInfo *date )
{
    const int eom[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int ret = eom[ date->month - 1 ];

    if( date->month == 2 )
    {
        if( date->year % 4 == 0 )
        {
            if( date->year % 100 == 0 )
            {
                if( date->year % 400 == 0 )
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
    // @@@ !!!ここを作って下さい
} // Cal_CalcEto()

// TodayInfo型データを一日進める関数
// 引数のeomはend of monthで月末の意
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

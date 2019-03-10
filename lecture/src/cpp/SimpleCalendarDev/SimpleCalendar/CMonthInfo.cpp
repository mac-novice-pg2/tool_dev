#include "pch.h"

#include <vector>
#include <string>
#include <cassert>

#include "calendar.h"
#include "CMonthInfo.h"

using string_container_t = std::vector< std::string >;

double
CMonthInfo::Calc_MoonAge( int year, int month, int day )
{
    double val = ( ( ( year - 2009 ) % 19 ) * 11 + month + day ) % 30;
    if( month <= 2 )
    {
        val += 2;
    }
    return val;
} // CMonthInfo::Calc_MoonAge()

const char*
CMonthInfo::Calc_ETO( int year )
{
    char buf[ 256 ];
    string_container_t eto_string =
    {
        "申","酉","戌","亥","子","丑","寅","卯","辰","巳","午","未"
    };
    int index = year % eto_string.size();
    strcpy( buf, eto_string[ index ].c_str() );
    return buf;
} // CMonthInfo::Calc_ETO()

const char*
CMonthInfo::Convert_MoonName( double moon_age )
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
        return "十日夜";
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
} // Convert_MoonName()

bool
CMonthInfo::Is_LeapYear( int year )
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
} // CMonthInfo::Is_LeapYear()

// Zeller の公式で週の何日目か調べる
eWeekday
CMonthInfo::Formula_Zeller( int year, int month, int day )
{
    if( month < 3 )
    {
        year--;
        month += 12;
    }

    int temp = ( year + year / 4 - year / 100 + year / 400 + ( 13 * month + 8 ) / 5 + day ) % 7;

    return ( eWeekday )temp;
} // CMonthInfo::Formula_Zeller()

int
CMonthInfo::GetEndOfMonth( int year, int month )
{
    const int eom_[] =     { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    const int eom_leap[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int eom;

    if( Is_LeapYear( year ) )
    {
        eom = eom_leap[ month - 1 ];
    }
    else
    {
        eom = eom_[ month - 1 ];
    }
    return eom;
} // CMonthInfo::GetEndOfMonth()
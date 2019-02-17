#include "pch.h"

#include "calendar.h"
#include "CMonthInfo.h"

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
CMonthInfo::Formula_Zeller(
    int year,
    int month,
    int day
)
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
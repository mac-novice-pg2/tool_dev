#include "pch.h"

#include "calendar.h"
#include "CMonthInfo.h"

CMonthInfo::CMonthInfo()
{
    const int eom_normal_year[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    const int eom_leap_year[]   = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    for( int year = 0; year < END_OF_YEAR; year++ )
    {
        const int *eom_table = Is_LeapYear( year + 1 ) ? eom_leap_year : eom_normal_year;
        for( int month = 0; month < END_OF_MONTH; month++ )
        {
            info_[ year ][ month ].eom = eom_table[ month ];
            info_[ year ][ month ].start_weekday = Formula_Zeller( year + 1, month + 1, 1 );
        }
    }
} // CMonthInfo::CMonthInfo()

MonthInfo*
CMonthInfo::GetMonthInfoTable( int year )
{
    return info_[ year - 1 ];
} // CMonthInfo::GetMonthInfoTable()

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
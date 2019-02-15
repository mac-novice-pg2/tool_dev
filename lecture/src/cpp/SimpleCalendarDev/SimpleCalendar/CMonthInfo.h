#pragma once


class CMonthInfo
{
public:
    static bool Is_LeapYear( int year );
    static eWeekday Formula_Zeller( int year, int month, int day );
    static int GetEndOfMonth( int year, int month );
};

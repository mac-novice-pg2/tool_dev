#pragma once


class CMonthInfo
{
public:
    double Calc_MoonAge( int year, int month, int day );
    const char* Calc_ETO( int year );
    static bool Is_LeapYear( int year );
    static eWeekday Formula_Zeller( int year, int month, int day );
    static int GetEndOfMonth( int year, int month );
};

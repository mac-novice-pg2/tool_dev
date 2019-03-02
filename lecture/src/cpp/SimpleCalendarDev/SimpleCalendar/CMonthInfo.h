#pragma once


class CMonthInfo
{
public:
    static double Calc_MoonAge( int year, int month, int day );
    static const char* Calc_ETO( int year );
    static const char* Convert_MoonName( double moon_age );
    static bool Is_LeapYear( int year );
    static eWeekday Formula_Zeller( int year, int month, int day );
    static int GetEndOfMonth( int year, int month );
};

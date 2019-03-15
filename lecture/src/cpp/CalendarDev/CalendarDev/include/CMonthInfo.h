#pragma once

class CMonthInfo
{
public:
    static double Calc_MoonAge( const DateInfo &date );
    static void Calc_ETO( int year, char* eto_str );
    static const char* Convert_MoonName( double moon_age );
    static bool Is_LeapYear( int year );
    static eWeekday Formula_Zeller( const DateInfo &date );
    static int GetEndOfMonth( const DateInfo &date );
};

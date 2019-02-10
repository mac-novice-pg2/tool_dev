#pragma once

class CMonthInfo
{
    MonthInfo info_[ END_OF_YEAR ][ END_OF_MONTH ];

public:
    CMonthInfo();
    virtual ~CMonthInfo();

    MonthInfo* GetMonthInfoTable( int year );

    static bool Is_LeapYear( int year );
    static eWeekday Formula_Zeller( int year, int month, int day );
};

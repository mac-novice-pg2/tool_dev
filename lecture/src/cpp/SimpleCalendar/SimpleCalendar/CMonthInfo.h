#pragma once

#include "calendar.h"

static const int END_OF_YEAR = 9999;
static const int END_OF_MONTH = 12;

class CMonthInfo
{

    MonthInfo month_info[ END_OF_YEAR ][ END_OF_MONTH ];
     
public:
    CMonthInfo();
    virtual ~CMonthInfo();

    MonthInfo* GetMonthInfoTable( int year );
    static bool Is_LeapYear( int year );
    static eWeekday Formula_Zeller( int year, int month, int day );
};


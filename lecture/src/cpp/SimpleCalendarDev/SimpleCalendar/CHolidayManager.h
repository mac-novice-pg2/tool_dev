#pragma once

#include <vector>
#include <string>

#include "calendar.h"

class CHolidayInfo
{
public:
    DateInfo date_;
    std::string event_name_;

    CHolidayInfo();

    bool IsMatch( const DateInfo& date ) const;
};

class CHolidayManager
{
    using HolidayList = std::vector< CHolidayInfo >;
    const char *HOLIDAY_FILENAME = "holiday.csv";

    HolidayList list_[ END_OF_YEAR ];

    CHolidayInfo read_event_info( const char *input ) const;

public:
    CHolidayManager();

    const CHolidayInfo*
    Search( const DateInfo* search_date ) const;
};
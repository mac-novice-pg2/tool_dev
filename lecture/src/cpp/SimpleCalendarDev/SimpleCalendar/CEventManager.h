#pragma once

#include <vector>
#include <string>

#include "common_func.h"
#include "calendar.h"

class CEventInfo
{
public:
    DateInfo date_;
    std::string event_name_;
    bool is_holiday_;

    CEventInfo();

    bool IsMatch( const DateInfo& date ) const;
};

class CEventManager
{
    using HolidayList = std::vector< CEventInfo >;
    const char *HOLIDAY_FILENAME = "holiday.csv";

    HolidayList list_[ END_OF_YEAR ];

    CEventInfo read_event_info( string_container entry ) const;

public:
    CEventManager();

    const CEventInfo*
    Search( const DateInfo* search_date ) const;
};
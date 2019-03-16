#pragma once

#include <climits>

#include <vector>
#include <string>

#include "common_func.h"
#include "calendar.h"

struct CEventInfo
{
    DateInfo date_;
    std::string name_;
    bool bHoliday_;
    bool valid_;

    CEventInfo();
    bool IsMatch( const DateInfo& date ) const;
    bool IsValid( void ) const;
};

struct EventCountInfo_t
{
    int start_index;
    int year;
    int count;
    EventCountInfo_t( void );
    bool IsEmpty();
};

using event_count_t = std::vector< EventCountInfo_t >;

struct EventListIndex_t
{
    int min_;
    int max_;

    event_count_t item_;
    EventListIndex_t( void );
};

class CEventManager
{
    using event_list_t = std::vector< CEventInfo >;

    event_list_t event_;
    EventListIndex_t idx_info_;
    CEventInfo invalid_data_;

    virtual CEventInfo read_info( string_container entry ) const;

public:
    CEventManager( const char *filename );

    const CEventInfo&
    Search( const DateInfo& search_date ) const;
};

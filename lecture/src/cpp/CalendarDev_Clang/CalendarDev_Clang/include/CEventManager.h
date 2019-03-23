#pragma once

#include <climits>

#include <vector>
#include <string>

#include "calendar_def.h"
#include "CSV_Reader.h"

struct CEventInfo
{
    DateInfo date_;
    std::string name_;
    bool bHoliday_;
    bool valid_;

    CEventInfo();
    bool IsMatch( DateInfo *date ) const;
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

struct EventListIndex_t
{
    int min_;
    int max_;

    std::vector< EventCountInfo_t > item_;
    EventListIndex_t( void );
};

using EvnetInfoList = std::vector< CEventInfo >;
class CEventManager
{
    std::vector< CEventInfo > event_;
    EventListIndex_t idx_info_;
    CEventInfo invalid_data_;

    CEventInfo read_info( string_container entry ) const;

public:
    CEventManager( const char *filename );

    static const CEventInfo
    Search( EvnetInfoList list, DateInfo *search_date );

    EvnetInfoList GetMonthEvent( DateInfo *date );
};

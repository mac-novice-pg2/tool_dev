#pragma once

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

class CEventManager
{
    using event_list_t = std::vector< CEventInfo >;

    event_list_t list_;
    CEventInfo invalid_data_;

    virtual CEventInfo read_info( string_container entry ) const;

public:
    CEventManager( const char *filename );

    const CEventInfo&
    Search( const DateInfo& search_date ) const;
};

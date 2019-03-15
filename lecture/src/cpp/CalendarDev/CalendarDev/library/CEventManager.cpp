#include "pch.h"

#include <exception>
#include "common_func.h"

#include "CSV_Reader.h"
#include "CEventManager.h"
#include "CMonthInfo.h"

CEventInfo
CEventManager::read_info( string_container entry ) const
{
    if( entry.size() != 3 )
        throw std::invalid_argument::exception();

    CEventInfo ret;
    char buf[ 256 ];

    // 年月日取り出し
    auto date_str = Split( entry[ 0 ], '/' );
    sprintf( buf, "%s %s %s",
        date_str[ 0 ].c_str(), date_str[ 1 ].c_str(), date_str[ 2 ].c_str() );
    sscanf( buf, "%d %d %d",
        &( ret.date_.year ), &( ret.date_.month ), &( ret.date_.day ) );
    ret.date_.weekday = CMonthInfo::Formula_Zeller( ret.date_ );

    // イベント名/祝日・平日情報取り出し
    ret.name_ = entry[ 1 ];
    ret.bHoliday_ = strcmp( entry[ 2 ].c_str(), "祝日" );
    ret.valid_ = true;

    return ret;
} // CHolidayManager::read_info()

CEventManager::CEventManager( const char *filename )
{
    CSV_Reader csv( filename );
    auto buf = csv.Readline();
    while( !buf.empty() )
    {
        auto info = read_info( buf );
        list_.push_back( info );
        buf = csv.Readline();
    }
    invalid_data_.valid_ = false;
} // CHolidayManager::CHolidayManager()

const CEventInfo&
CEventManager::Search( const DateInfo& date ) const
{
    for( size_t i = 0; i < list_.size(); i++ )
    {
        if( list_[ i ].IsMatch( date ) )
            return list_[ i ];
    }
    return invalid_data_;
} // CHolidayManager::Search()

CEventInfo::CEventInfo()
    : name_( "" ), bHoliday_( false )
{
    date_.year = 0;
    date_.month = 0;
    date_.day = 0;
    date_.weekday = eWeekEnd;
    name_ = "";
    bHoliday_ = false;
    valid_ = false;
}

bool
CEventInfo::IsMatch( const DateInfo& check ) const
{
    if( ( this->date_.year == check.year ) &&
        ( this->date_.month == check.month ) &&
        ( this->date_.day == check.day ) )
    {
        return true;
    }
    else
    {
        return false;
    }
} // HolidayInfo::IsMatch()

bool
CEventInfo::IsValid( void ) const
{
    return valid_;
}

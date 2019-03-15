#include "pch.h"

#include "common_func.h"

#include "CSV_Reader.h"
#include "CEventManager.h"
#include "CMonthInfo.h"

CEventInfo
CEventManager::read_event_info( string_container tokens ) const
{
    char buf[ 256 ];

    // スラッシュで年月日取り出し
    string_container date_str = Split( tokens[ 0 ], '/' );
    sprintf( buf, "%s %s %s",
        date_str[ 0 ].c_str(), date_str[ 1 ].c_str(), date_str[ 2 ].c_str() );

    CEventInfo ret;
    sscanf( buf, "%d %d %d",
        &( ret.date_.year ), &( ret.date_.month ), &( ret.date_.day ) );
    ret.date_.weekday = CMonthInfo::Formula_Zeller(
        ret.date_.year, ret.date_.month, ret.date_.day
    );

    // イベント名取り出し
    if( tokens.size() >= 2 )
        ret.event_name_ = tokens[ 1 ];

    if( tokens.size() >= 3 )
    {
        ret.is_holiday_ = false;
        if( strcmp( tokens[ 2 ].c_str(), "祝日" ) == 0  )
            ret.is_holiday_ = true;
    }

    return ret;
} // CHolidayManager::read_event_info()

CEventManager::CEventManager()
{
    CSV_Reader csv( HOLIDAY_FILENAME );
    string_container buf = csv.Readline();
    while( !buf.empty() )
    {
        auto info = read_event_info( buf );
        list_[ info.date_.year ].push_back( info );
        buf = csv.Readline();
    }
} // CHolidayManager::CHolidayManager()

const CEventInfo*
CEventManager::Search( const DateInfo* today ) const
{
    for( size_t i = 0; i < list_[ today->year ].size(); i++ )
    {
        if( list_[ today->year ][ i ].IsMatch( *today ) )
            return &( list_[ today->year ][ i ] );
    }

    return nullptr;
} // CHolidayManager::Search()

CEventInfo::CEventInfo()
{
    date_.year = 0;
    date_.month = 0;
    date_.day = 0;
    event_name_ = "";
} // CEventInfo::CEventInfo()

bool
CEventInfo::IsMatch( const DateInfo& check ) const
{
    if( ( this->date_.year == check.year ) &&
        ( this->date_.month == check.month ) &&
        ( this->date_.day == check.day ) &&
        ( this->date_.weekday == check.weekday ) )
    {
        return true;
    }
    else
    {
        return false;
    }
} // HolidayInfo::IsMatch()

#include "pch.h"

#include <algorithm>
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
    CEventInfo info;
    EventCountInfo_t cur_index;

    size_t entry_count = 0;
    int cur_year = 0;
    auto buf = csv.Readline();

    while( !buf.empty() )
    {
        info = read_info( buf );
        event_.push_back( info );
        entry_count++;

        // イベント年数の範囲設定
        idx_info_.min_ = std::min( idx_info_.min_, info.date_.year );
        idx_info_.max_ = std::max( idx_info_.max_, info.date_.year );

        if( cur_year == info.date_.year )
            cur_index.count++;
        else
        {
            // 初回エントリは空なのでスキップする
            if( !cur_index.IsEmpty() )
                idx_info_.item_.push_back( cur_index );

            // 次年度イベント情報を設定
            cur_index.start_index = entry_count - 1;
            cur_index.year = info.date_.year;
            cur_index.count = 1;
        }
        cur_year = info.date_.year;
        buf = csv.Readline();
    }
    invalid_data_.valid_ = false;
} // CHolidayManager::CHolidayManager()

const CEventInfo&
CEventManager::Search( const DateInfo& date ) const
{
    if( ( date.year < idx_info_.min_ ) ||
        ( date.year > idx_info_.max_ ) )
        return invalid_data_;

    auto idx_info = idx_info_.item_[ date.year - idx_info_.min_ ];
    auto search_start = idx_info.start_index;
    int start_month = 0;
    for( int i = 0; i < idx_info.count; i++ )
    {
        // 当月を超えたら、探索打ち切り
        if( ( event_[ search_start + i ].date_.month ) > date.month )
            break;

        if( event_[ search_start + i ].IsMatch( date ) )
            return event_[ search_start + i ];
    }
    return invalid_data_;
} // CHolidayManager::Search()

EvnetInfoList
CEventManager::GetMonthEvent( const DateInfo & date )
{
    auto idx_info = idx_info_.item_[ date.year - idx_info_.min_ ];
    int search_start = idx_info.start_index;

    bool found = false;
    int start, end, cur;
    start = end = INT_MAX;
    for( int i = 0; i < idx_info.count; i++ )
    {
        cur = search_start + i;
        if( event_[ cur ].date_.month == date.month )
        {
            found = true;
            if( start == INT_MAX )
                start = cur;
            end = cur;
        }
    }

    if( found )
        return EvnetInfoList( &event_[ start ], &event_[ end + 1 ] );
    else
        return EvnetInfoList();
} // CEventManager::GetMonthEvent()

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

EventCountInfo_t::EventCountInfo_t( void )
    : start_index( 0 ), year( 0 ), count( 0 )
{
} // event_count_info_t::event_count_info_t()

bool
EventCountInfo_t::IsEmpty()
{
    return (
        ( start_index == 0 ) &&
        ( year == 0 ) &&
        ( count == 0 ) );
} // event_count_info_t::IsEmpty()

EventListIndex_t::EventListIndex_t( void )
    : min_( INT_MAX ), max_( INT_MIN )
{
}

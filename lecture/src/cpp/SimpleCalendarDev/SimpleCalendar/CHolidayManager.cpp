#include "pch.h"

#include <sstream>

#include "CHolidayManager.h"
#include "CMonthInfo.h"

using string_container = std::vector< std::string >;

string_container
split(
    const std::string &work,
    char splitter
)
{
    string_container result;
    std::stringstream ss( work );
    std::string token;
    while( getline( ss, token, splitter ) )
    {
        if( !token.empty() )
            result.push_back( token );
    }
    return result;
} // split()

CHolidayInfo
CHolidayManager::read_event_info( const char* input ) const
{
    CHolidayInfo ret;
    char buf[ 256 ];

    // カンマ区切りで文字列分割
    std::string ss( input );
    string_container tokens = split( ss, ',' );

    // スラッシュで年月日取り出し
    string_container date_str = split( tokens[ 0 ], '/' );
    sprintf( buf, "%s %s %s",
        date_str[ 0 ].c_str(), date_str[ 1 ].c_str(), date_str[ 2 ].c_str() );
    sscanf( buf, "%d %d %d",
        &( ret.date_.year ), &( ret.date_.month ), &( ret.date_.day ) );
    ret.date_.weekday = CMonthInfo::Formula_Zeller(
        ret.date_.year, ret.date_.month, ret.date_.day
    );

    // イベント名取り出し
    ret.event_name_ = tokens[ 1 ];

    return ret;
} // CHolidayManager::read_event_info()

CHolidayManager::CHolidayManager()
{
    FILE *fp = fopen( HOLIDAY_FILENAME, "rt" );
    char input_buffer[ 256 ];
    while( fgets( input_buffer, sizeof( input_buffer ), fp ) != NULL )
    {
        auto info = read_event_info( input_buffer );
        list_[ info.date_.year ].push_back( info );
    }
    fclose( fp );
} // CHolidayManager::CHolidayManager()

const CHolidayInfo*
CHolidayManager::Search( const DateInfo* today ) const
{
    for( int i = 0; i < list_[ today->year ].size(); i++ )
    {
        if( list_[ today->year ][ i ].IsMatch( *today ) )
            return &( list_[ today->year ][ i ] );
    }

    return nullptr;
} // CHolidayManager::Search()

CHolidayInfo::CHolidayInfo()
{
    date_.year = 0;
    date_.month = 0;
    date_.day = 0;
    event_name_ = "";
} // HolidayInfo::HolidayInfo()

bool
CHolidayInfo::IsMatch( const DateInfo& check ) const
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

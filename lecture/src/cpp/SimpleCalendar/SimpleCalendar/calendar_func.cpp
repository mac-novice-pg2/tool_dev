#include "pch.h"
#include <cstdio>

#include "calendar.h"

// 外部参照
extern Month_Info_t month_info_2019[];

static eWeekday
next_weekday( eWeekday current )
{
    switch( current )
    {
    case eSun:
        return eMon;
    case eMon:
        return eTue;
    case eTue:
        return eWed;
    case eWed:
        return eThu;
    case eThu:
        return eFri;
    case eFri:
        return eSat;
    case eSat:
        return eSun;
    }
} // next_weekday()

static const char*
weekday2string( eWeekday weekday )
{
    switch( weekday )
    {
    case eSun:
        return "日";
    case eMon:
        return "月";
    case eTue:
        return "火";
    case eWed:
        return "水";
    case eThu:
        return "木";
    case eFri:
        return "金";
    case eSat:
        return "土";
    }
} // weekday2string()

void
print_calener( int month )
{
    Month_Info_t *pInfo = &( month_info_2019[ month - 1 ] );

    // 月の部分を出力
    printf( "%2d月のカレンダー\n\n", month );

    // 曜日部分を出力する
    for( int idx = 0; idx < eWeekEnd; idx++ )
    {
        printf( "%s ", weekday2string( ( eWeekday )idx ) );
    }
    printf( "\n" );

    // 日部分の出力位置合わせ
    eWeekday cur_weekday = pInfo->weekday;
    int pos = ( int )cur_weekday;
    for( int idx = 0; idx < pos; idx++ )
    {
        printf( "   " );
    }

    // 日部分を出力する
	for( int today = 1; today <= pInfo->last; today++, pos++ )
    {
    	if( pos && ( ( pos % 7 ) == 0 ) )
        {
            printf( "\n" );
        }
        printf( "%2d ", today );
    }
	printf( "\n" );
} // print_calener()

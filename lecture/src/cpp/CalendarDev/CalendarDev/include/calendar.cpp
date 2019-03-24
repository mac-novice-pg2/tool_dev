#include "pch.h"
#include "calendar.h"

DateInfo::DateInfo( const DateInfo & date ) :
    year( date.year ),
    month( date.month ),
    day( date.day ),
    weekday( date.weekday )
{}

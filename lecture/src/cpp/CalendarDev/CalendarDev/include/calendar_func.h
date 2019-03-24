#pragma once

#include "CEventManager.h"
#include "calendar.h"

// APIプロトタイプ宣言
void PrintToday( void );
void PrintCalendar( const DateInfo &date );
void PrintNoOvertime( const DateInfo& date );
void PrintHoliday( const DateInfo &date );
void PrintEventDay( const DateInfo &date );

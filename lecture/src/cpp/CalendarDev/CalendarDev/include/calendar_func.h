#pragma once

#include "CEventManager.h"
#include "calendar.h"

// APIプロトタイプ宣言
void PrintToday( void );
void PrintCalendar( const DateInfo &date );
void PrintEvent( const DateInfo &date );
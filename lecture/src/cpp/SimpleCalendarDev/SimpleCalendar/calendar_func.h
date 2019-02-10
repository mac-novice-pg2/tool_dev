#pragma once

#include "CalendarPrinter.h"
#include "CHolidayManager.h"

// APIプロトタイプ宣言
void PrintToday( CalendarPrinter *printer );
void PrintCalendar( int year, int month, CalendarPrinter *printer,
    MonthInfo month_info[] );
void PrintEventAlert( int year, int month, CalendarPrinter *printer,
    MonthInfo month_info[], CHolidayManager *holiday );
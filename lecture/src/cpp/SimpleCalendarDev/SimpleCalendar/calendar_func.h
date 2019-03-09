#pragma once

#include "CEventManager.h"

// APIプロトタイプ宣言
void PrintToday( void );
void PrintCalendar( int year, int month );
void PrintEventAlert( int year, int month, CEventManager *holiday );
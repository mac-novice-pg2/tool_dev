#pragma once

#include "calendar_def.h"

// カレンダー一般
void Cal_ConvJapaneseEraName( int year, char *result );
eWeekday Cal_GetNextWeekday( eWeekday current );
eWeekday Cal_FormulaZeller( DateInfo *date );
DateInfo Cal_GetFirstOfMonth( int year, int month );
int Cal_GetEndOfMonth( DateInfo *date );
bool Cal_IsLeapYear( int year );
EventInfo* Cal_CreateEventTable( const char *holiday_filename, int *tbl_size );
EventInfo* Cal_GetCurEventTable( DateInfo *date, EventInfo *base_tbl,
    int base_tbl_size, int *out_tbl_size );
int Cal_SearchEventTable( DateInfo *date, EventInfo *tbl, int tbl_size );

// 月齢
double Cal_CalcMoonAge( DateInfo *date );
const char* Cal_GetMoonName( double moon_age );

// 干支
void Cal_CalcEto( int year, char *eto_str );

// DateInfo操作用関数
void Cal_StepDateInfo( DateInfo *today, int eom );
void Cal_InitDateInfo( DateInfo *date );

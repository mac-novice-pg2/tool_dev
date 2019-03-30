#pragma once

// カレンダー一般
eWeekday Cal_GetNextWeekday( eWeekday current );
eWeekday Cal_FormulaZeller( DateInfo *date );
DateInfo Cal_GetFirstOfMonth( int year, int month );
int Cal_GetEndOfMonth( DateInfo *date );
bool Cal_IsLeapYear( int year );

// 月齢
double Cal_CalcMoonAge( DateInfo *date );
const char* Cal_GetMoonName( double moon_age );

// 干支
void Cal_CalcEto( int year, char *eto_str );

// DateInfo操作用関数
void Cal_StepDateInfo( DateInfo *today, int eom );
void Cal_InitDateInfo( DateInfo *date );

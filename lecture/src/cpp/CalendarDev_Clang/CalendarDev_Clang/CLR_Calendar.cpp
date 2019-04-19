#include "pch.h"
#include "calendar_func.h"

using System::Globalization::JapaneseLunisolarCalendar;

const char*
Cal_GetRokuyouString( DateInfo *new_date )
{
    auto old_date = System::DateTime( new_date->year, new_date->month, new_date->day );
    auto old_cal = gcnew JapaneseLunisolarCalendar();

    int old_month = old_cal->GetMonth( old_date );
    int old_day = old_cal->GetDayOfMonth( old_date );
    auto new_d = old_cal->AddDays( 
        old_date, 1 - old_cal->GetDayOfYear( old_date ) );

    // 閏月を取得
    int leap_month = old_cal->GetLeapMonth(
        old_cal->GetYear( new_d ),
        old_cal->GetEra( new_d ) );

    if( ( leap_month > 0 ) &&
        ( ( old_month - leap_month ) >= 0 ) )
    {
        old_month--;
    }
    static const char* string_table[] = {
        "大安", "赤口", "先勝", "友引", "先負", "仏滅"
    };

    return string_table[ ( old_month + old_day ) % 6 ];
} // Cal_GetRokuyouIndex()

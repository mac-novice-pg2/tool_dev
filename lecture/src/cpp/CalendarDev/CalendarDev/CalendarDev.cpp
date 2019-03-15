#include "pch.h"

#include "calendar.h"
#include "calendar_func.h"
#include "CMonthInfo.h"
#include "user_interface.h"

//#define TEST_MODE

int main()
{
    DateInfo date;
    date.year = 2019;
    CalendarControler controler;

#ifndef TEST_MODE
    controler.PrintMenu();
#endif
    bool apl_end = false;
    while( !apl_end )
    {
#ifndef TEST_MODE
        date = controler.GetInputDate();
#endif
        PrintCalendar( date );
        controler.ShowInstruction();
        PrintEventAlert( date );
        apl_end = controler.ChangeCalendar();
    }
}

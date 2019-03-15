#include "pch.h"

#include "calendar.h"
#include "calendar_func.h"
#include "CMonthInfo.h"
#include "user_interface.h"

//#define TEST_MODE

int main()
{
    CalendarControler controler;
    CEventManager holiday;

    int year = 2019;
    int month = 3;
#ifndef TEST_MODE
    controler.PrintMenu();
#endif
    bool apl_end = false;
    while( !apl_end )
    {
#ifndef TEST_MODE
        year = controler.GetInputYear();
        month = controler.GetInputMonth();
#endif
        PrintToday();
        PrintEventAlert( year, month, &holiday );
        PrintCalendar( year, month );
        apl_end = controler.ChangeCalendar();
    }
}

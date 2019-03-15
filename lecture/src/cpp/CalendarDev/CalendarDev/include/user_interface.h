#pragma once

#include "calendar.h"

class CalendarControler
{
    int year_;
    int month_;

    void cursor_key_proc( int key );

public:
    void ClearScreen( void ) const;
    bool PrintMenu( void );
    void ShowInstruction( void );
    bool ChangeCalendar( void );
    DateInfo GetInputDate( void );
};

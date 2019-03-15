#pragma once

class CalendarControler
{
    int year_;
    int month_;

    void cursor_key_proc( int key );

public:
    void ClearScreen( void ) const;
    bool PrintMenu();
    bool ChangeCalendar();

    int GetInputYear() const { return year_; }
    int GetInputMonth() const { return month_; }
};


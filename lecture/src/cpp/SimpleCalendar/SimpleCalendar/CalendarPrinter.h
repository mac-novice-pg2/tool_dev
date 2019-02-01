#pragma once

#include <cstdio>

enum eOutputMode
{
    eMonitor,
    eFile,
};

class CalendarPrinter
{
    eOutputMode m_output_mode;
    FILE *m_output;

public:
    CalendarPrinter( void );
    CalendarPrinter( const char *filename );
    ~CalendarPrinter();

    FILE* Output( void );
};


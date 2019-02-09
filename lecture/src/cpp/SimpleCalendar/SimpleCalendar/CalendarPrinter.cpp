#include "pch.h"
#include "CalendarPrinter.h"

CalendarPrinter::CalendarPrinter()
{
    m_output = stdout;
    m_output_mode = eOutputMode::eMonitor;
} // CalendarPrinter::CalendarPrinter()

CalendarPrinter::CalendarPrinter( const char *filename )
{
    m_output = fopen( filename, "wt" );
    m_output_mode = eOutputMode::eFile;
} // CalendarPrinter::CalendarPrinter()

CalendarPrinter::~CalendarPrinter()
{
    if( m_output_mode == eOutputMode::eFile )
    {
        fclose( m_output );
    }
} // CalendarPrinter::~CalendarPrinter()

FILE*
CalendarPrinter::Output( void )
{
    return m_output;
} // CalendarPrinter::Output()

#include "pch.h"
#include "CalendarPrinter.h"


CalendarPrinter::CalendarPrinter()
{
    m_output = stdout;
    m_output_mode = eOutputMode::eMonitor;
}

CalendarPrinter::CalendarPrinter( const char *filename )
{
    m_output = fopen( filename, "wt" );
    m_output_mode = eOutputMode::eFile;
}

CalendarPrinter::~CalendarPrinter()
{
    if( m_output_mode == eOutputMode::eFile )
    {
        fclose( m_output );
    }
}

FILE*
CalendarPrinter::Print( void )
{
    return m_output;
}

#include "pch.h"
#include "user_interface.h"
#include "CPrinter.h"

CPrinter::CPrinter()
{
    m_output = stdout;
    m_output_mode = eMonitor;
} // CalendarPrinter::CalendarPrinter()

CPrinter::CPrinter( const char *filename )
{
    m_output = fopen( filename, "wt" );
    m_output_mode = eFile;
} // CalendarPrinter::CalendarPrinter()

CPrinter::~CPrinter()
{
    if( m_output_mode == eFile )
    {
        fclose( m_output );
    }
} // CalendarPrinter::~CalendarPrinter()

FILE*
CPrinter::Output( void )
{
    return m_output;
} // CalendarPrinter::Output()

#pragma once

#include <cstdio>

class CPrinter
{
    enum eOutputStream
    {
        eMonitor,
        eFile,
    };

    eOutputStream m_output_mode;
    FILE *m_output;

public:
    CPrinter( void );
    CPrinter( const char *filename );
    ~CPrinter();

    FILE* Output( void );
};


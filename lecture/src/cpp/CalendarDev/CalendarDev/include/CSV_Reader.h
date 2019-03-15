#pragma once

#include <cstdio>
#include "common_func.h"

class CSV_Reader
{
    char input_buffer_[ 256 ];
    FILE *fp_;
public:
    CSV_Reader( const char *filename );
    ~CSV_Reader();
    string_container Readline( void );
};


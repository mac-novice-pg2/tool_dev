#pragma once

#include <cstdio>
#include <vector>

using string_container = std::vector< std::string >;

class CSV_Reader
{
    char input_buffer_[ 256 ];
    FILE *fp_;
public:
    CSV_Reader( const char *filename );
    ~CSV_Reader();
    string_container Readline( void );
    static string_container
    Split( const std::string& work, char splitter );
};


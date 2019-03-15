#include "pch.h"
#include "CSV_Reader.h"

#include <cstdio>
#include <sstream>

#include "common_func.h"

CSV_Reader::CSV_Reader( const char *filename )
    : fp_( fopen( filename, "rt" ) )
{

}

CSV_Reader::~CSV_Reader()
{
    fclose( fp_ );
}

string_container
CSV_Reader::Readline( void )
{
    if( feof( fp_ ) )
        return string_container(); // 空要素を返す
    fgets( input_buffer_, sizeof( input_buffer_ ), fp_ );
    return Split( input_buffer_, ',' );
} // CSV_Reader::Readline()

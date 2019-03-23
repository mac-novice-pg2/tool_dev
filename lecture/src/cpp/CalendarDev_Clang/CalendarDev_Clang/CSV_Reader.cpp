#include "pch.h"
#include "CSV_Reader.h"

#include <cstdio>
#include <sstream>
#include <exception>

string_container
CSV_Reader::Split( const std::string& work, char splitter )
{
    string_container result;
    std::stringstream ss( work );
    std::string token;
    while( getline( ss, token, splitter ) )
    {
        if( !token.empty() )
            result.push_back( token );
    }
    return result;
} // CSV_Reader::Split()

CSV_Reader::CSV_Reader( const char *filename )
    : fp_( fopen( filename, "rt" ) )
{
    if( fp_ == NULL )
    {
        fprintf( stderr, "%sが存在しません\n", filename );
        throw std::invalid_argument::exception();
    }
} // CSV_Reader::CSV_Reader()

CSV_Reader::~CSV_Reader()
{
    fclose( fp_ );
} // CSV_Reader::~CSV_Reader()

string_container
CSV_Reader::Readline( void )
{
    if( feof( fp_ ) )
        return string_container(); // 空要素を返す
    fgets( input_buffer_, sizeof( input_buffer_ ), fp_ );
    return Split( input_buffer_, ',' );
} // CSV_Reader::Readline()

#include "pch.h"

#include <sstream>

#include "common_func.h"

string_container
Split( const std::string& work, char splitter )
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
} // Split()
#pragma once

#include <vector>

using string_container = std::vector< std::string >;

string_container
Split( const std::string& work, char splitter );

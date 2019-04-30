#include "Util.h"

#include <cstring>
#include <cassert>

const char *
Util::ConvEnum2String( eFoodGenre genre )
{
    switch( genre )
    {
    case eFoodGenre::Japanese:
        return "Japanese";

    case eFoodGenre::Western:
        return "Western";

    case eFoodGenre::China:
        return "China";

    case eFoodGenre::Italian:
        return "Italian";

    case eFoodGenre::French:
        return "French";

    case eFoodGenre::NonGenre:
        return "NonGenre";

    default:
        assert( 0 );
    }
} // Util::ConvEnum2String()

const char *
Util::ConvEnum2String( eSeason season )
{
    switch( season )
    {
    case eSeason::Spring:
        return "Spring";

    case eSeason::Summer:
        return "Summer";

    case eSeason::Autumn:
        return "Autumn";

    case eSeason::Winter:
        return "Winter";

    case eSeason::AllSeason:
        return "AllSeason";

    default:
        assert( 0 );
    }
} // Util::ConvEnum2String()

const char *
Util::ConvEnum2String( eDifficulty genre )
{
    switch( genre )
    {
    case eDifficulty::Hard:
        return "Hard";

    case eDifficulty::Normal:
        return "Normal";

    case eDifficulty::Easy:
        return "Easy";

    default:
        assert( 0 );
    }
} // Util::ConvEnum2String()

const char *
Util::ConvEnum2String( eType type )
{
    switch( type )
    {
    case eType::Breakfast:
        return "Breakfast";

    case eType::Lunch:
        return "Lunch";

    case eType::Dinner:
        return "Dinner";

    case eType::Desert:
        return "Desert";

    case eType::LightMeal:
        return "LightMeal";

    case eType::HomeParty:
        return "HomeParty";

    case eType::Outdoor:
        return "Outdoor";

    case eType::AllSituation:
        return "AllSituation";

    default:
        assert( 0 );
    }
} // Util::ConvEnum2String()

char *
Util::Trim_C_String( char *c_str )
{
    int cur = 0;

    // 先頭部分のスペースをスキップ
    while( c_str[ cur ] == ' ' )
    {
        cur++;
    }

    // 末尾の改行コードと空白を削る
    char *part = &( c_str[ cur ] );
    char end_char = part[ strlen( part ) - 1 ];
    if( ( end_char == '\n' ) || ( end_char == ' ' ) )
        part[ strlen( part ) - 1 ] = '\0';

    return part;
} // Trim_C_String()


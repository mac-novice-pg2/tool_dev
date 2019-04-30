#include "Util.h"

#include <cstdio>
#include <cstring>
#include <cassert>

const char *
Util::ConvEnum2String( eFoodGenre genre )
{
    switch( genre )
    {
    case eFoodGenre::Japanese_Food:
        return "和食";

    case eFoodGenre::Western_Food:
        return "洋食";

    case eFoodGenre::Chinese_Food:
        return "中華";

    case eFoodGenre::Italian_Food:
        return "イタリアン";

    case eFoodGenre::French_Food:
        return "フランス料理";

    case eFoodGenre::Stateless_Food:
        return "無国籍料理";

    case eFoodGenre::Invalid_Food:
        return "";

    default:
        assert( 0 );
        return "";
    }
} // Util::ConvEnum2String()

const char *
Util::ConvEnum2String( eSeason season )
{
    switch( season )
    {
    case eSeason::Spring:
        return "春";

    case eSeason::Summer:
        return "夏";

    case eSeason::Autumn:
        return "秋";

    case eSeason::Winter:
        return "冬";

    case eSeason::AllSeason:
        return "年中";

    case eSeason::Invalid_Season:
        return "";

    default:
        assert( 0 );
        return "";
    }
} // Util::ConvEnum2String()

const char *
Util::ConvEnum2String( eDifficulty genre )
{
    switch( genre )
    {
    case eDifficulty::Hard_Difficulty:
        return "むずかしい";

    case eDifficulty::Normal_Difficulty:
        return "ほどほど";

    case eDifficulty::Easy_Difficulty:
        return "かんたん";

    case eDifficulty::Invalid_Difficulty:
        return "";

    default:
        assert( 0 );
        return "";
    }
} // Util::ConvEnum2String()

const char *
Util::ConvEnum2String( eFoodStyle type )
{
    switch( type )
    {
    case eFoodStyle::Breakfast:
        return "朝食";

    case eFoodStyle::Lunch:
        return "昼食";

    case eFoodStyle::Dinner:
        return "夕食";

    case eFoodStyle::Desert:
        return "デザート";

    case eFoodStyle::LightMeal:
        return "軽食";

    case eFoodStyle::HomeParty:
        return "ホームパーティ";

    case eFoodStyle::Outdoor:
        return "アウトドア";

    case eFoodStyle::AllSituation:
        return "不問";

    case eFoodStyle::Invalid_FoodStyle:
        return "";

    default:
        assert( 0 );
        return "";
    }
} // Util::ConvEnum2String()

void
Util::WaitEnterKey( const char *message )
{
    char buf[ 8 ];
    printf( "%s\n", message );
    fgets( buf, sizeof( buf ), stdin );
} // Util::WaitEnterKey();

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


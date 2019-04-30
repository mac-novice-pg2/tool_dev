#include "Recipe.h"

#include <cassert>

#include "Util.h"

#define ARRAY_COUNT( arr ) ( sizeof( arr ) / sizeof( arr[ 0 ] ) )

static eFoodGenre
char2FoodGenre( const char *s )
{
    const char* tbl[] = {
        "和食","中華", "洋食", "イタリア", "フランス"
    };

    for( int idx = 0; idx < ARRAY_COUNT( tbl ); idx++ )
    {
        if( strcmp( s, tbl[ idx ] ) == 0 )
        {
            return static_cast< eFoodGenre >( idx );
        }
    }
    assert( 0 );
} // char2FoodGenre()

static eSeason
char2eSeason( const char *s )
{
    const char *tbl[] = {
        "春","夏", "秋", "冬", "オールシーズン"
    };

    for( int idx = 0; idx < ARRAY_COUNT( tbl ); idx++ )
    {
        if( strcmp( s, tbl[ idx ] ) == 0 )
        {
            return static_cast< eSeason >( idx );
        }
    }
    assert( 0 );
} // char2eSeason()

static eDifficulty
char2eDifficulty( const char *s )
{
    const char *tbl[] = {
        "むずかしい","ほどほど", "かんたん",
    };

    for( int idx = 0; idx < ARRAY_COUNT( tbl ); idx++ )
    {
        if( strcmp( s, tbl[ idx ] ) == 0 )
        {
            return static_cast< eDifficulty >( idx );
        }
    }
    assert( 0 );
} // char2eDifficulty()

static eType
char2eType( const char *s )
{
    const char *tbl[] = {
        "朝食","昼食", "ディナー", "軽食", "立食パーティ", "アウトドア"
    };

    for( int idx = 0; idx < ARRAY_COUNT( tbl ); idx++ )
    {
        if( strcmp( s, tbl[ idx ] ) == 0 )
        {
            return static_cast< eType >( idx );
        }
    }
    assert( 0 );
} // char2eType()

void
Recipe::set_numeric_field( FILE *fp, int *out )
{
    static char buf[ 256 ];

    fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    fgets( buf, sizeof( buf ), fp ); // 設定値読み出し
    if( sscanf( buf, "%d", out ) != 1 )
        assert( 0 );
} // Recipe::set_numeric_field()

const char*
Recipe::read_string_param( FILE *fp )
{
    static char buf[ 256 ];

    fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    fgets( buf, sizeof( buf ), fp ); // 設定値読み出し
    if( buf[ strlen( buf ) - 1 ] == '\n' )
        buf[ strlen( buf ) - 1 ] = '\0';

    return buf;
} // Recipe::read_string_param()

SearchInfo
Recipe::read_entry( FILE *fp )
{
    SearchInfo out;
    char buf[ 256 ];

    // [ITEMxxxxx]
    fgets( buf, sizeof( buf ), fp );
    buf[ strlen( buf ) - 2 ] = '\0';
    if( sscanf( &( buf[ 6 ] ), "%d", &( out.item_no ) ) != 1 )
        assert( 0 );

    // 料理名フィールド読み出し
    fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    fgets( buf, sizeof( buf ), fp ); // 設定値読み出し
    strcpy( out.name, Util::Trim_C_String( buf ) );

    // 調理時間フィールド読み出し
    set_numeric_field( fp, &( out.time ) );

    // 費用フィールド読み出し
    set_numeric_field( fp, &( out.cost ) );

    // カロリーフィールド読み出し
    set_numeric_field( fp, &( out.kcal ) );

    // ジャンルフィールド読み出し
    out.genre = char2FoodGenre( read_string_param( fp ) );

    // 形式フィールド読み出し
    out.type = char2eType( read_string_param( fp ) );

    // 難易度フィールド読み出し
    out.difficulty = char2eDifficulty( read_string_param( fp ) );

    // シーズンフィールド読み出し
    out.season = char2eSeason( read_string_param( fp ) );

    // 調理器具フィールド読み出し
    fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    fgets( buf, sizeof( buf ), fp ); // 設定値読み出し
    strcpy( out.kitchenware, Util::Trim_C_String( buf ) );

    // レシピフィールド読み出し
    fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    out.recipe[ 0 ] = '\0';
    while( strcmp( buf, "<ingredients>\n" ) != 0 )
    {
        fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
        if( ( sizeof( out.recipe ) / 2 ) < strlen( out.recipe ) + strlen( buf ) )
            break;
        strcat( out.recipe, buf );
    }

    // 材料フィールド読み出し
    fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    out.ingredients[ 0 ] = '\0';
    while( strcmp( buf, "<EOI>\n" ) != 0 )
    {
        fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
        if( ( sizeof( out.ingredients ) / 2 ) < strlen( out.ingredients ) + strlen( buf ) )
            break;
        strcat( out.ingredients, buf );
    }
    return out;
} // Recipe::read_entry()

void Recipe::create_search_list( const char* recipe_fname )
{
    FILE *fp = fopen( recipe_fname, "rt" );

    // アイテム数取得(配列版置き換え時には使用する)
    char buf[ 0x100 ];
    int info_count;
    fgets( buf, sizeof( buf ), fp );
    if( sscanf( buf, "%d", &info_count ) != 1 )
        assert( 0 );

    SearchInfo item;
    int info_idx = 0;
    while( info_idx < info_count )
    {
        search_info_.push_back( read_entry( fp ) );
        info_idx++;
    }
    fclose( fp );
} // Recipe::create_search_list()

Recipe::Recipe( const char *recipe_fname )
{
    create_search_list( recipe_fname );
} // Recipe::Recipe()

Recipe::~Recipe()
{
}

void Recipe::ShowDishList()
{
    printf( "\n"
            "登録されている料理一覧\n"
    );
    for( SearchInfo r : search_info_ )
    {
        printf( "%s\n", r.name );
    }
} // Recipe::ShowDishList()

const SearchInfoList&
Recipe::Search( SearchCondition *cond )
{
    search_result_.clear();
    for( SearchInfo r : search_info_ )
    {
        if( cond->IsMatch_name( r.name ) ||
            cond->IsMatch_time( r.time ) ||
            cond->IsMatch_cost( r.cost ) ||
            cond->IsMatch_kcal( r.kcal ) ||
            cond->IsMatch_genre( r.genre ) ||
            cond->IsMatch_season( r.season ) ||
            cond->IsMatch_difficulty( r.difficulty ) ||
            cond->IsMatch_type( r.type ) )
        {
            search_result_.push_back( r );
        }
    }

    return search_result_;
} // Recipe::Search()

bool
SearchCondition::IsMatch_name( const char *check )
{
    if( ( strcmp( check, "" ) != 0 ) &&
        ( strcmp( check, this->name ) == 0 ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_name()

bool
SearchCondition::IsMatch_time( int check )
{
    if( ( check > 0 ) &&
        ( check < this->time ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_time()

bool
SearchCondition::IsMatch_cost( int check )
{
    if( ( check > 0 ) &&
        ( check < this->cost ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_cost()

bool
SearchCondition::IsMatch_kcal( int check )
{
    if( ( check > 0 ) &&
        ( check < this->kcal ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_kcal()

bool
SearchCondition::IsMatch_genre( eFoodGenre check )
{
    if( ( check != eFoodGenre::NonGenre ) &&
        ( check == this->genre ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_genre()

bool
SearchCondition::IsMatch_season( eSeason check )
{
    if( ( check != eSeason::AllSeason ) &&
        ( check == this->season ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_season()

bool
SearchCondition::IsMatch_difficulty( eDifficulty check )
{
    if( ( check != eDifficulty::Easy ) &&
        ( check == this->difficulty ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_difficulty()

bool
SearchCondition::IsMatch_type( eType check )
{
    if( ( check != eType::AllSituation ) &&
        ( check == this->type ) )
    {
        return false;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_type()

const char *
SearchInfo::ToString()
{
    static char buf[ 1024 * 2 ];

    sprintf( buf,
             "No       : %d\n"
             "料理名   : %s\n"
             "調理時間 : %2d分\n"
             "費用     : %5d円\n"
             "カロリー : %4dkcal\n"
             "調理器具 : %s\n"
             "ジャンル : %s\n"
             "シーズン : %s\n"
             "難易度   : %s\n"
             "形式     : %s\n"
             "レシピ:\n%s\n"
             "材料:\n%s\n",
             item_no,
             name,
             time,
             cost,
             kcal,
             kitchenware,
             Util::ConvEnum2String( genre ),
             Util::ConvEnum2String( season ),
             Util::ConvEnum2String( difficulty ),
             Util::ConvEnum2String( type ),
             recipe,
             ingredients );

    return buf;
} // SearchInfo::ToString()

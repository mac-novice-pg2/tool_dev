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
    return eFoodGenre::Invalid_Food;
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
    return eSeason::Invalid_Season;
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
    return eDifficulty::Invalid_Difficulty;
} // char2eDifficulty()

static eFoodStyle
char2eFoodStyle( const char *s )
{
    const char *tbl[] = {
        "朝食","昼食", "ディナー", "軽食", "立食パーティ", "アウトドア"
    };

    for( int idx = 0; idx < ARRAY_COUNT( tbl ); idx++ )
    {
        if( strcmp( s, tbl[ idx ] ) == 0 )
        {
            return static_cast< eFoodStyle >( idx );
        }
    }
    assert( 0 );
    return eFoodStyle::Invalid_FoodStyle;
} // char2eFoodStyle()

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
    strcpy( buf, Util::Trim_C_String( buf ) );

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
    out.type = char2eFoodStyle( read_string_param( fp ) );

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
    fgets( buf, sizeof( buf ), fp ); // 設定値読み出し
    out.recipe[ 0 ] = '\0';
    while( strcmp( buf, "<ingredients>\n" ) != 0 )
    {
        if( ( sizeof( out.recipe ) / 2 ) < strlen( out.recipe ) + strlen( buf ) )
            break;
        strcat( out.recipe, buf );
        fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    }

    // 材料フィールド読み出し
    fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
    fgets( buf, sizeof( buf ), fp ); // 設定値読み出し
    out.ingredients[ 0 ] = '\0';
    while( strcmp( buf, "<EOI>\n" ) != 0 )
    {
        if( ( sizeof( out.ingredients ) / 2 ) < strlen( out.ingredients ) + strlen( buf ) )
            break;
        strcat( out.ingredients, buf );
        fgets( buf, sizeof( buf ), fp ); // フィールドヘッダ読み飛ばし
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
            "%s\n", Util::separator );
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

bool dbg_matching = true;
bool
SearchCondition::IsMatch_name( const char *check )
{
    if( ( strcmp( this->name, "" ) != 0 ) &&
        ( strcmp( this->name, check ) == 0 ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_name()

bool
SearchCondition::IsMatch_time( int check )
{
    if( ( this->time > 0 ) &&
        ( this->time >= check ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_time()

bool
SearchCondition::IsMatch_cost( int check )
{
    if( ( this->cost > 0 ) &&
        ( this->cost >= check ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_cost()

bool
SearchCondition::IsMatch_kcal( int check )
{
    if( ( this->kcal > 0 ) &&
        ( this->kcal >= check ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_kcal()

bool
SearchCondition::IsMatch_genre( eFoodGenre check )
{
    if( ( this->genre != eFoodGenre::Invalid_Food ) &&
        ( this->genre == check ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_genre()

bool
SearchCondition::IsMatch_season( eSeason check )
{
    if( ( this->season != eSeason::Invalid_Season ) &&
        ( this->season == check ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_season()

bool
SearchCondition::IsMatch_difficulty( eDifficulty check )
{
    if( ( this->difficulty != eDifficulty::Invalid_Difficulty ) &&
        ( this->difficulty == check ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
    }
    else
    {
        return false;
    }
} // SearchCondition::IsMatch_difficulty()

bool
SearchCondition::IsMatch_type( eFoodStyle check )
{
    if( ( this->type != eFoodStyle::Invalid_FoodStyle ) &&
        ( this->type == check ) )
    {
        if( dbg_matching )
            puts( __func__ );
        return true;
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
             "調理時間 : %d分\n"
             "費用     : %d円\n"
             "カロリー : %dkcal\n"
             "調理器具 : %s\n"
             "ジャンル : %s\n"
             "シーズン : %s\n"
             "難易度   : %s\n"
             "形式     : %s\n"
             "%s\n"
             "<<< レシピ >>>\n%s\n"
             "%s\n"
             "<<< 材料/分量 >>>\n%s\n",
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
             Util::separator,
             recipe,
             Util::separator,
             ingredients );

    return buf;
} // SearchInfo::ToString()

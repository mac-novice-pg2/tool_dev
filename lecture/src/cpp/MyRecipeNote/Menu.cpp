#include "Menu.h"

#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <cassert>

#include "Util.h"

int
Menu::input_num( const char *message )
{
    int ret;
    char buf[ 256 ];

    printf( "%sを入力して下さい( Enterで確定 )\n", message );
    fgets( buf, sizeof( buf ), stdin );
    if( sscanf( buf, "%d", &ret ) != 1 )
        assert( 0 );

    return ret;
} // Menu::input_num()

void
Menu::input_string( const char *message,
                    char *output,
                    int len )
{
    printf( "%sを入力して下さい( Enterで確定 )\n", message );
    fgets( output, len, stdin );
} // Menu::input_string

Menu::Menu()
    : recipe_( nullptr )
{}

Menu::~Menu()
{}

KeyCode Menu::InputKey()
{
    KeyCode key_code;

    key_code.key1st = _getch(); // 1回目のキーコードを拾う
    key_code.key2nd = _getch(); // 2回目のキーコードを拾う

    return key_code;
} // Menu::InputKey()

void Menu::Clear()
{
    system( "cls" );
} // Menu::Clear()

void
SearchMenu::cursor_key_proc( int key )
{
    switch( key )
    {
    case 0x50: // "↓"
        break;
    case 0x48: // "↑"
        break;
    case 0x4D: // "→"
        break;
    case 0x4B: // "←"
        break;
    }
} // SearchMenu::cursor_key_proc()

void
SearchMenu::menu_show_list()
{
    recipe_->ShowDishList();
    Util::WaitEnterKey( "Enterキーで検索メニューに戻ります\n" );
} // SearchMenu::menu_show_list()

void
SearchMenu::menu_search()
{
    char input_buf[ 256 ];
    SearchCondition cond;
    bool isEnd = false;
    while( !isEnd )
    {
        printf( "検索項目\n"
                "s : 指定条件で検索\n"
                "q : 終了\n"
                "-------------------------------------------\n"
        );
        printf( "1 : 料理番号 | %d\n", cond.item_no );
        printf( "2 : 料理名   | %s\n", cond.name );
        printf( "3 : 調理時間 | %d\n", cond.time );
        printf( "4 : 費用     | %d\n", cond.cost );
        printf( "5 : カロリー | %d\n", cond.kcal );
        printf( "6 : ジャンル | %s\n", Util::ConvEnum2String( cond.genre ) );
        printf( "7 : シーズン | %s\n", Util::ConvEnum2String( cond.season ) );
        printf( "8 : 難易度   | %s\n", Util::ConvEnum2String( cond.difficulty ) );
        printf( "9 : 形式     | %s\n", Util::ConvEnum2String( cond.type ) );

        fgets( input_buf, sizeof( input_buf ), stdin );
        switch( input_buf[ 0 ] )
        {
        case '1':
            cond.item_no = input_num( "アイテム番号" );
            break;

        case '2':
            input_string( "料理名", input_buf, sizeof( input_buf ) );
            strcpy( cond.name, Util::Trim_C_String( input_buf ) );
            break;

        case '3':
            cond.time = input_num( "調理時間" );
            break;

        case '4':
            cond.cost = input_num( "費用" );
            break;

        case '5':
            cond.kcal = input_num( "カロリー" );
            break;

        case '6':
            cond.genre = input_genre( "ジャンル" );
            break;

        case '7':
            cond.season = input_season( "シーズン" );

        case '8':
            cond.difficulty = input_difficulty( "難易度" );
            break;

        case '9':
            cond.type = input_type( "形式" );
            break;

        case 'q':
            isEnd = true;
            break;

        case 's':
            Clear();
            printf( "検索結果\n" );
            for( auto r : recipe_->Search( &cond ) )
                puts( r.ToString() );
            Util::WaitEnterKey( "Enterキーを押すと元のメニューに戻ります\n" );
            break;
        }
    }
} // SearchMenu::menu_search()

eFoodGenre
SearchMenu::input_genre( const char *message )
{
    char buf[ 256 ];

    printf( "%sを入力して下さい( Enterで確定 )\n"
            "-----------------------------------\n"
            "1    :和食\n"
            "2    :洋食\n"
            "3    :中華\n"
            "4    :イタリアン\n"
            "5    :フランス料理\n"
            "6    :無国籍\n"
            "以外 :検索除外\n"
            , message );

    fgets( buf, sizeof( buf ), stdin );
    int select;
    if( sscanf( buf, "%d", &select ) != 1 )
        assert( 0 );
    switch( select )
    {
    case 1: return eFoodGenre::Japanese_Food;
    case 2: return eFoodGenre::Western_Food;
    case 3: return eFoodGenre::Chinese_Food;
    case 4: return eFoodGenre::Italian_Food;
    case 5: return eFoodGenre::French_Food;
    case 6: return eFoodGenre::Stateless_Food;
    default:
        return eFoodGenre::Invalid_Food;
    }
} // SearchMenu::input_genre()

eSeason
SearchMenu::input_season( const char *message )
{
    char buf[ 256 ];

    printf( "%sを入力して下さい( Enterで確定 )\n"
            "-----------------------------------\n"
            "1    :春\n"
            "2    :夏\n"
            "3    :秋\n"
            "4    :冬\n"
            "5    :年中\n"
            "以外 :検索除外\n"
            , message );

    fgets( buf, sizeof( buf ), stdin );
    int select;
    if( sscanf( buf, "%d", &select ) != 1 )
        assert( 0 );
    switch( select )
    {
    case 1: return eSeason::Spring;
    case 2: return eSeason::Summer;
    case 3: return eSeason::Autumn;
    case 4: return eSeason::Winter;
    case 5: return eSeason::AllSeason;
    default:
        return eSeason::Invalid_Season;
    }
} // SearchMenu::input_season()

eDifficulty
SearchMenu::input_difficulty( const char *message )
{
    char buf[ 256 ];

    printf( "%sを入力して下さい( Enterで確定 )\n"
            "-----------------------------------\n"
            "1    :むずかしい\n"
            "2    :ほどほど\n"
            "3    :かんたん\n"
            "以外 :検索除外\n"
            , message );

    fgets( buf, sizeof( buf ), stdin );
    int select;
    if( sscanf( buf, "%d", &select ) != 1 )
        assert( 0 );
    switch( select )
    {
    case 1: return eDifficulty::Hard_Difficulty;
    case 2: return eDifficulty::Normal_Difficulty;
    case 3: return eDifficulty::Easy_Difficulty;
    default:
        return eDifficulty::Invalid_Difficulty;
    }
} // SearchMenu::input_difficulty()

eFoodStyle
SearchMenu::input_type( const char *message )
{
    char buf[ 256 ];

    printf( "%sを入力して下さい( Enterで確定 )\n"
            "-----------------------------------\n"
            "1    :朝食\n"
            "2    :昼食\n"
            "3    :夕食\n"
            "4    :デザート\n"
            "5    :軽食\n"
            "6    :ホームパーティ\n"
            "7    :アウトドア\n"
            "8    :不問\n"
            "以外 :検索除外\n"
            , message );

    fgets( buf, sizeof( buf ), stdin );
    int select;
    if( sscanf( buf, "%d", &select ) != 1 )
        assert( 0 );
    switch( select )
    {
    case 1: return eFoodStyle::Breakfast;
    case 2: return eFoodStyle::Lunch;
    case 3: return eFoodStyle::Dinner;
    case 4: return eFoodStyle::Desert;
    case 5: return eFoodStyle::LightMeal;
    case 6: return eFoodStyle::HomeParty;
    case 7: return eFoodStyle::Outdoor;
    case 8: return eFoodStyle::AllSituation;
    default:
        return eFoodStyle::Invalid_FoodStyle;
    }
} // SearchMenu::input_type()

void SearchMenu::Show( Recipe *recipe )
{
    recipe_ = recipe;
    
    char input_buf[ 256 ];
    bool isEnd = false;
    while( !isEnd )
    {
        printf( "レシピ検索メニュー\n"
                "l : 料理一覧\n"
                "i : 検索\n"
                "q : 終了\n"
        );

        fgets( input_buf, sizeof( input_buf ), stdin );
        switch( input_buf[ 0 ] )
        {
        case 'l': // 料理一覧表示
            menu_show_list();
            Clear();
            break;

        case 'i': // 検索メニューへ
            Clear();
            menu_search();
            break;

        case 'q':
            isEnd = true;
            break;
        }
    }
} // SearchMenu::Show()

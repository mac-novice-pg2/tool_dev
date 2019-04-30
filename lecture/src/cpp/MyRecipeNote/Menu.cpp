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

void SearchMenu::menu_show_list()
{
    recipe_->ShowDishList();
} // SearchMenu::menu_show_list()
void SearchMenu::menu_search()
{
    char input_buf[ 256 ];
    SearchCondition cond;
    bool isEnd = false;
    int key_code;
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
            strcpy( cond.name, input_buf );
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
            for( auto r : recipe_->Search( &cond ) )
                puts( r.ToString() );
            break;
        }
    }
}

eFoodGenre
SearchMenu::input_genre( const char *message )
{
    printf( "%sを選択して下さい\n", message );
    /*
    Japanese,
    Western,
    China,
    Italian,
    French,
    NonGenre,
    */

    
    return eFoodGenre();
} // SearchMenu::input_genre()

eSeason
SearchMenu::input_season( const char *message )
{
    printf( "%sを選択して下さい\n", message );

    /*
    Spring,
    Summer,
    Autumn,
    Winter,
    AllSeason,
    */

    return eSeason();
} // SearchMenu::input_season()

eDifficulty
SearchMenu::input_difficulty( const char *message )
{
    printf( "%sを選択して下さい\n", message );

    /*
    Hard,
    Normal,
    Easy,
    */
    return eDifficulty();
} // SearchMenu::input_difficulty()

eType
SearchMenu::input_type( const char *message )
{
    printf( "%sを選択して下さい\n", message );

    /*
    Breakfast,
    Lunch,
    Dinner,
    Desert,
    LightMeal,
    HomeParty,
    Outdoor,
    AllSituation
    */

    return eType();
} // SearchMenu::input_type()

void SearchMenu::Show( Recipe *recipe )
{
    recipe_ = recipe;
    
    char input_buf[ 256 ];
    int key_code;
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
            Clear();
            menu_show_list();
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

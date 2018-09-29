#include "pch.h"

#include <cstdio>
#include <cstring>

#define dbg() printf( "[%d]%s()\n", __LINE__, __func__ )

typedef void( *func_t )( void );
typedef enum{
    eMENU_SELECT_FILE = 0, // 画像ファイル選択
    eMENU_COLOR_CHG_MONO,  // [色変換] モノクロ変換
    eMENU_COLOR_CHG_SEPIA, // [色変換] セピア変換
    eMENU_ROTATE_L,        // [回転]   反時計回り/CCW
    eMENU_ROTATE_R,        // [回転]   時計回り/CW
    eMENU_ZOOM_UP,         // [サイズ] 拡大
    eMENU_ZOOM_DOWN,       // [サイズ] 縮小
}eMenuID;

// eMENU_SELECT_FILE
static void
menu_select_file( void )
{
    dbg();
}

// eMENU_COLOR_CHG_MONO
static void
menu_color_chg_mono( void )
{
    dbg();
}

// eMENU_COLOR_CHG_SEPIA
static void
menu_color_chg_sepia( void )
{
    dbg();
}

// eMENU_ROTATE_L
static void
menu_rotate_left( void )
{
    dbg();
}

// eMENU_ROTATE_R
static void
menu_rotate_right( void )
{
    dbg();
}

// eMENU_ZOOM_UP
static void
menu_zoom_up( void )
{
    dbg();
}

// eMENU_ZOOM_DOWN
static void
menu_zoom_down( void )
{
    dbg();
}

func_t  menu_func[] = {
    { menu_select_file }, // eMENU_SELECT_FILE
    { menu_color_chg_mono }, // eMENU_COLOR_CHG_MONO
    { menu_color_chg_sepia }, // eMENU_COLOR_CHG_SEPIA
    { menu_rotate_left }, // eMENU_ROTATE_L
    { menu_rotate_right }, // eMENU_ROTATE_R
    { menu_zoom_up }, // eMENU_ZOOM_UP
    { menu_zoom_down } // eMENU_ZOOM_DOWN
};

void UI_menu_main( void )
{
    printf(
        "メニューから行いたい操作を入力し、Enterキーを押して下さい\n"
        " ファイル選択   : 0\n"
        " モノクロ変換   : 1\n"
        " セピア変換     : 2\n"
        " 右回転         : 3\n"
        " 左回転         : 4\n"
        " 拡大           : 5\n"
        " 縮小           : 6\n"
        " 終了           : Q\n" );

    const int buf_size = 256;

    int menu_id;
    char buf[ buf_size ];
    for( ;; )
    {
        printf( "input> " );
        if( gets_s( buf, buf_size ) == NULL )
        {
            printf( "入力の読み取りに失敗しました。もう一度入力して下さい\n" );
            continue;
        }
        else if( strcmp( buf, "q" ) == 0 )
        {
            printf( "メニューを終了します\n" );
            break;
        }
        else
        {
            sscanf_s( buf, "%d", &menu_id );
            menu_func[ menu_id ]();
        }
    }
}
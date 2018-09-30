#include "pch.h"

#include "ui_input_result.h"

#include <cstdio>
#include <cstring>
#include <ctype.h>

#define dbg() printf( "[%d]%s()\n", __LINE__, __func__ )
const char *input_err_msg = "入力の読み取りに失敗しました。もう一度入力して下さい";

/**
    ファイル操作系メニュー
 */
// eMENU_FILE_OPEN
static void
menu_file_open( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_FILE_OPEN;

    auto *param = ( eMENU_INPUT_RESULT_FILE_OPEN*  )( menu_param->input_result );
}

// eMENU_FILE_CLOSE
static void
menu_file_close( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_FILE_CLOSE;

    auto *param = ( eMENU_INPUT_RESULT_FILE_CLOSE* )( menu_param->input_result );
}

// eMENU_FILE_SAVE
static void
menu_file_save( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_FILE_SAVE;

    auto *param = ( eMENU_INPUT_RESULT_FILE_SAVE* )( menu_param->input_result );
}

/**
    色操作系メニュー
 */
// eMENU_COLOR_CHG_MONO
static void
menu_color_chg_mono( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_COLOR_CHG_MONO;

    auto *param = ( eMENU_INPUT_RESULT_COLOR_CHG_MONO* )( menu_param->input_result );
}

// eMENU_COLOR_CHG_SEPIA
static void
menu_color_chg_sepia( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_COLOR_CHG_SEPIA;

    auto *param = ( eMENU_INPUT_RESULT_COLOR_CHG_SEPIA* )( menu_param->input_result );
}

/**
    回転系メニュー
 */
// eMENU_ROTATE_L
static void
menu_rotate_left( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_ROTATE_L;

    auto *param = ( eMENU_INPUT_RESULT_ROTATE_L* )( menu_param->input_result );
}

// eMENU_ROTATE_R
static void
menu_rotate_right( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_ROTATE_R;

    auto *param = ( eMENU_INPUT_RESULT_ROTATE_R* )( menu_param->input_result );
}

/**
    変形系メニュー
 */
// eMENU_ZOOM_UP
static void
menu_zoom_up( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_ZOOM_UP;

    auto *param = ( eMENU_INPUT_RESULT_ZOOM_UP* )( menu_param->input_result );
}

// eMENU_ZOOM_DOWN
static void
menu_zoom_down( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_ZOOM_DOWN;

    auto *param = ( eMENU_INPUT_RESULT_ZOOM_DOWN* )( menu_param->input_result );
}

// eMENU_RESIZE
static void
menu_resize( eMenuParam *menu_param )
{
    dbg();
    menu_param->menu_id = eMENU_RESIZE;

    auto *param = ( eMENU_INPUT_RESULT_RESIZE* )( menu_param->input_result );
}

/**
    メニュー関数
*/
typedef void( *func_t )( eMenuParam *menu_param );
func_t menu_func_file_ctrl[] = {
    menu_file_open,
    menu_file_close,
    menu_file_save
};

func_t menu_func_color_chg[] = {
    menu_color_chg_mono,
    menu_color_chg_sepia,
};

func_t menu_func_rotate[] = {
    menu_rotate_left,
    menu_rotate_right,
};

func_t menu_func_resize[] = {
    menu_zoom_up,
    menu_zoom_down,
    menu_resize
};

/**
    メニュー処理メイン
 */
void
UI_menu_main( eMenuParam *menu_param )
{
    printf(
        "メニューから行いたい操作を入力し、Enterキーを押して下さい\n"
        "[ファイル操作]\n"
        " - 操作 -         : - 入力値 -\n"
        " ファイルオープン : O\n"
        " ファイル保存     : S\n"
        " ファイルクローズ : C\n"
        "\n"

        "[色操作]\n"
        " - 操作 -         : - 入力値 -\n"
        " モノクロ変換     : 1000\n"
        " セピア変換       : 1001\n"
        "\n"

        "[回転]\n"
        " - 操作 -         : - 入力値 -\n"
        " 右回転           : 2001\n"
        " 左回転           : 2002\n"
        "\n"

        "[変形]\n"
        " - 操作 -         : - 入力値 -\n"
        " 拡大             : 3001\n"
        " 縮小             : 3002\n"
        " リサイズ         : 3003\n"
        "\n"

        "[その他]\n"
        " 終了             : Q\n"
    );

    const int buf_size = 256;

    char input_char;
    char buf[ buf_size ];
    for( ;; )
    {
        printf( "input> " );
        if( gets_s( buf, buf_size ) == NULL )
        {
            puts( input_err_msg );
            continue;
        }

        input_char = toupper( buf[ 0 ] );
        if( input_char == 'Q' )
        {
            printf( "アプリケーションを終了します\n" );
            menu_param->menu_id = eMENU_ID_END;
            break;
        }
        else if( isalpha( input_char ) )
        {
            switch( input_char )
            {
            case 'O':
                menu_file_open( menu_param );
                break;
            case 'C':
                menu_file_close( menu_param );
                break;
            case 'S':
                menu_file_save( menu_param );
                break;
            default:
                puts( input_err_msg );
                break;
            }
        }
        else
        {
            int menu_id;
            sscanf_s( buf, "%d", &menu_id );
            if( ( menu_id >= 1000 ) && ( menu_id <= 1999 ) )
            {
                menu_func_color_chg[ menu_id - 1000 ]( menu_param );
            }
            else if( ( menu_id >= 2000 ) && ( menu_id <= 2999 ) )
            {
                menu_func_rotate[ menu_id - 2000 ]( menu_param );
            }
            else if( ( menu_id >= 3000 ) && ( menu_id <= 3999 ) )
            {
                menu_func_resize[ menu_id - 3000 ]( menu_param );
            }
            else
            {
                puts( input_err_msg );
            }
        }
    }
}
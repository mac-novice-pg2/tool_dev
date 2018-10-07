﻿#include "pch.h"

#include "UI_input_result.h"

#include <cstdio>
#include <cstring>
#include <ctype.h>

const char *input_err_msg = "入力の読み取りに失敗しました。もう一度入力して下さい";

/**
    ファイル操作系メニュー
 */
// eMENU_FILE_OPEN
static bool
menu_file_open( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_FILE_OPEN input_param;
    menu_param->menu_id = eMENU_FILE_OPEN;

    printf( "処理対象の画像ファイル名を入力して下さい\n" );
    char input_buf[ BUFFER_SIZE ];
    if( gets_s( input_buf, BUFFER_SIZE ) == NULL )
    {
        return false;
    }
    else
    {
        // @@@ TODO ファイルの存在をチェックする
        strcpy_s( input_param.filename, input_buf );
        menu_param->input_param = &input_param;
    }
    return true;
} // menu_file_open()

// eMENU_FILE_CLOSE
static bool
menu_file_close( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_FILE_CLOSE input_param;
    menu_param->menu_id = eMENU_FILE_CLOSE;
    menu_param->input_param = &input_param;

    return true;
} // menu_file_close()

// eMENU_FILE_SAVE
static bool
menu_file_save( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_FILE_SAVE input_param;
    menu_param->menu_id = eMENU_FILE_SAVE;

    printf( "保存ファイル名を入力して下さい\n" );
    char input_buf[ BUFFER_SIZE ];
    if( gets_s( input_buf, BUFFER_SIZE ) == NULL )
    {
        return false;
    }
    else
    {
        // @@@ TODO ファイルの存在をチェックする
        strcpy_s( input_param.filename, input_buf );
        menu_param->input_param = &input_param;
    }
    return true;
} // menu_file_save()

/**
    色操作系メニュー
 */
// eMENU_COLOR_CHG_MONO
static bool
menu_color_chg_mono( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_COLOR_CHG_MONO input_param;
    menu_param->menu_id = eMENU_COLOR_CHG_MONO;
    menu_param->input_param = &input_param;

    return true;
} // menu_color_chg_mono()

// eMENU_COLOR_CHG_SEPIA
static bool
menu_color_chg_sepia( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_COLOR_CHG_SEPIA input_param;
    menu_param->menu_id = eMENU_COLOR_CHG_SEPIA;
    menu_param->input_param = &input_param;

    return true;
} // menu_color_chg_sepia()

/**
    回転系メニュー
 */
// eMENU_ROTATE_L
static bool
menu_rotate( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_ROTATE input_param;
    menu_param->menu_id = eMENU_ROTATE;

    printf(
        "画像の回転角度を入力して下さい(反時計を正とする)\n"
        "\n"
        "=== 入力方法 ===\n"
        "(フォーマット) input > [角度]\n"
        "(入力例)       input > 60\n"
        "\n"
        "input > "
    );
    char input_buf[ BUFFER_SIZE ];
    if( gets_s( input_buf, BUFFER_SIZE ) == NULL )
    {
        return false;
    }
    else
    {
        if( sscanf_s( input_buf, "%d", &( input_param.angle ) ) != 1 )
        {
            return false;
        }
        menu_param->input_param = &input_param;
    }
    return true;
} // menu_rotate()

  /**
    変形系メニュー
 */
// eMENU_ZOOM
static bool
menu_zoom( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_ZOOM input_param;
    menu_param->menu_id = eMENU_ZOOM;

    printf(
        "画像の幅/高さの倍率を入力して下さい\n"
        "\n"
        "=== 入力方法 ===\n"
        "(フォーマット) input > [幅の倍率] [高さの倍率]\n"
        "(入力例)       input > 3.0 2.5\n"
        "\n"
        "input > "
    );
    char input_buf[ BUFFER_SIZE ];
    if( gets_s( input_buf, BUFFER_SIZE ) == NULL )
    {
        return false;
    }
    else
    {
        if( sscanf_s( input_buf, "%f %f",
            &( input_param.zoom_ratio_width ),
            &( input_param.zoom_ratio_width ) ) != 2 )
        {
            return false;
        }
        menu_param->input_param = &input_param;
    }

    return true;
} // menu_zoom()

// eMENU_RESIZE
static bool
menu_resize( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_RESIZE input_param;
    menu_param->menu_id = eMENU_RESIZE;

    printf(
        "変更後の画像の幅/高さを入力して下さい\n"
        "\n"
        "=== 入力方法 ===\n"
        "(フォーマット) input > [変更後の幅] [変更後の高さ]\n"
        "(入力例)       input > 640 480\n"
        "\n"
        "input > "
    );
    char input_buf[ BUFFER_SIZE ];
    if( gets_s( input_buf, BUFFER_SIZE ) == NULL )
    {
        return false;
    }
    else
    {
        if( sscanf_s( input_buf, "%d %d",
            &( input_param.width ),
            &( input_param.height) ) != 2 )
        {
            return false;
        }
        menu_param->input_param = &input_param;
    }

    return true;
} // menu_resize()

// eMENU_ROTATE_R
static bool
menu_demo( eMenuParam *menu_param )
{
    static eMENU_INPUT_RESULT_ input_param;
    menu_param->menu_id = eMENU_ID_DEMO;
    menu_param->input_param = &input_param;

    return true;
} // menu_dmeo()

/**
    メニュー関数
*/
typedef bool ( *func_t )( eMenuParam *menu_param );
func_t menu_func_color_chg[] = {
    menu_color_chg_mono,
    menu_color_chg_sepia,
};

func_t menu_func_rotate[] = {
    menu_rotate,
};

func_t menu_func_resize[] = {
    menu_zoom,
    menu_resize
};

static void
print_menu( void)
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
        " 回転             : 2000\n"
        "\n"

        "[変形]\n"
        " - 操作 -         : - 入力値 -\n"
        " 拡大/縮小        : 3000\n"
        " リサイズ         : 3001\n"
        "\n"

        "[その他]\n"
        " デモ表示         : D\n"
        " 終了             : Q\n"
    );
}

/**
    メニュー処理メイン
 */
void
UI_menu_main( eMenuParam *menu_param )
{
    bool menu_result = false;
    int input_char;
    char buf[ BUFFER_SIZE ];
    while( menu_result == false )
    {
        print_menu();
        printf( "input> " );
        if( gets_s( buf, BUFFER_SIZE ) == NULL )
        {
            puts( input_err_msg );
            continue;
        }

        menu_result = false;
        input_char = toupper( buf[ 0 ] );
        if( input_char == 'Q' )
        {
            printf( "アプリケーションを終了します\n" );
            menu_param->menu_id = eMENU_ID_END;
            menu_result = true;
            break;
        }
        else if( isalpha( input_char ) )
        {
            switch( input_char )
            {
            case 'O':
                menu_result = menu_file_open( menu_param );
                break;
            case 'C':
                menu_result = menu_file_close( menu_param );
                break;
            case 'S':
                menu_result = menu_file_save( menu_param );
                break;
            case 'D':
                menu_result = menu_demo( menu_param );
                break;
            default:
                puts( input_err_msg );
            }
        }
        else
        {
            int menu_id;
            sscanf_s( buf, "%d", &menu_id );
            if( ( menu_id >= 1000 ) && ( menu_id <= 1999 ) )
            {
                menu_result = menu_func_color_chg[ menu_id - 1000 ]( menu_param );
            }
            else if( ( menu_id >= 2000 ) && ( menu_id <= 2999 ) )
            {
                menu_result = menu_func_rotate[ menu_id - 2000 ]( menu_param );
            }
            else if( ( menu_id >= 3000 ) && ( menu_id <= 3999 ) )
            {
                menu_result = menu_func_resize[ menu_id - 3000 ]( menu_param );
            }
            else
            {
                puts( input_err_msg );
            }
        }
    }
}
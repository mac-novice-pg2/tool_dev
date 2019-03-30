#include "pch.h"

#include <cstdio>
#include <cstdlib>
#include <conio.h>

#include "calendar_def.h"
#include "MenuControl.h"

static void
menu_cursor_key_proc( int key, DateInfo *date )
{
    switch( key )
    {
    case 0x50: // "↓"
        if( date->year < END_OF_YEAR )
            date->year++;
        break;
    case 0x48: // "↑"
        if( date->year > 1 )
            date->year--;
        break;
    case 0x4D: // "→"
        if( date->month < END_OF_MONTH )
            date->month++;
        break;
    case 0x4B: // "←"
        if( date->month > 1 )
            date->month--;
        break;
    }
} // menu_cursor_key_proc()

void
Menu_ClearScreen( void )
{
    system( "cls" );
} // Menu_ClearScreen()

void
Menu_ShowInstruction( void )
{
    printf( "↑:去年 ↓:来年 →:来月 ←:先月 c:年月指定 q:終了\n" );
} // Menu_ShowInstruction()

bool
Menu_ChangeCalendar( DateInfo *date )
{
    bool request_quit = false;
    int key_1st = _getch(); // 1回目のキーコードを拾う
    int key_2nd = _getch(); // 2回目のキーコードを拾う
    switch( key_1st )
    {
    case 'q':
        request_quit = true;
        break;

    case 'c':
        Menu_Main( date );
        break;

    case 0xE0:
        menu_cursor_key_proc( key_2nd, date );
        Menu_ClearScreen();
        break;
    }

    return request_quit;
} // Menu_ChangeCalendar()

bool
Menu_Main( DateInfo *date )
{
    Menu_ClearScreen(); // 画面表示をクリアする
    printf(
        "表示年月日を入力して下さい\n"
        "ex) 2019 2\n"
        "> "
    );

    /*
     キーボードからの入力を受け付ける
      [fgets]
       キー入力やファイルから、指定された変数に入力を読み込む
      [sscanf]
       文字列から、空白を区切りとした部分を指定された変数に詰め直す
    */
    char input_buffer[ 256 ]; // 入力内容を入れる為の箱を作る

    // キー入力
    fgets( input_buffer, sizeof( input_buffer ), stdin );

    // input_bufferからyearとmonthに詰め直す
    // 以下のように、char型配列から整数型変数2つに詰め直す
    //  input_buffer : char型配列
    //  year         : int型
    //  month        : int型
    int year;
    int month;
    int input_value_count = sscanf( input_buffer, "%d %d", &year, &month );

    // 入力値チェック
    bool success = true; // 先ずは入力成功に設定しておく
    if( input_value_count != 2 )// 入力値が2になっている？
    {
        success = false;
    }
    // 年は1から9999の値が指定されている？
    if( ( year < 1 ) ||    // 年が   1より小さい？
        ( year > 9999 ) )  // 年が9999より大きい？
    {
        success = false;
    }
    // 月は1から12の値が指定されている？
    if( ( month < 1 ) ||  // 月が 1より小さい？
        ( month > 12 ) )  // 月が12より大きい？
    {
        success = false;
    }
    if( !success )
    {
        printf( "入力に誤りがあります。もう一度入力して下さい\n" );
    }
    else
    {
        date->year = year;
        date->month = month;
        Menu_ClearScreen(); // 画面をクリアして、表示更新に備える
    }

    return success;
} // Menu_Main()

#include "pch.h"

#include <cstdio>
#include <cstdlib>
#include <conio.h>

#include "calendar.h"
#include "CMonthInfo.h"
#include "user_interface.h"

void
CalendarControler::ClearScreen( void ) const
{
    system( "cls" );
} // CalendarControler::ClearScreen()

void
CalendarControler::cursor_key_proc( int key )
{
    switch( key )
    {
    case 0x50: // "↓"
        if( year_ < END_OF_YEAR )
            year_++;
        break;
    case 0x48: // "↑"
        if( year_ > 1 )
            year_--;
        break;
    case 0x4D: // "→"
        if( month_ < END_OF_MONTH )
            month_++;
        break;
    case 0x4B: // "←"
        if( month_ > 1 )
            month_--;
        break;
    }
} // CalendarControler::cursor_key_proc()

bool
CalendarControler::PrintMenu( void )
{
    ClearScreen(); // 画面表示をクリアする
    printf(
        "表示年月日を入力して下さい\n"
        "ex) 2019 2\n"
        "> "
    );

    char input_buffer[ 256 ];
    fgets( input_buffer, sizeof( input_buffer ), stdin );
    int input_value_count = sscanf( input_buffer, "%d %d", &year_, &month_ );

    // 入力値チェック
    bool success = true; // 先ずは入力成功に設定しておく
    if( input_value_count != 2 )// 入力値が2になっている？
    {
        success = false;
    }
    // 年は1から9999の値が指定されている？
    if( ( year_ <    1 ) || // 年が   1より小さい？
        ( year_ > 9999 ) )  // 年が9999より大きい？
    {
        success = false;
    }
    // 月は1から12の値が指定されている？
    if( ( month_ <  1 ) || // 月が 1より小さい？
        ( month_ > 12 ) )  // 月が12より大きい？
    {
        success = false;
    }
    if( !success )
    {
        printf( "入力に誤りがあります。もう一度入力して下さい\n" );
    }
    else
    {
        ClearScreen(); // 画面をクリアして、表示更新に備える
    }

    return success;
} // CalendarControler::PrintMenu()

void
CalendarControler::ShowInstruction( void )
{
    printf( "↑:去年 ↓:来年 →:来月 ←:先月 c:年月指定 q:終了\n" );
} // CalendarControler::PrintInstruction()

bool
CalendarControler::ChangeCalendar( void )
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
        PrintMenu();
        break;

    case 0xE0:
        cursor_key_proc( key_2nd );
        ClearScreen();
        break;
    }

    return request_quit;
} // CalendarControler::ChangeCalendar()

DateInfo
CalendarControler::GetInputDate( void )
{
    DateInfo date;
    date.year = year_;
    date.month = month_;

    return date;
} // CalendarControler::GetInputDate()

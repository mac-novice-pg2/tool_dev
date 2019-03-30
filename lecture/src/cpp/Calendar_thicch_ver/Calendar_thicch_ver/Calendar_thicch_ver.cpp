// Calendar_thicch_ver.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <conio.h>

#include "calendar_def.h"
#include "calendar_func.h"
#include "MenuControl.h"

/*
  ------------------------------------------
   static関数
  ------------------------------------------
*/
static void
print_skip_weekday( eWeekday start_weekday )//3
{
    // 頭の読み飛ばし
    /*
     eSun = 0,
     eMon = 1
     eTue = 2
     eWed = 3
     eThu = 4
     eFri = 5
     eSat = 6
    */

    // 2019年1月1日は火曜日
    // 従って日曜と月曜は不要なので、空白を入れてスキップする。
    // （1月1日の場合）0 < 2やから0と1の2セット分の空白3個分を入れる
    // 半角スペース3個 x 2セット = 合計6個のスペースが入る )
    for( int skip = 0; skip < start_weekday; skip++ )
    {
        printf( "   " );
    }
} // print_skip_weekday()

static void
print_calendar( DateInfo *date )//2
{
    printf( "%4d/%2dのカレンダー\n", date->year, date->month );
    printf( "日 月 火 水 木 金 土\n" );

    // 1月1日がどこから(火曜日)始まることを意味してる
    // その月が何曜日から始まるのかは、ツェラーの公式で取得する
    DateInfo cur = *date;
    cur.day = 1;
    eWeekday start_weekday = Cal_FormulaZeller( &cur );

    // 曜日の位置まで空白を詰める
    print_skip_weekday( start_weekday ); // 曜日の位置まで空白を詰める

    // 1日から始めて、月末までの日を表示する
    // 月末はGet_EndOfMonth()関数で取得出来る
    cur.weekday = start_weekday;
    int eom = Cal_GetEndOfMonth( &cur );
    for( int loop_cnt = 0; loop_cnt < eom; loop_cnt++ )
    {
        // [書式指定文字列]
        // 数字の出力幅は2桁(%2d)分取り、スペース1つを出力する
        printf( "%2d ", cur.day );

        if( cur.weekday == eSat ) // 今日は土曜日
        {
            printf( "\n" ); // 改行を入れる
        }
        Cal_StepDateInfo( &cur, eom );//■
    }
    printf( "\n\n" );
} // print_calendar()

int
main()// 1
{
#if 1
    DateInfo date;

    // メインメニューを表示する
    Menu_Main( &date );

    // apl_endがfalseになるまでカレンダー表示処理を行う
    bool apl_end = false;
    while( !apl_end )
    {
        // カレンダーを表示する
        print_calendar( &date );

        // 操作方法の表示
        Menu_ShowInstruction();

        // キー入力を受け付け、カレンダーを更新する
        apl_end = Menu_ChangeCalendar( &date );
    }
#else
    int year = 2020;
    for( int month = 1; month <= 12; month++ )
    {
        // カレンダーを表示する
        print_calendar( year, month );

        printf( "=================================\n" );
    }
#endif
    // getchar()はEnterキーを押すまで待つ
    printf( "Enterキーを押すと終了します" );
    getchar();
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

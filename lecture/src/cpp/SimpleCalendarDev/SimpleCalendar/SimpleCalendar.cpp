// SimpleCalendar.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

#include "calendar.h"
#include "calendar_func.h"
#include "CMonthInfo.h"
#include "CalendarControler.h"

#define TEST_MODE
int
main()
{
    CalendarControler controler;
    CHolidayManager holiday;

    int year = 2019;
    int month = 1;
#ifndef TEST_MODE
    controler.PrintMenu();
#endif
    bool apl_end = false;
    while( !apl_end )
    {
#ifndef TEST_MODE
        year = controler.GetInputYear();
        month = controler.GetInputMonth();
#endif
        PrintToday();
        PrintEventAlert( year, month, &holiday );
        PrintCalendar( year, month );
        apl_end = controler.ChangeCalendar();
    }
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//   1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

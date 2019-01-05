// Calendar_thicch_ver.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

#include <cstdio>
#include <cassert>

/*
  ------------------------------------------
   型定義
  ------------------------------------------
*/
// 曜日を示す列挙型
typedef enum
{
    eSun = 0,
    eMon,
    eTue,
    eWed,
    eThu,
    eFri,
    eSat,
    eWeekEnd,
}eWeekday;

// カレンダー作成用情報
typedef struct
{
    int      eom;    // 月末( end of month )
    eWeekday weekday; // 開始曜日
}MonthInfo;

/*
  ------------------------------------------
   static関数
  ------------------------------------------
*/
static eWeekday
next_weekday( eWeekday current )
{
    switch( current )
    {
    case eSun:
        return eMon;
    case eMon:
        return eTue;
    case eTue:
        return eWed;
    case eWed:
        return eThu;
    case eThu:
        return eFri;
    case eFri:
        return eSat;
    case eSat:
        return eSun;
    default:
        assert( 0 );
        return eWeekEnd;
    }
} // next_weekday()

int
main( int argc, const char* argv[] )
{
    MonthInfo
    eom_table[] = {
        // eom, weekday
        {  31,  eTue }, // 2019/01
        {  28,  eFri }, // 2019/02
        {  31,  eFri }, // 2019/03
        {  30,  eMon }, // 2019/04
        {  31,  eWed }, // 2019/05
        {  30,  eSat }, // 2019/06
        {  31,  eMon }, // 2019/07
        {  31,  eThu }, // 2019/08
        {  30,  eSun }, // 2019/09
        {  31,  eTue }, // 2019/10
        {  30,  eFri }, // 2019/11
        {  31,  eSun }, // 2019/12
    };

    for( int month = 0; month < 12; month++ )
    {
        printf( "%d月のカレンダー\n", month + 1 );
        printf( "日 月 火 水 木 金 土\n" );

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

        //１月１日が火曜日だから、日曜と月曜は要らないから空白をいれるためにスキップする。
        //（１月1日の場合）0 < 2やから0と1の二セット分の空白３個分を入れる
        // 半角スペース3個 x 2セット = 合計6個のスペースが入る )
        for( int skip = 0; skip < eom_table[ month ].weekday; skip++ )
        {
            printf( "   " );
        }

        //1月１日がどこから(火曜日)始まることを意味してる
        eWeekday current_weekday = eom_table[ month ].weekday;

        // １日から始まって、月末を表示
        for( int day = 1; day <= eom_table[ month ].eom; day++ )
        {
            //数字２桁分取る
            printf( "%2d ", day );

            //next_weekdayっていう関数
            //土曜日で改行
            current_weekday = next_weekday( current_weekday );
            if( current_weekday == eSun )
            {
                printf( "\n" );
            }
        }
        printf( "\n\n" );
    }

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

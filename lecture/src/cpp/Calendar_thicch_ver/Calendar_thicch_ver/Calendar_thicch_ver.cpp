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
    int      eom;     // 月末( end of month )
    eWeekday start_weekday; // 開始曜日
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
        // eom, start_weekday
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

        // 2019年1月1日は火曜日
        // 従って日曜と月曜は不要なので、空白を入れてスキップする。
        // （1月1日の場合）0 < 2やから0と1の2セット分の空白3個分を入れる
        // 半角スペース3個 x 2セット = 合計6個のスペースが入る )
        for( int skip = 0; skip < eom_table[ month ].start_weekday; skip++ )
        {
            printf( "   " );
        }

        // 1月1日がどこから(火曜日)始まることを意味してる
        // その月が何曜日から始まるのかは、
        // eom_table配列の[ 月 ].start_weekdayメンバに入っている
        eWeekday current_weekday = eom_table[ month ].start_weekday;
        eWeekday next;

        // 1日から始めて、月末までの日を表示する
        // 月末はeom_table配列の[ 月 ].eomメンバに入っている
        for( int day = 1; day <= eom_table[ month ].eom; day++ )
        {
            // [書式指定文字列]
            // 数字の出力幅は2桁(%2d)分取り、スペース1つを出力する
            printf( "%2d ", day );

            // next_weekday()という関数は、現在の曜日を渡すと、次の曜日を返す
            //  ex) next_weekday( eMon ) // eMonを渡すとeTueが返ってくる
            next = next_weekday( current_weekday );
            // 土曜日で改行する
            // next_weekday()に今日の曜日を渡すと明日の曜日が得られるので、
            //  「明日が日曜日 = 今日は土曜日」
            // という風に判定出来る
            if( next == eSun ) // 明日は日曜日( 今日は土曜日？ )
            {
                printf( "\n" ); // 改行を入れる
            }
            current_weekday = next; // 今日の曜日を更新する
        }
        printf( "\n\n" );
    }

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

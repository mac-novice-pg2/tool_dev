#pragma once

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
    int      last;    // 月末
    eWeekday weekday; // 開始曜日
}Month_Info_t;

// 関数軍のプロとライプ宣言
void print_calener( int month );

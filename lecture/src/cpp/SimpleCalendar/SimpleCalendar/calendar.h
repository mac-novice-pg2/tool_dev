#pragma once

/*
  ------------------------------------------
   マクロ定義
  ------------------------------------------
*/
#define EVENT_ITEM_MAX ( 8 )
#define EVENT_END      ( 0xFFFFFFFFU )

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
    int      last;    // 月末
    eWeekday weekday; // 開始曜日
}MonthInfo;

// 今日の年月日情報
typedef struct
{
    int year;
    int month;
    int day;
    eWeekday weekday;
}TodayInfo;

typedef struct
{
    int day;
    const char *event_name;
    bool holiday;
}EventInfo;

// APIプロトタイプ宣言
void PrintToday( void );
void PrintCalendar( int month );
void EventAlert( int month );

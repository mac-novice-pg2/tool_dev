#pragma once

/*
  ------------------------------------------
   マクロ定義
  ------------------------------------------
*/
#define SALARY_DAY     ( 25 )
#define NOT_FOUND      ( 0xFFFFFFFF )
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
    int      eom;    // 月末( end of month )
    eWeekday start_weekday; // 開始曜日
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
    int day; // イベント日
    const char *event_name; // イベント名
    bool is_holiday; // true:休日 false:平日
}EventInfo;

// APIプロトタイプ宣言
void PrintToday( void );
void PrintCalendar( int year, int month );
void PrintEventAlert( int year, int month );

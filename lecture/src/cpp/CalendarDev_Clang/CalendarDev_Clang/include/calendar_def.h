#pragma once

/*
  ------------------------------------------
   マクロ定義
  ------------------------------------------
*/
#define ARRAY_SIZE( arr ) ( sizeof( arr ) / sizeof( arr[ 0 ] ) )

#define SALARY_DAY     ( 25 )
#define NOT_FOUND      ( 0xFFFFFFFF )
#define EVENT_ITEM_MAX ( 8 )
#define EVENT_END      ( 0xFFFFFFFFU )
#define END_OF_YEAR    ( 9999 )
#define END_OF_MONTH   ( 12 )

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

// 今日の年月日情報
struct DateInfo
{
    int year;
    int month;
    int day;
    eWeekday weekday;
};

struct EventInfo
{
    DateInfo date;
    char event_name[ 64 ];
    bool is_holiday;
    bool is_valid;
};

#include "pch.h"

#include "calendar.h"

MonthInfo month_info_2019[] = {
    { 31, eTue }, // 2019/01
    { 28, eFri }, // 2019/02
    { 31, eFri }, // 2019/03
    { 30, eMon }, // 2019/04
    { 31, eWed }, // 2019/05
    { 30, eSat }, // 2019/06
    { 31, eMon }, // 2019/07
    { 31, eThu }, // 2019/08
    { 30, eSun }, // 2019/09
    { 31, eTue }, // 2019/10
    { 30, eFri }, // 2019/11
    { 31, eSun }, // 2019/12
};

EventInfo event_info_2019[][ EVENT_ITEM_MAX ] = {
    { // 2019/1
        { 1, "元旦", true },
        { 7, "七草の日", false },
        { 14,"成人の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/2
        { 3, "節分", false },
        { 4, "立春", false },
        { 11, "建国記念日", true },
        { 14, "バレンタインデー", false },
        { EVENT_END,nullptr, false },
    },
    { // 2019/3
        { 3, "ひな祭り", false },
        { 4, "立春", false },
        { 14, "ホワイトデー", false },
        { 21, "春分の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/4
        { 1, "エイプリルフール", false },
        { 29, "昭和の日", true },
        { EVENT_END,nullptr },
    },
    { // 2019/5
        { 1, "メーデー", false },
        { 3, "憲法記念日", true },
        { 4, "みどりの日", true },
        { 5, "こどもの日", true },
        { 6, "振替休日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/6
        { 1, "衣替え", false },
        { EVENT_END,nullptr, false },
    },
    { // 2019/7
        { 7, "七夕", false },
        { 15, "海の日", true },
        { 17, "祇園祭", false },
        { 25, "天神祭", false },
        { EVENT_END,nullptr, false },
    },
    { // 2019/8
        { 11, "山の日", true },
        { 12, "振替休日", true },
        { 15, "終戦記念日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/9
        { 15, "月見", false },
        { 16, "敬老の日", true },
        { 23, "秋分の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/10
        { 1, "衣替え", false },
        { 14, "体育の日", true },
        { 23, "秋分の日", true },
        { EVENT_END,nullptr, false },
    },
    { // 2019/12
        { 23, "天皇誕生日？", true },
        { 24, "クリスマスイブ", false },
        { 25, "クリスマス", false },
        { 31, "大晦日", false },
        { EVENT_END,nullptr, false },
    },
};
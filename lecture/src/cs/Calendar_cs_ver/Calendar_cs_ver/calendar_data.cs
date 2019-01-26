using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Calendar_cs_ver
{
    // 曜日を示す列挙型
    public enum 曜日
    {
        日, 月, 火, 水, 木, 金, 土, 週末,
    }

    public class 月情報
    {
        public int 月末;     // 月末( end of month )
        public 曜日 開始曜日; // 開始曜日
        public 月情報(int 月末, 曜日 開始曜日)
        {
            this.月末 = 月末;
            this.開始曜日 = 開始曜日;
        }
    }

    public class 本日の情報
    {
        public int 年;
        public int 月;
        public int 日;
        public 曜日 曜日;
        public 本日の情報(int 年, int 月, int 日, 曜日 曜日)
        {
            this.年 = 年;
            this.月 = 月;
            this.日 = 日;
            this.曜日 = 曜日;
        }
    }

    public class イベント情報
    {
        public uint イベント日;
        public string イベント名;
        public bool 休日判定;
        public イベント情報(uint イベント日, string イベント名, bool 休日判定)
        {
            this.イベント日 = イベント日;
            this.イベント名 = イベント名;
            this.休日判定 = 休日判定;
        }
    }

    public class カレンダー情報
    {
        public static readonly int 給料日 = 25;
        public static List<月情報> 月情報テーブル = new List<月情報>
        {
            // 月末, 開始曜日
            new 月情報( 31, 曜日.火 ), // 2019/01
            new 月情報( 28, 曜日.金 ), // 2019/02
            new 月情報( 31, 曜日.金 ), // 2019/03
            new 月情報( 30, 曜日.月 ), // 2019/04
            new 月情報( 31, 曜日.水 ), // 2019/05
            new 月情報( 30, 曜日.土 ), // 2019/06
            new 月情報( 31, 曜日.月 ), // 2019/07
            new 月情報( 31, 曜日.木 ), // 2019/08
            new 月情報( 30, 曜日.日 ), // 2019/09
            new 月情報( 31, 曜日.火 ), // 2019/10
            new 月情報( 30, 曜日.金 ), // 2019/11
            new 月情報( 31, 曜日.日 ), // 2019/12
        };
        public static List<イベント情報>[] イベントリスト2019年版 = new List<イベント情報>[]
        {
            new List<イベント情報> // 2019/1
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報( 1, "元旦", true ),
                new イベント情報(  7, "七草の日", false ),
                new イベント情報(  14,"成人の日", true ),
            },
            new List<イベント情報> // 2019/1
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  3, "節分", false ),
                new イベント情報(  4, "立春", false ),
                new イベント情報(  11, "建国記念日", true ),
                new イベント情報(  14, "バレンタインデー", false ),
            },
            new List<イベント情報> // 2019/3
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  3, "ひな祭り", false ),
                new イベント情報(  4, "立春", false ),
                new イベント情報(  14, "ホワイトデー", false ),
                new イベント情報(  21, "春分の日", true ),
            },
            new List<イベント情報> // 2019/4
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報( 1, "エイプリルフール", false ),
                new イベント情報(  29, "昭和の日", true ),
            },
            new List<イベント情報> // 2019/5
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  1, "メーデー", false ),
                new イベント情報(  3, "憲法記念日", true ),
                new イベント情報(  4, "みどりの日", true ),
                new イベント情報(  5, "こどもの日", true ),
                new イベント情報(  6, "振替休日", true ),
            },
            new List<イベント情報> // 2019/6
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  1, "衣替え", false ),
            },
            new List<イベント情報> // 2019/7
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  7, "七夕", false ),
                new イベント情報(  15, "海の日", true ),
                new イベント情報(  17, "祇園祭", false ),
                new イベント情報(  25, "天神祭", false ),
            },
            new List<イベント情報> // 2019/8
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  11, "山の日", true ),
                new イベント情報(  12, "振替休日", true ),
                new イベント情報(  15, "終戦記念日", true ),
            },
            new List<イベント情報> // 2019/9
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  15, "月見", false ),
                new イベント情報(  16, "敬老の日", true ),
                new イベント情報(  23, "秋分の日", true ),
            },
            new List<イベント情報> // 2019/10
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  1, "衣替え", false ),
                new イベント情報(  14, "体育の日", true ),
                new イベント情報(  23, "秋分の日", true ),
            },
            new List<イベント情報> // 2019/11
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  3, "文化の日", true ),
                new イベント情報(  15, "七五三", false ),
                new イベント情報(  23, "勤労感謝の日", true ),
                new イベント情報(  28, "感謝祭", false ),
            },
            new List<イベント情報> // 2019/12
            {
                // { イベント日, イベント名, 休日判定}
                new イベント情報(  23, "天皇誕生日？", true ),
                new イベント情報(  24, "クリスマスイブ", false ),
                new イベント情報(  25, "クリスマス", false ),
                new イベント情報(  31, "大晦日", false ),
            },
        };
    }
}
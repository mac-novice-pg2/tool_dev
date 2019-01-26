using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Calendar_cs_ver
{
    class CalendarMain
    {
        static 曜日 次の曜日に進める(曜日 現在の曜日)
        {
            switch( 現在の曜日 )
            {
            case 曜日.日: return 曜日.月;
            case 曜日.月: return 曜日.火;
            case 曜日.火: return 曜日.水;
            case 曜日.水: return 曜日.木;
            case 曜日.木: return 曜日.金;
            case 曜日.金: return 曜日.土;
            case 曜日.土: return 曜日.日;
            default:
                Debug.Assert( false );
                return 曜日.週末;
            }
        } // 次の曜日に進める()

        static void 一日進める(本日の情報 本日, int 月末)
        {
            if( 本日.日 != 月末 )
                本日.日++;
            else // 今日は月末？
            {
                本日.日 = 1;
                本日.月++;
                if( 本日.月 == 13 ) // 年末？
                {
                    本日.月 = 1;
                    本日.年++;
                }
            }
            本日.曜日 = 次の曜日に進める( 本日.曜日 );
        } // 一日進める()

        static イベント情報
        イベントリスト検索(本日の情報 本日)
        {
            イベント情報 検索結果 = null;
            foreach( var イベント in カレンダー情報.イベントリスト2019年版[ 本日.月 - 1 ] )
            {
                if( イベント.イベント日 == 本日.日 )
                    検索結果 = イベント;
            }
            return 検索結果;
        } // search_event_list()

        static uint 曜日からスキップ数を算出(曜日 開始曜日)
        {
            switch( 開始曜日 )
            {
            case 曜日.日: return 0;
            case 曜日.月: return 1;
            case 曜日.火: return 2;
            case 曜日.水: return 3;
            case 曜日.木: return 4;
            case 曜日.金: return 5;
            case 曜日.土: return 6;
            default:
                System.Diagnostics.Debug.Assert( false );
                return 0xdeadbeaf;
            }
        }

        static void
        曜日読み飛ばし(曜日 開始曜日)
        {
            uint スキップ数 = 曜日からスキップ数を算出( 開始曜日 );
            for( uint ステップ = 0; ステップ < スキップ数; ステップ++ )
                Console.Write( "   " );
        }

        static void
        カレンダー表示(int 年, int 月)//2
        {
            Console.WriteLine( "{0}月のカレンダー", 月 );
            Console.WriteLine( "日 月 火 水 木 金 土" );

            // 1月1日がどこから(火曜日)始まることを意味してる
            // その月が何曜日から始まるのかは、
            // CalendarData.月情報テーブル[ 月 - 1 ].開始曜日に入っている
            曜日 開始曜日 = カレンダー情報.月情報テーブル[ 月 - 1 ].開始曜日;

            曜日読み飛ばし( 開始曜日 ); // 曜日の位置まで空白を詰める

            // 1日から始めて、月末までの日を表示する
            // 月末はCalendarData.月情報テーブル[ 月 - 1 ].月末に入っている
            本日の情報 本日 = new 本日の情報( 年, 月, 1, 開始曜日 );
            int 月末 = カレンダー情報.月情報テーブル[ 月 - 1 ].月末;
            for( int ループ数 = 0; ループ数 < 月末; ループ数++ )
            {
                // [書式指定文字列]
                // 数字の出力幅は2桁分取り、スペース1つを出力する
                Console.Write( "{0:D2} ", 本日.日 );
                if( 本日.曜日 == 曜日.土 )
                    Console.WriteLine( "" ); // 改行を入れる
                一日進める( 本日, 月末 );
            }
            Console.WriteLine( "\n" );
        }

        static void
        イベント表示(int 年, int 月)
        {
            Console.WriteLine( "{0:D2}月のイベント日\n", 月 );
            Console.WriteLine( "---------------------------" );

            // 1月1日がどこから(火曜日)始まることを意味してる
            // その月が何曜日から始まるのかは、
            // CalendarData.月情報テーブル[ 月 - 1 ].開始曜日メンバに入っている
            曜日 開始曜日 = カレンダー情報.月情報テーブル[ 月 - 1 ].開始曜日;

            本日の情報 本日 = new 本日の情報( 年, 月, 1, 開始曜日 );
            int 月末 = カレンダー情報.月情報テーブル[ 月 - 1 ].月末;
            while( 本日.日 < 月末 )
            {
                var イベント = イベントリスト検索( 本日 );
                if( イベント != null )
                    Console.WriteLine( "{0:D2}/{1:D2}は{2}です", 本日.月, 本日.日, イベント.イベント名 );
                一日進める( 本日, 月末 );
            }
            Console.WriteLine();
        }

        static void Main(string[] args)
        {
            int 年 = 2019;
            for( int 月 = 1; 月 <= 12; 月++ )
            {
                カレンダー表示( 年, 月 );
                イベント表示( 年, 月 );
                Console.WriteLine( "=================================" );
            }
        }
    }
}

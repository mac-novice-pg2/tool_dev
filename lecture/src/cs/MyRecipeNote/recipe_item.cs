using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyRecipeNote
{
    public class 料理情報
    {
        public int ItemNo { get; set; }
        // 検索用アイテム
        public string 料理名 { get; set; }
        public decimal 調理時間 { get; set; }
        public decimal 費用 { get; set; }
        public decimal kcal { get; set; }
        public string ジャンル { get; set; } // 列挙型
        public string 形式 { get; set; }// 列挙型
        public string 難易度 { get; set; }
        public string 材料 { get; set; }
        public string シーズン { get; set; } // 列挙型

        public string レシピ { get; set; }
        public string 調理器具 { get; set; }


        public static string read_entry(System.IO.StreamReader sr, string next_item)
        {
            var buf = sr.ReadLine();
            if ((buf = sr.ReadLine()) != next_item)
                throw new ArgumentException();
            return buf;
        }
            
        public static string read_multiline_entry(System.IO.StreamReader sr, string next_item)
        {
            var sb = new StringBuilder();
            var buf = "";

            while ((buf = sr.ReadLine()) != next_item)
            {
                if (buf == null)
                    throw new ArgumentException();
                sb.Append(buf + "\n");
            }
            return sb.ToString();
        }

        private string add_escape(string s)
        {
            if (System.Text.RegularExpressions.Regex.IsMatch(s, @"^[<|\[|=]"))
            {
                return "$$$" + s;
            }
            return s;
        }

        public void WriteItem(System.IO.StreamWriter sw)
        {
            sw.WriteLine("[ITEM:{0:D5}]",ItemNo );
            sw.WriteLine("=name=\n{0}", add_escape( 料理名 ));
            sw.WriteLine("=time=\n{0}", 調理時間);
            sw.WriteLine("=cost=\n{0}", 費用);
            sw.WriteLine("=kcal=\n{0}", kcal);
            sw.WriteLine("=genre=\n{0}", add_escape(ジャンル));
            sw.WriteLine("=type=\n{0}", add_escape(形式));
            sw.WriteLine("=difficulty=\n{0}", add_escape(難易度));
            sw.WriteLine("=season=\n{0}", add_escape(シーズン));
            sw.WriteLine("=kitchenware=\n{0}", add_escape(調理器具));

            sw.WriteLine("<recipe>\n{0}", add_escape(レシピ));
            sw.WriteLine("<ingredients>\n{0}", add_escape(材料));

            sw.WriteLine("<EOI>");
        }
        public static 料理情報 ReadItem(System.IO.StreamReader sr)
        {
            料理情報 r = new 料理情報();
            string buf = sr.ReadLine();

            if (buf == null) // データファイル終端検出時はnullを返しておく
            {
                return null;
            }

            // アイテム検索
            while ( ( buf = sr.ReadLine() ) != null )
            {
                if (buf == null)
                    throw new ArgumentException();
                if (buf.StartsWith("[ITEM:"))
                    break;
            }

            // nameエントリ読み出し
            buf = sr.ReadLine(); // =name=読み捨て

            r.料理名 = read_entry(sr, "=time=");
            r.調理時間 = Convert.ToDecimal( read_entry(sr, "=cost=") );
            r.費用 = Convert.ToDecimal(read_entry(sr, "=kcal="));
            r.kcal = Convert.ToDecimal(read_entry(sr, "=genre="));
            r.ジャンル = read_entry(sr, "=type=");
            r.形式 = read_entry(sr, "=difficulty=");
            r.難易度 = read_entry(sr, "=ingredients=");
            r.材料 = read_multiline_entry(sr, "=season=");
            r.シーズン = read_multiline_entry(sr, "<recipe>");
            r.レシピ = read_multiline_entry(sr, "<kitchenware>");
            r.調理器具 = read_multiline_entry(sr, "<EOI>");

            return r;
        }
    }
}

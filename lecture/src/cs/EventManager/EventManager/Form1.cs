using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
using System.Windows.Forms;

namespace EventManager
{
    public partial class Form1 : Form
    {
        string event_filename_ = "event.csv";
        Encoding file_enc_ = Encoding.GetEncoding( "shift_jis" );

        public Form1()
        {
            InitializeComponent();
            イベント表更新();
        }

        private void イベント表更新()
        {
            イベント表.Clear();
            using( var sr = new StreamReader( event_filename_, file_enc_ ) )
            {
                イベント表.Text = sr.ReadToEnd();
            }
        }

        private void イベントカレンダー_DateSelected(object sender, DateRangeEventArgs e)
        {
            イベント日テキスト.Text = e.Start.ToString( "yyyy/mm/dd" );
        }

        private void 登録ボタン_Click(object sender, EventArgs e)
        {
            using( var sw = new StreamWriter( event_filename_, true, file_enc_ ) )
            {
                sw.WriteLine( string.Format( "{0},{1}",
                イベント日テキスト.Text,
                イベント内容テキスト.Text ) );
            }
            イベント表更新();
        }
    }
}

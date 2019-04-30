using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MyRecipeNote
{
    public partial class レシピ作成フォーム : Form
    {
        private string recipe_fname_ = "recipe.txt";
        private List<料理情報> my_list = new List<料理情報>();
        public レシピ作成フォーム()
        {
            InitializeComponent();
            load_item();
        }

        private void load_item()
        {
            if ( Directory.Exists(recipe_fname_) )
            {
                var item = new 料理情報();
                using (var sr = new StreamReader(recipe_fname_))
                {
                    while( ( item = 料理情報.ReadItem(sr) ) != null )
                        my_list.Add(item);
                }
            }
        }

        private void 料理登録ボタン_Click(object sender, EventArgs e)
        {
            using (var sw = new StreamWriter(recipe_fname_,
                true,
                Encoding.GetEncoding("Shift_JIS")))
            {
                var item = new 料理情報();
                item.料理名 = 料理名ボックス.Text;
                item.レシピ = レシピボックス.Text;
                item.材料 = 材料ボックス.Text;
                item.ジャンル = 料理ジャンルボックス.Text;
                item.形式 = ジャンルボックス.Text;
                item.シーズン = 旬ボックス.Text;
                item.調理器具 = 調理器具ボックス.Text;
                item.調理時間 = 調理時間ボックス.Value;
                item.費用 = 費用ボックス.Value;
                item.kcal = kcalボックス.Value;
                item.難易度 = 難易度ボックス.Text;
                my_list.Add(item);
                item.ItemNo = my_list.Count;
                item.WriteItem(sw);
            }
        }
    }
}

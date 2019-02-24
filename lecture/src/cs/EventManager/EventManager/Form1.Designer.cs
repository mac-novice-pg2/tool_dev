namespace EventManager
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if( disposing && ( components != null ) )
            {
                components.Dispose();
            }
            base.Dispose( disposing );
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.イベントカレンダー = new System.Windows.Forms.MonthCalendar();
            this.イベント表 = new System.Windows.Forms.TextBox();
            this.登録ボタン = new System.Windows.Forms.Button();
            this.イベント日テキスト = new System.Windows.Forms.TextBox();
            this.イベント内容テキスト = new System.Windows.Forms.TextBox();
            this.イベント日ラベル = new System.Windows.Forms.Label();
            this.イベント内容ラベル = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // イベントカレンダー
            // 
            this.イベントカレンダー.Location = new System.Drawing.Point(18, 12);
            this.イベントカレンダー.Name = "イベントカレンダー";
            this.イベントカレンダー.TabIndex = 0;
            this.イベントカレンダー.DateSelected += new System.Windows.Forms.DateRangeEventHandler(this.イベントカレンダー_DateSelected);
            // 
            // イベント表
            // 
            this.イベント表.Location = new System.Drawing.Point(223, 12);
            this.イベント表.Multiline = true;
            this.イベント表.Name = "イベント表";
            this.イベント表.Size = new System.Drawing.Size(260, 262);
            this.イベント表.TabIndex = 1;
            // 
            // 登録ボタン
            // 
            this.登録ボタン.Location = new System.Drawing.Point(18, 242);
            this.登録ボタン.Name = "登録ボタン";
            this.登録ボタン.Size = new System.Drawing.Size(75, 23);
            this.登録ボタン.TabIndex = 2;
            this.登録ボタン.Text = "イベント登録";
            this.登録ボタン.UseVisualStyleBackColor = true;
            this.登録ボタン.Click += new System.EventHandler(this.登録ボタン_Click);
            // 
            // イベント日テキスト
            // 
            this.イベント日テキスト.Location = new System.Drawing.Point(85, 183);
            this.イベント日テキスト.Name = "イベント日テキスト";
            this.イベント日テキスト.Size = new System.Drawing.Size(132, 19);
            this.イベント日テキスト.TabIndex = 3;
            // 
            // イベント内容テキスト
            // 
            this.イベント内容テキスト.Location = new System.Drawing.Point(85, 208);
            this.イベント内容テキスト.Name = "イベント内容テキスト";
            this.イベント内容テキスト.Size = new System.Drawing.Size(132, 19);
            this.イベント内容テキスト.TabIndex = 4;
            // 
            // イベント日ラベル
            // 
            this.イベント日ラベル.AutoSize = true;
            this.イベント日ラベル.Location = new System.Drawing.Point(16, 186);
            this.イベント日ラベル.Name = "イベント日ラベル";
            this.イベント日ラベル.Size = new System.Drawing.Size(53, 12);
            this.イベント日ラベル.TabIndex = 5;
            this.イベント日ラベル.Text = "イベント日";
            // 
            // イベント内容ラベル
            // 
            this.イベント内容ラベル.AutoSize = true;
            this.イベント内容ラベル.Location = new System.Drawing.Point(16, 211);
            this.イベント内容ラベル.Name = "イベント内容ラベル";
            this.イベント内容ラベル.Size = new System.Drawing.Size(65, 12);
            this.イベント内容ラベル.TabIndex = 6;
            this.イベント内容ラベル.Text = "イベント内容";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(493, 279);
            this.Controls.Add(this.イベント内容ラベル);
            this.Controls.Add(this.イベント日ラベル);
            this.Controls.Add(this.イベント内容テキスト);
            this.Controls.Add(this.イベント日テキスト);
            this.Controls.Add(this.登録ボタン);
            this.Controls.Add(this.イベント表);
            this.Controls.Add(this.イベントカレンダー);
            this.Name = "Form1";
            this.Text = "イベントマネージャー";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MonthCalendar イベントカレンダー;
        private System.Windows.Forms.TextBox イベント表;
        private System.Windows.Forms.Button 登録ボタン;
        private System.Windows.Forms.TextBox イベント日テキスト;
        private System.Windows.Forms.TextBox イベント内容テキスト;
        private System.Windows.Forms.Label イベント日ラベル;
        private System.Windows.Forms.Label イベント内容ラベル;
    }
}


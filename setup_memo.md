# 種々雑多なメモ

## 開発環境

- ### バージョン管理

  - 手元に32bit環境が無い為、下記説明は全て64bit版のものとします
  - Git( GitHubにリポジトリを用意 )
  - SourceTree

- ビルド/実行環境(統合開発環境。IDEとも呼びます)

  - Visual Studio 2017

- 開発言語

  - C/C++

- 使用ライブラリ

  - OpenCV

## 各種セットアップ

### SourceTree

1. AtlassianのHPに行く
   1. "SourceTree"で検索すると、おそらく一番上に出てきます
   2. ページ一番下のリソース列の更に一番下の**マイアカウント**を選択
   3. 次のページ一番"Sign up for an account"をクリックし、以下の情報を入力
      - 有効なEmailアドレス
      - フルネーム
      - パスワード
      - captchaで画像認識と格闘
2. HPからSourceTreeのセットアップファイル入手
3. インストーラ起動
   - アカウント選択画面でAtlassianアカウント**(2018/11/11更新)**(注1)を選択。以下の情報を入力
     - Emailアドレス
     - パスワード
   - あとはデフォルトのまま進めればOK
     - 注1)SourceTreeインストール時にBitBucketアカウントが必要になった模様
       - インストール後のGitHub切り替えは可能なので、一旦BitBucket登録を行います
         1. 下記Atlassianアカウントの部分をBitBucket(Serverじゃない方)を選択
         2. その先からBitBucketアカウントを登録(といっても、Atlassianアカウントと紐付くだけ)
         3. 改めてBitBucketアカウントでインストールを進める

### OpenCV

- ダウンロード
  - Windows用は[こちら](https://opencv.org/opencv-3-4-1.html)のページの下の方にある**Win pack**をクリックするとダウンロードが開始されます
  - ダウンロード後、適当なディレクトリに置く
- インストール
  - OpenCVインストール先ディレクトリを用意します。ここでは、以下のようなディレクトリにしました
    - ディレクトリ：C:\devel\lib
  - ファイルをダブルクリックすると、圧縮ファイルが展開されるので、上記ディレクトリを選択します
    - c:\devel\lib\opencvとしてインストール(ただの解凍)されます
  - [マイコンピュータ]を右クリックして、[プロパティ]を選択します([Win] + [Pause/Break]でも可)
    - [システムの詳細設定]をクリックします([システムのプロパティ]ダイアログが表示される)
    - [詳細設定]タブを選び、[環境変数]を選択します
    - ユーザー環境変数にPATHに以下の文字列を追加します
      - C:\devel\lib\opencv\build\x64\vc15\bin
      - Win10では(入力画面が違うので)そのまま追加出来ますが、Win7の場合は**既存の文字列の末尾に区切り文字となるセミコロン';'を入力してから**追加して下さい
    - 環境変数PATHが存在しない場合は[新規]を選んで以下のように追加して下さい
      - 名前：PATH
      - %PATH%;C:\devel\lib\opencv\build\x64\vc15\bin
- 使い方
  - Visual Studio等にインクルード/ライブラリパスを設定すると使えるようになります
    - [Visual Studioプロジェクト作成](create_vc_proj.md)で詳しく説明します

# GitHubリポジトリ情報

## .gitignore

Visual Studio

## license

BSD-3-Clause …

### クローンするリポジトリは？

リポジトリページ右側の緑ボタン**Clone or download**から
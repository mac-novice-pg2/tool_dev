# Visual Studio 2017

Visual Studioでは、プロジェクトという単位でソースコードや実行ファイルを管理します。という訳で先ずは練習ソフト用プロジェクトを作成します

とはいえ、OpenCVの組み込みは色々とややこしい点がありますので、例えば開発に慣れている方であってもつまづくポイントが発生するかもしれません。そこで、一通り設定したプロジェクトのひな形を[こちら](opencv_template.zip)にご用意しましたので、ひな形をベースに構築されるのが良いかと思います

### 32bit環境について

大変申し訳ございませんが、手元にWindows10 64bitの環境しかない為、特に32bit環境については確認不十分です。一応の配慮をしたつもりですが、不十分な点が含まれる可能性がある事をご了承願います

## 開発用プロジェクト作成

Visual Studioを起動したら、[ファイル] -> [新規作成] -> [プロジェクト]を選んで下さい

今回は取りあえず以下のようにしておきました

- Visual C++
  - プロジェクト形式：Windowsコンソールアプリケーション
  - 名前：ocv_test
  - 場所：任意(ソースコードを置きたい場所)

## OpenCVセットアップ

セットアップ説明ページでも触れましたが、OpenCVをプロジェクトのインクルード/ライブラリとして登録する必要があります。OpenCVは**C:\devel\library\open_cv\3.4.1**に置いてあるものとします

- ocv_testプロジェクトを開く
  - 画面右端の[ソリューションエクスプローラー]をクリック
  - ocv_testを選んで右クリックして[プロパティ]を選択
    - 左ペインの[構成プロパティ] -> [デバッグ]を選択

      - 右ペイン(中央辺りの)[環境]の欄に以下の文字列を入力する
        - PATH=C:\devel\library\open_cv\3.4.1\build\x64\vc15\bin;%PATH%

    - 左ペインの[構成プロパティ] -> [C/C+] -> [全般]を選択

      - 右ペインの[追加のインクルードディレクトリ]に以下の文字列を入力
        - C:\devel\library\open_cv\3.4.1\build\include

    - 左ペインの[構成プロパティ] -> [リンカ] -> [全般]を選択 
      - 右ペイン(中央辺りの)[追加のライブラリディレクトリ]に以下の文字列を入力
        - (32bit版Windowsの場合) : C:\devel\library\open_cv\3.4.1\build\x86\vc15\lib
        - (64bit版Windowsの場合) : C:\devel\library\open_cv\3.4.1\build\x64\vc15\lib

    - 左ペインの[構成プロパティ] -> [リンカ] -> [入力]を選択

      - [追加の依存ファイル]に以下の文字列を入力する

        - (32bit版Windowsの場合) 

          - C:\devel\library\open_cv\3.4.1\build\x86\vc15\lib\opencv_world341.lib

            C:\devel\library\open_cv\3.4.1\build\x86\vc15\lib\opencv_world341d.lib

        - (64bit版Windowsの場合) 

          - C:\devel\library\open_cv\3.4.1\build\x64\vc15\lib\opencv_world341.lib
          - C:\devel\library\open_cv\3.4.1\build\x64\vc15\lib\opencv_world341d.lib

  ### OpenCVを使って画像表示

  ここまでの手順で、OpenCVをVisual Studioのプロジェクトへ組み込む事が出来ました。

  早速、OpenCVを使って画像表示してみましょう。

  ```cpp
  #include "pch.h" // 各ソース先頭は必ずこの行で始める事
  #include <cassert>
  
  using namespace cv;
  
  int main()
  {
      IplImage *img_ptr = cvLoadImage(
          "red-panda-731987_960_720.jpg",
          CV_LOAD_IMAGE_COLOR );
      if( img_ptr == nullptr )
      {
          printf( "画像ファイルが見つかりません\n" );
          assert( 0 );
      }
  
      const char* window_name = "pic_windows";
      cvNamedWindow( window_name );
      cvShowImage( window_name, img_ptr );
      cvWaitKey();
      cvDestroyAllWindows();
  
      if( img_ptr )
      {
          cvReleaseImage( &img_ptr );
      }
  }
  
  ```

  環境が正しく設定されていれば画像が表示されますが、設定不備がある場合エラーとなってしまいます。エラーメッセージを頼りに、上記設定手順の関連箇所を確認して環境を見直してみて下さい

  - OpenCV用ヘッダファイル(opencv.hpp)が見つからない
    - [追加のインクルードディレクトリ]は正しく設定されていますか？
  - (リンクエラー)定義されていない参照/未解決の外部シンボルxxxがある
    - ライブラリパス及びライブラリファイルが正しく追加されていますか？
  - 実行するとopencv_world3.4.1.dllが無いと言われる
    - [構成プロパティ] -> [デバッグ] -> [環境]の設定は合っていますか？
      - PATH環境変数にOpenCVアプリケーションを動作させるのに必要な動的ライブラリパスを追加するものなので、追加先に上記dllファイルがあるか確認して下さい

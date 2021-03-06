# トラブルシューティング

OpenCVでハマったポイントをまとめていきます。

困った事があった時のヒントになるような内容があれば幸いです

## 環境編



## コーディング編

### opencv1.x系のヘッダファイルのインクルード出来ない

古いサンプルコードなどを見ていると

```c
#include <cv.h> 
#include <cxcore.h>
#include <highgui.h>
```

などと書かれているので、真似して書いてみるとインクルード出来ないというエラーメッセージ。これは、OpenCVのバージョンアップの際に以下のようなディレクトリ構成になったのが原因のようです。(ここまでの手順で/includeまでがインクルードパスに付け加えられている前提)

- /include
  - /opencv
    - cv.h
    - cxcore.h
    - highgui.h
  - /opencv2

という訳で、上記のようなコードは以下のように書き換える必要があります

```c
#include <opencv/cv.h> 
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
```

### 画像が出力されない

画像ファイルを読み込んで、ダイアログに出力するだけのサンプルです

```cpp
int main()
{
    Mat img = imread("Windows.jpg");
    imshow("img_dialog",img);
    waitKey(0); // ★ここがポイント
}
```

上記サンプルコードの内、waitkey()について

「キー入力を待ち、入力されたキーコードを返す」

とだけ説明されている事が大変多いです。しかし、

「キー入力受付待ちは、別のやり方で待っているから不要だから消そう」

とすると**画像が出力されなくなる**という状態になります。

実は、これはOpenCVのGUI周りをやってくれるHighGUIの仕組みに関するもので[OpenCV 2.2 C++ リファレンス](http://opencv.jp/opencv-2svn/cpp/user_interface.html)で下記のように説明されています。(この文面からダイアログの描画との関連性に気付くのは難しい気がします。)

> **注意：** この関数は，HighGUI においてイベントを取得して扱うための唯一の手段です．よって，実行環境がイベント処理を扱ってくれる場合を除けば，通常のイベント処理ではこの関数を周期的に呼び出す必要があります。
>
> **注意 2:** この関数は，HighGUI ウィンドウが少なくとも1つ存在し，それがアクティブな場合にのみ動作します．複数の HighGUI ウィンドウがある場合は，どれかがアクティブであれば構いません．

imshow()ではWM_PAINTイベント発行するのですが、そのイベントを取得して処理する為の機構を走らせる標準的な手法とされています。とはいえ、自作のメッセージループを書く時代ではない気がするので、取り敢えずは**imshow()を呼んだら、セットでwaitkey()を呼んでおく**と覚えるのが素直かなと思います。

### waitKey()入れたのに再描画されない

再描画用の処理を回す目的でwaitKey()を呼ぶのは正解です。が、引数指定値はミリ秒なので

```cpp
int main()
{
    Mat img = imread("Windows.jpg");
    imshow("img_dialog",img);
    waitKey(1); // 1msecだけイベント処理を行う
}
```

としていると**イベントを処理する前にタイムアウトしてしまい再描画処理されない**ケースが頻発します

「1msecで再描画が保証されないなら、どの程度待てば保証されるの？」

という疑問も、OSの気分次第なのでなんとも。。。

取り敢えず１秒( waitKey(1000) )にしておけばそれなりに描画は走りはしますが、サンプルコードなどで見掛ける「waitKey(0)」で、ユーザーのキー入力を待つ形が一番無難な気はします


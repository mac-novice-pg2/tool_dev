#pragma once

const int BUFFER_SIZE = 256;

typedef enum{
    eMENU_START = 0,
    // ファイル操作
    eMENU_FILE_OPEN = eMENU_START,// ファイルオープン
    eMENU_FILE_CLOSE,             // ファイルクローズ
    eMENU_FILE_SAVE,              // ファイル保存

    // 色操作
    eMENU_COLOR_CHG_MONO,  // モノクロ変換
    eMENU_COLOR_CHG_SEPIA, // セピア変換

    // 回転
    eMENU_ROTATE_L,        // 反時計回り/CCW
    eMENU_ROTATE_R,        // 時計回り/CW

    // 変形
    eMENU_ZOOM_UP,         // 拡大
    eMENU_ZOOM_DOWN,       // 縮小
    eMENU_RESIZE,          // リサイズ
    eMENU_FLIP,            // 反転

    // 画像加工
    eMENU_TRIM,            // トリミング
    eMENU_BLUR,            // ぼかし
    eMENU_GAUSSIAN_BLUR,   // ガウシアンフィルタ

    // 特徴抽出
    eMENU_DETECT_EDGE,     // エッジ抽出
    eMENU_DETECT_OUTLINE,  // 輪郭抽出

    eMENU_ID_DEMO,         // OpenCVデモ表示
    eMENU_ID_END,
}eMenuID;

/**
    ファイル操作系メニュー
 */
typedef struct{ // eMENU_FILE_OPEN
    char filename[ BUFFER_SIZE ];
}eMENU_INPUT_RESULT_FILE_OPEN;

typedef struct{ // eMENU_FILE_CLOSE
    int reserved;
}eMENU_INPUT_RESULT_FILE_CLOSE;

typedef struct{ // eMENU_FILE_SAVE
    char filename[ BUFFER_SIZE ];
}eMENU_INPUT_RESULT_FILE_SAVE;

/**
    色変換
 */
typedef struct{ // eMENU_COLOR_CHG_MONO
    int reserved;
}eMENU_INPUT_RESULT_COLOR_CHG_MONO;

typedef struct{ // eMENU_COLOR_CHG_SEPIA
    int reserved;
}eMENU_INPUT_RESULT_COLOR_CHG_SEPIA;

/**
    回転
 */
typedef struct{ // eMENU_ROTATE_L
    int angle_L;
}eMENU_INPUT_RESULT_ROTATE_L;

typedef struct{ // eMENU_ROTATE_R
    int angle_R;
}eMENU_INPUT_RESULT_ROTATE_R;

/**
    変形
 */
typedef struct{ // eMENU_ZOOM_UP
    float zoom_up_ratio_width;
    float zoom_up_ratio_height;
}eMENU_INPUT_RESULT_ZOOM_UP;

typedef struct{ // eMENU_ZOOM_DOWN
    float zoom_down_ratio_width;
    float zoom_down_ratio_height;
}eMENU_INPUT_RESULT_ZOOM_DOWN;

typedef struct{ // eMENU_RESIZE
    int width;
    int height;
}eMENU_INPUT_RESULT_RESIZE;

/**
    トリミング
 */
typedef struct{ // eMENU_RESIZE
    int x_start;
    int x_end;
    int y_start;
    int y_end;
}eMENU_INPUT_RESULT_TRIM;

// テンプレート
typedef struct{
    int reserved;
}eMENU_INPUT_RESULT_;

// 外部I/F用パラメータ

typedef struct{
    cv::Mat mat;
    cv::Mat mat_aft;
    const char *img_name;
    const char *img_name2;
}ocv_param;

typedef struct{
    eMenuID  menu_id;
    void    *input_param;
    ocv_param ocv_param;
}eMenuParam;

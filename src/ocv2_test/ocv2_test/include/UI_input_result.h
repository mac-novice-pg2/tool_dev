#pragma once

const int BUFFER_SIZE = 256;

typedef enum
{
    eMENU_START = 0,
    // ファイル操作
    eMENU_FILE_OPEN = eMENU_START,// ファイルオープン
    eMENU_FILE_CLOSE,             // ファイルクローズ
    eMENU_FILE_SAVE,              // ファイル保存

    // 色操作
    eMENU_COLOR_CHG_MONO,  // モノクロ変換
    eMENU_COLOR_CHG_SEPIA, // セピア変換

    // 回転
    eMENU_ROTATE,          // 回転

    // 変形
    eMENU_ZOOM,            // 拡大/縮小
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
 // eMENU_FILE_OPEN
typedef struct
{
    char filename[ BUFFER_SIZE ];
}eMENU_INPUT_RESULT_FILE_OPEN;

// eMENU_FILE_CLOSE
typedef struct
{
    int reserved;
}eMENU_INPUT_RESULT_FILE_CLOSE;

// eMENU_FILE_SAVE
typedef struct
{
    char filename[ BUFFER_SIZE ];
}eMENU_INPUT_RESULT_FILE_SAVE;

/**
    色変換
 */
 // eMENU_COLOR_CHG_MONO
typedef struct
{
    int reserved;
}eMENU_INPUT_RESULT_COLOR_CHG_MONO;

// eMENU_COLOR_CHG_SEPIA
typedef struct
{ 
    int reserved;
}eMENU_INPUT_RESULT_COLOR_CHG_SEPIA;

/**
    回転
 */
 // eMENU_ROTATE
typedef struct
{
    int angle;
}eMENU_INPUT_RESULT_ROTATE;

/**
    変形
 */
// eMENU_ZOOM_UP
typedef struct
{
    float zoom_ratio_width;
    float zoom_ratio_height;
}eMENU_INPUT_RESULT_ZOOM;

// eMENU_RESIZE
typedef struct
{
    int width;
    int height;
}eMENU_INPUT_RESULT_RESIZE;

/**
    トリミング
 */
 // eMENU_RESIZE
typedef struct
{
    int x_start;
    int x_end;
    int y_start;
    int y_end;
}eMENU_INPUT_RESULT_TRIM;

// テンプレート
typedef struct
{
    int reserved;
}eMENU_INPUT_RESULT_;

// 外部I/F用パラメータ

typedef struct
{
    bool is_open;
    IplImage *img;
}img_handler_t;

typedef struct
{
    bool is_open;
    cv::Mat mat;
}mat_handler_t;

typedef struct
{
    mat_handler_t mat_handler;
    img_handler_t img_handler;
    const char *img_name;
    const char *img_name2;
}ocv_param;

typedef struct
{
    eMenuID  menu_id;
    void    *input_param;
    ocv_param ocv_param;
}eMenuParam;

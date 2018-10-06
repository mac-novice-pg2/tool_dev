#include "pch.h"

#include "OCV.h"
#include "UI_input_result.h"

#include <cassert>

using namespace cv;

/**
    ファイル操作
 */
static void
ocv_file_open( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_OPEN* )param->input_param;

    param->ocv_param.img = cvLoadImage( p->filename, CV_LOAD_IMAGE_COLOR );
    if( param->ocv_param.img == NULL )
    {
        printf( "%sの読み込みに失敗しました。ファイルの有無を確認して下さい\n", p->filename );
        return;
    }
    cvNamedWindow( param->ocv_param.img_name, CV_WINDOW_AUTOSIZE );
    cvShowImage( param->ocv_param.img_name, param->ocv_param.img );
    waitKey( 1 ); // 再描画を促す
}

static void
ocv_file_close( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_CLOSE* )param->input_param;

    printf( "%s() not implemented\n", __func__ );
}

static void
ocv_file_save( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_SAVE* )param->input_param;

    cvSaveImage( p->filename, param->ocv_param.img );
    printf( "%sとして保存しました\n", p->filename );
}

/**
    色変換
 */
static void
ocv_color_change_mono( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_MONO* )param->input_param;

    IplImage *img_gray = cvCreateImage( cvGetSize( param->ocv_param.img ), IPL_DEPTH_8U, 1 );
    IplImage *img_aft = cvCreateImage( cvGetSize( param->ocv_param.img ), IPL_DEPTH_8U, 3 );
    cvCvtColor( param->ocv_param.img, img_gray, CV_BGR2GRAY );
    cvMerge( img_gray, img_gray, img_gray, NULL, img_aft );
    cvReleaseImage( &img_gray );

    cvShowImage( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    cvReleaseImage( &param->ocv_param.img );
    param->ocv_param.img = img_aft;
}

static void
ocv_color_change_sepia( eMenuParam *param )
{
    const int sepia_hue = 22;
    const int sepia_sat = 90;

    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_SEPIA* )param->input_param;

    CvSize img_size = cvGetSize( param->ocv_param.img );

    //	画像を生成する
    IplImage *img_HSV = cvCreateImage( img_size, IPL_DEPTH_8U, 3 ); // HSV画像
    IplImage *img_hue = cvCreateImage( img_size, IPL_DEPTH_8U, 1 ); // 色相(H)
    IplImage *img_sat = cvCreateImage( img_size, IPL_DEPTH_8U, 1 ); // 彩度(S)
    IplImage *img_val = cvCreateImage( img_size, IPL_DEPTH_8U, 1 ); // 明度(V)

    IplImage *img_merge = cvCreateImage( img_size, IPL_DEPTH_8U, 3 ); // for merge
    IplImage *img_aft = cvCreateImage( img_size, IPL_DEPTH_8U, 3 );

    CvScalar cv_value_hue = cvScalar( sepia_hue );
    CvScalar cv_value_sat = cvScalar( sepia_sat );

    // 表色系変換( BGR -> HSV )
    cvCvtColor( param->ocv_param.img, img_HSV, CV_BGR2HSV );

    // HSV画像 -> [Hue][Sat][Val]分解
    cvSplit( img_HSV, img_hue, img_sat, img_val, NULL );
    cvReleaseImage( &img_HSV );

    cvSet( img_hue, cv_value_hue, NULL ); // Hueの値をセピア用の値へ
    cvSet( img_sat, cv_value_sat, NULL ); // Satの値をセピア用の値へ

    // [Hue][Sat][Val] => img_mergeとして統合
    cvMerge( img_hue, img_sat, img_val, NULL, img_merge );
    cvReleaseImage( &img_hue );
    cvReleaseImage( &img_sat );
    cvReleaseImage( &img_val );

    //	HSVからBGRに変換する
    cvCvtColor( img_merge, img_aft, CV_HSV2BGR );
    cvReleaseImage( &img_merge );
    cvShowImage( param->ocv_param.img_name, img_aft );
    cvWaitKey( 1 );
    param->ocv_param.img = img_aft;
}

/**
    回転
 */
static void
ocv_rotate_left( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_ROTATE_L* )param->input_param;

    printf( "%s() not implemented\n", __func__ );
}

static void
ocv_rotate_right( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_ROTATE_R* )param->input_param;

    printf( "%s() not implemented\n", __func__ );
}

/**
    変形
 */
static void
ocv_zoom_up( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_ZOOM_UP* )param->input_param;

    Mat img_aft;
    int calc_width = ( int )( param->ocv_param.mat.size().width * p->zoom_up_ratio_width );
    int calc_height = ( int )( param->ocv_param.mat.size().height * p->zoom_up_ratio_height );
    resize( param->ocv_param.mat, img_aft, Size( calc_width, calc_height ) );
    imshow( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    param->ocv_param.mat = img_aft;
}

static void
ocv_zoom_down( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_ZOOM_DOWN* )param->input_param;

    Mat img_aft;
    int calc_width = ( int )( param->ocv_param.mat.size().width * p->zoom_down_ratio_width );
    int calc_height = ( int )( param->ocv_param.mat.size().height * p->zoom_down_ratio_height );
    resize( param->ocv_param.mat, img_aft, Size( calc_width, calc_height ) );
    imshow( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    param->ocv_param.mat = img_aft;
}

static void
ocv_resize( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_RESIZE* )param->input_param;

    Mat img_aft;
    resize( param->ocv_param.mat, img_aft, Size( p->width, p->height ) );
    imshow( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    param->ocv_param.mat = img_aft;
}

static void
ocv_trim( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_TRIM* )param->input_param;

    Mat dst( param->ocv_param.mat, Rect( p->x_start, p->y_start, p->x_end, p->y_end ) );
    imshow( param->ocv_param.img_name, dst );
    waitKey( 1 ); // 再描画を促す
    param->ocv_param.mat_aft = dst;
}

static void
OCV_Demo( eMenuParam *param )
{
    param; // warning C4100対策の有名なテクニック( 引数は関数の本体部で参照されません )
    IplImage *img_ptr = cvLoadImage( "red-panda-731987_960_720.jpg", CV_LOAD_IMAGE_COLOR );
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
} // OCV_Demo()

static void
OCV_Unknown( eMenuParam *param )
{
    param; // warning C4100対策の有名なテクニック( 引数は関数の本体部で参照されません )
    assert( 0 );
}

/*
 OpenCV処理関数テーブル
*/
typedef void( *funt_t )( eMenuParam *param );
typedef struct{
    eMenuID menu_id;
    funt_t func;
}ocv_func_tbl_t;
ocv_func_tbl_t ocv_func_tbl[] = {
    { eMENU_FILE_OPEN, ocv_file_open },
    { eMENU_FILE_CLOSE, ocv_file_close },
    { eMENU_FILE_SAVE, ocv_file_save },
    { eMENU_COLOR_CHG_MONO, ocv_color_change_mono },
    { eMENU_COLOR_CHG_SEPIA, ocv_color_change_sepia },
    { eMENU_ROTATE_L, ocv_rotate_left },
    { eMENU_ROTATE_R, ocv_rotate_right },
    { eMENU_ZOOM_UP, ocv_zoom_up },
    { eMENU_ZOOM_DOWN, ocv_zoom_down },
    { eMENU_RESIZE, ocv_resize },
    { eMENU_TRIM, ocv_trim },
    { eMENU_ID_DEMO, OCV_Demo },
    { eMENU_ID_END, OCV_Unknown },
};

void OCV_main( eMenuParam *param )
{
    int idx = ( int )eMENU_START;
    // 呼び出すOpenCV処理関数を探す
    while( idx != eMENU_ID_END )
    {
        if( param->menu_id == ocv_func_tbl[ idx ].menu_id )
        {
            ocv_func_tbl[ idx ].func( param );
            break;
        }
        idx++;
    }
}

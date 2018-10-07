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

    IplImage *img = cvLoadImage( p->filename, CV_LOAD_IMAGE_COLOR );
    if( img == NULL )
    {
        printf( "%sの読み込みに失敗しました。ファイルの有無を確認して下さい\n", p->filename );
        return;
    }
    param->ocv_param.img_handler.img = img;
    param->ocv_param.img_handler.is_open = true;

    cvNamedWindow( param->ocv_param.img_name, CV_WINDOW_AUTOSIZE );
    cvShowImage( param->ocv_param.img_name, param->ocv_param.img_handler.img );
    waitKey( 1 ); // 再描画を促す
}

static void
ocv_file_close( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_CLOSE* )param->input_param;

    destroyAllWindows();
    if( param->ocv_param.img_handler.is_open )
    {
        cvReleaseImage( &( param->ocv_param.img_handler.img ) );
    }
}

static void
ocv_file_save( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_SAVE* )param->input_param;

    cvSaveImage( p->filename, param->ocv_param.img_handler.img );
    printf( "%sとして保存しました\n", p->filename );
}

/**
    色変換
 */
static void
ocv_color_change_mono( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_MONO* )param->input_param;
    IplImage *my_img = param->ocv_param.img_handler.img;

    IplImage *img_gray = cvCreateImage( cvGetSize( my_img ), IPL_DEPTH_8U, 1 );
    IplImage *img_aft = cvCreateImage( cvGetSize( my_img ), IPL_DEPTH_8U, 3 );
    cvCvtColor( param->ocv_param.img_handler.img, img_gray, CV_BGR2GRAY );
    cvMerge( img_gray, img_gray, img_gray, NULL, img_aft );
    cvReleaseImage( &img_gray );

    cvShowImage( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    cvReleaseImage( &my_img );
    my_img = img_aft;
}

static void
ocv_color_change_sepia( eMenuParam *param )
{
    const int sepia_hue = 22;
    const int sepia_sat = 90;

    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_SEPIA* )param->input_param;
    IplImage *my_img = param->ocv_param.img_handler.img;

    //	画像を生成する
    CvSize img_size = cvGetSize( my_img );
    IplImage *img_HSV = cvCreateImage( img_size, IPL_DEPTH_8U, 3 ); // HSV画像
    IplImage *img_hue = cvCreateImage( img_size, IPL_DEPTH_8U, 1 ); // 色相(H)
    IplImage *img_sat = cvCreateImage( img_size, IPL_DEPTH_8U, 1 ); // 彩度(S)
    IplImage *img_val = cvCreateImage( img_size, IPL_DEPTH_8U, 1 ); // 明度(V)

    IplImage *img_merge = cvCreateImage( img_size, IPL_DEPTH_8U, 3 ); // for merge
    IplImage *img_aft = cvCreateImage( img_size, IPL_DEPTH_8U, 3 );

    CvScalar cv_value_hue = cvScalar( sepia_hue );
    CvScalar cv_value_sat = cvScalar( sepia_sat );

    // 表色系変換( BGR -> HSV )
    cvCvtColor( my_img, img_HSV, CV_BGR2HSV );
    cvReleaseImage( &my_img );

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
    my_img = img_aft;
}

/**
    回転
 */
static void
ocv_rotate( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_ROTATE* )param->input_param;
    IplImage *my_img = param->ocv_param.img_handler.img;
    IplImage *img_aft = cvCreateImage( cvGetSize( my_img ), IPL_DEPTH_8U, 3 );

    // 回転中心算出
    CvPoint2D32f center = cvPoint2D32f( my_img->width / 2.0, my_img->height / 2.0 );	//回転中心

    // 回転行列を生成する
    CvMat *rotate_matrix = cvCreateMat( 2, 3, CV_32FC1 );

    //	変換行列を求める
    cv2DRotationMatrix( center, p->angle, 1, rotate_matrix );

    //	画像の拡大、縮小、回転を行う
    cvWarpAffine( my_img, img_aft, rotate_matrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll( 0 ) );
    cvShowImage( param->ocv_param.img_name, img_aft );
    cvWaitKey( 1 );
    cvReleaseImage( &my_img );
    my_img = img_aft;
}

/**
    変形
 */
static void
ocv_zoom( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_ZOOM* )param->input_param;
    IplImage *my_img = param->ocv_param.img_handler.img;

    // ズーム後のサイズに合わせたイメージを作成する
    IplImage *img_aft = cvCreateImage( cvSize(
        ( int )( my_img->width * p->zoom_ratio_width ),
        ( int )( my_img->height * p->zoom_ratio_height ) ),
        IPL_DEPTH_8U, 3 );
    cvResize( my_img, img_aft, CV_INTER_LINEAR );
    cvReleaseImage( &my_img );

    cvShowImage( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    my_img = img_aft;
}

static void
ocv_resize( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_RESIZE* )param->input_param;
    IplImage *my_img = param->ocv_param.img_handler.img;

    // ズーム後のサイズに合わせたイメージを作成する
    IplImage *img_aft = cvCreateImage( cvSize( p->width, p->height ), IPL_DEPTH_8U, 3 );
    cvResize( my_img, img_aft, CV_INTER_LINEAR );
    cvReleaseImage( &my_img );
    cvShowImage( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    my_img = img_aft;
}

static void
ocv_trim( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_TRIM* )param->input_param;

    printf( "not implemented\n" );
#if 0
    Mat dst( param->ocv_param.mat, Rect( p->x_start, p->y_start, p->x_end, p->y_end ) );
    imshow( param->ocv_param.img_name, dst );
    waitKey( 1 ); // 再描画を促す
    param->ocv_param.mat_aft = dst;
#endif
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
    { eMENU_ROTATE, ocv_rotate },
    { eMENU_ZOOM, ocv_zoom },
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

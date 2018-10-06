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

    param->ocv_param.mat = imread( p->filename, CV_LOAD_IMAGE_COLOR );
    if( param->ocv_param.mat.data == 0 )
    {
        printf( "%sの読み込みに失敗しました。ファイルの有無を確認して下さい\n", p->filename );
        return;
    }
    namedWindow( param->ocv_param.img_name, CV_WINDOW_AUTOSIZE );
    imshow( param->ocv_param.img_name, param->ocv_param.mat );
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

    imwrite( p->filename, param->ocv_param.mat );
}

/**
    色変換
 */
static void
ocv_color_change_mono( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_MONO* )param->input_param;

    Mat img_aft;
    cvtColor( param->ocv_param.mat, img_aft, COLOR_RGB2GRAY );
    imshow( param->ocv_param.img_name, img_aft );
    waitKey( 1 ); // 再描画を促す
    param->ocv_param.mat = img_aft;
}

static void
ocv_color_change_sepia( eMenuParam *param )
{
    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_SEPIA* )param->input_param;

    printf( "%s() not implemented\n", __func__ );
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

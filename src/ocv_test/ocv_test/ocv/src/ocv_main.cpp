#include "pch.h"

#include "OCV.h"
#include "UI_input_result.h"

#include <cassert>

using namespace cv;

/**
    ファイル操作
 */
static void
ocv_file_open( eMenuParam *param,
               Mat        *img,
               const char *img_name )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_OPEN* )param->input_result;

    *img = imread( p->filename );
    imshow( img_name, *img );
}

static void
ocv_file_close( eMenuParam *param,
                Mat        *img )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_CLOSE* )param->input_result;

    printf( "%s() not implemented\n", __func__ );
}

static void
ocv_file_save( eMenuParam *param,
               Mat        *img )
{
    auto p = ( eMENU_INPUT_RESULT_FILE_SAVE* )param->input_result;

    imwrite( p->filename, *img );
}

/**
    色変換
 */
static void
ocv_color_change_mono( eMenuParam *param,
                       Mat        *img,
                       const char *img_name )
{
    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_MONO* )param->input_result;

    Mat img_aft;
    cvtColor( *img, img_aft, COLOR_RGB2GRAY );
    imshow( img_name, img_aft );
    *img = img_aft;
}

static void
ocv_color_change_sepia( eMenuParam *param,
                        Mat        *img )
{
    auto p = ( eMENU_INPUT_RESULT_COLOR_CHG_SEPIA* )param->input_result;

    printf( "%s() not implemented\n", __func__ );
}

/**
    回転
 */
static void
ocv_rotate_left( eMenuParam *param,
                 Mat        *img )
{
    auto p = ( eMENU_INPUT_RESULT_ROTATE_L* )param->input_result;

    printf( "%s() not implemented\n", __func__ );
}

static void
ocv_rotate_right( eMenuParam *param,
                  Mat        *img )
{
    auto p = ( eMENU_INPUT_RESULT_ROTATE_R* )param->input_result;

    printf( "%s() not implemented\n", __func__ );
}

/**
    変形
 */
static void
ocv_zoom_up( eMenuParam *param,
             Mat        *img,
             const char *img_name )
{
    auto p = ( eMENU_INPUT_RESULT_ZOOM_UP* )param->input_result;

    Mat img_aft;
    int calc_width = ( int )( img->size().width * p->zoom_up_ratio_width );
    int calc_height = ( int )( img->size().height * p->zoom_up_ratio_height );
    resize( *img, img_aft, Size( calc_width, calc_height ) );
    imshow( img_name, img_aft );
    *img = img_aft;
}

static void
ocv_zoom_down( eMenuParam *param,
               Mat        *img,
               const char *img_name )
{
    auto p = ( eMENU_INPUT_RESULT_ZOOM_DOWN* )param->input_result;

    Mat img_aft;
    int calc_width = ( int )( img->size().width * p->zoom_down_ratio_width );
    int calc_height = ( int )( img->size().height * p->zoom_down_ratio_height );
    resize( *img, img_aft, Size( calc_width, calc_height ) );
    imshow( img_name, img_aft );
    *img = img_aft;
}

static void
ocv_resize( eMenuParam *param,
            Mat        *img,
            const char *img_name )
{
    auto p = ( eMENU_INPUT_RESULT_RESIZE* )param->input_result;

    Mat img_aft;
    resize( *img, img_aft, Size( p->width, p->height ) );
    imshow( img_name, img_aft );
    *img = img_aft;
}

static void
ocv_trim( eMenuParam *param,
          Mat        *img,
          Mat        *img_trim,
          const char *img_name )
{
    auto p = ( eMENU_INPUT_RESULT_TRIM* )param->input_result;

    Mat dst( *img, Rect( p->x_start, p->y_start, p->x_end, p->y_end ) );
    imshow( img_name, dst );
    *img_trim = dst;
}

static void
OCV_Demo( void )
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

void OCV_main( eMenuParam *param )
{
    Mat mat;
    Mat mat_aft;
    const char *img_name = "test_img";
    const char *img_name2 = "test_img2";
    switch( param->menu_id )
    {
    case eMENU_FILE_OPEN:
        ocv_file_open( param, &mat, img_name );
        break;
    case eMENU_FILE_CLOSE:
        ocv_file_close( param, &mat );
        break;

    case eMENU_FILE_SAVE:
        ocv_file_save( param, &mat );
        break;
        
    case eMENU_COLOR_CHG_MONO:
        ocv_color_change_mono( param, &mat, img_name );
        break;

    case eMENU_COLOR_CHG_SEPIA:
        ocv_color_change_sepia( param, &mat );
        break;

    case eMENU_ROTATE_L:
        ocv_rotate_left( param, &mat );
        break;

    case eMENU_ROTATE_R:
        ocv_rotate_right( param, &mat );
        break;

    case eMENU_ZOOM_UP:
        ocv_zoom_up( param, &mat, img_name );
        break;

    case eMENU_ZOOM_DOWN:
        ocv_zoom_down( param, &mat, img_name );
        break;

    case eMENU_RESIZE:
        ocv_resize( param, &mat, img_name );
        break;

    case eMENU_TRIM:
        ocv_trim( param, &mat, &mat_aft, img_name2 );
        break;

    case eMENU_ID_DEMO:
        OCV_Demo();
        break;

    default:
        printf( "定義されていない機能です\n" );
        assert( 0 );
    }
}

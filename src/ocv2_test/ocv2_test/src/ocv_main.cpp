#include "pch.h"

#include "UI_input_result.h"

#include <vector>
#include <cassert>

using namespace cv;

/**
    ファイル操作
 */
void
OCV_file_open( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_OPEN *menu_input )
{
    auto img = imread( menu_input->filename, CV_LOAD_IMAGE_COLOR );
    if( img.empty() )
    {
        printf( "%sの読み込みに失敗しました。ファイルの有無を確認して下さい\n", menu_input->filename );
        return;
    }
    ocv_param->mat_handler.mat = img;
    ocv_param->mat_handler.is_open = true;

    imshow( ocv_param->img_name, ocv_param->mat_handler.mat );
    waitKey( 1 ); // 再描画を促す
} // OCV_file_open()

void
OCV_file_close( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_CLOSE *menu_input )
{
} // OCV_file_close()

void
OCV_file_save( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_SAVE *menu_input )
{
    auto save_img = ocv_param->mat_handler.mat;

    imwrite( menu_input->filename, save_img );
    printf( "%sとして保存しました\n", menu_input->filename );
} // OCV_file_save()

/**
    色変換
 */
void
OCV_color_change_mono( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_COLOR_CHG_MONO *menu_input )
{
    auto my_img = ocv_param->mat_handler.mat;

    Mat img_aft;
    cvtColor( my_img, img_aft, COLOR_RGB2GRAY );

    imshow( ocv_param->img_name, img_aft );
    waitKey( 1 ); // 再描画を促す

    my_img = img_aft.clone();
} // OCV_color_change_mono()

void
OCV_color_change_sepia( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_COLOR_CHG_SEPIA *menu_input )
{
    auto my_img = ocv_param->mat_handler.mat;

    // 表色系変換( BGR -> HSV )
    // HSV画像 -> [Hue][Sat][Val]分解
    Size img_size = my_img.size();
    Mat img_HSV( img_size, CV_8UC3 );
    std::vector< Mat > vec_hsv( 3 );


    cvtColor( my_img, img_HSV, CV_BGR2HSV );
    split( img_HSV, vec_hsv );
    // セピア色に着色
    const int sepia_hue = 22;
    const int sepia_sat = 90;
    Scalar hue( sepia_hue );
    Scalar sat( sepia_sat );
    vec_hsv[ 0 ] = hue; // Hueの値をセピア用の値へ
    vec_hsv[ 1 ] = sat; // Satの値をセピア用の値へ

    // [Hue][Sat][Val] => img_mergeとして統合
    Mat img_merge( img_size, CV_8UC3 );
    merge( vec_hsv, img_merge );

    //	HSVからBGRに変換する
    Mat img_aft( img_size, CV_8UC3 );
    cvtColor( img_merge, img_aft, CV_HSV2BGR );

    imshow( ocv_param->img_name, img_aft );
    cvWaitKey( 1 );

    my_img = img_aft.clone();
} // OCV_color_change_sepia()

/**
    回転
 */
void
OCV_rotate( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ROTATE *menu_input )
{
    auto my_img = ocv_param->mat_handler.mat;

    Point2f center( ( float )( my_img.cols * 0.5 ), ( float )( my_img.rows * 0.5 ) );
    auto rotate_matrix = getRotationMatrix2D( center, menu_input->angle, 1 );
    Mat img_aft;
    warpAffine( my_img, img_aft, rotate_matrix, my_img.size() );

    imshow( ocv_param->img_name, img_aft );
    cvWaitKey( 1 );

    my_img = img_aft.clone();
} // OCV_rotate()

/**
    変形
 */
void
OCV_zoom( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ZOOM *menu_input )
{
    auto my_img = ocv_param->mat_handler.mat;

    Mat img_aft;
    resize( my_img, img_aft, Size(), menu_input->zoom_ratio_width, menu_input->zoom_ratio_height );

    imshow( ocv_param->img_name, img_aft );
    waitKey( 1 ); // 再描画を促す

    my_img = img_aft.clone();
} // OCV_zoom()

void
OCV_resize( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_RESIZE *menu_input )
{
    auto my_img = ocv_param->mat_handler.mat;

    Mat img_aft( menu_input->width, menu_input->height, my_img.type() );
    resize( my_img, img_aft, img_aft.size(), INTER_CUBIC );

    imshow( ocv_param->img_name, img_aft );
    waitKey( 1 ); // 再描画を促す

    my_img = img_aft.clone();
} // OCV_resize()

void
OCV_trim( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_TRIM *menu_input )
{
    auto my_img = ocv_param->mat_handler.mat;

    Mat img_aft( my_img, Rect( menu_input->x_start, menu_input->y_start, menu_input->x_end, menu_input->y_end ) );
    imshow( ocv_param->img_name, img_aft );
    waitKey( 1 ); // 再描画を促す

    my_img = img_aft.clone();
} // OCV_trim()

void
OCV_Demo( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ *menu_input )
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

void
OCV_Unknown( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ *menu_input )
{
    assert( 0 );
} // OCV_Unknown()

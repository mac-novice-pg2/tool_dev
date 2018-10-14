#include "pch.h"

#include "UI_input_result.h"

using namespace cv;

const int wait_time_def = 1000;

/**
    ファイル操作
 */
void
OCV_file_open( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_OPEN *menu_input )
{
    IplImage *my_img = cvLoadImage( menu_input->filename, CV_LOAD_IMAGE_COLOR );
    if( my_img == NULL )
    {
        printf( "%sの読み込みに失敗しました。ファイルの有無を確認して下さい\n", menu_input->filename );
        return;
    }

    // 画面描画
    cvNamedWindow( ocv_param->img_name, CV_WINDOW_AUTOSIZE );
    cvShowImage( ocv_param->img_name, my_img );
    waitKey( wait_time_def ); // 再描画を促す

    ocv_param->img_handler.is_open = true;
    ocv_param->img_handler.img = my_img;
} // OCV_file_open()

void
OCV_file_close( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_CLOSE *menu_input )
{
    destroyAllWindows();
    if( ocv_param->img_handler.is_open )
    {
        cvReleaseImage( &( ocv_param->img_handler.img ) );
    }
} // OCV_file_close()

void
OCV_file_save( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_SAVE *menu_input )
{
    cvSaveImage( menu_input->filename, ocv_param->img_handler.img );
    printf( "%sとして保存しました\n", menu_input->filename );
} // OCV_file_save()

/**
    色変換
 */
void
OCV_color_change_mono( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_COLOR_CHG_MONO *menu_input )
{
    IplImage *my_img = ocv_param->img_handler.img;

    IplImage *img_gray = cvCreateImage( cvGetSize( my_img ), IPL_DEPTH_8U, 1 );
    IplImage *img_aft = cvCreateImage( cvGetSize( my_img ), IPL_DEPTH_8U, 3 );
    cvCvtColor( my_img, img_gray, CV_BGR2GRAY );
    cvMerge( img_gray, img_gray, img_gray, NULL, img_aft );
    cvReleaseImage( &img_gray );

    cvShowImage( ocv_param->img_name, img_aft );
    waitKey( wait_time_def ); // 再描画を促す
    cvReleaseImage( &my_img );
    ocv_param->img_handler.img = img_aft;
} // OCV_color_change_mono()

void
OCV_color_change_sepia( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_COLOR_CHG_SEPIA *menu_input )
{
    const int sepia_hue = 22;
    const int sepia_sat = 90;

    IplImage *my_img = ocv_param->img_handler.img;

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
    cvShowImage( ocv_param->img_name, img_aft );
    cvWaitKey( wait_time_def );
    cvReleaseImage( &my_img );
    ocv_param->img_handler.img = img_aft;
} // OCV_color_change_sepia()

/**
    回転
 */
void
OCV_rotate( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ROTATE *menu_input )
{
    IplImage *my_img = ocv_param->img_handler.img;
    IplImage *img_aft = cvCreateImage( cvGetSize( my_img ), IPL_DEPTH_8U, 3 );

    // 回転中心算出
    CvPoint2D32f center = cvPoint2D32f( my_img->width / 2.0, my_img->height / 2.0 );	//回転中心

    // 回転行列を生成する
    CvMat *rotate_matrix = cvCreateMat( 2, 3, CV_32FC1 );

    //	変換行列を求める
    cv2DRotationMatrix( center, menu_input->angle, 1, rotate_matrix );

    //	画像の拡大、縮小、回転を行う
    cvWarpAffine( my_img, img_aft, rotate_matrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll( 0 ) );
    cvShowImage( ocv_param->img_name, img_aft );
    cvWaitKey( wait_time_def );
    cvReleaseImage( &my_img );
    ocv_param->img_handler.img = img_aft;
} // OCV_rotate()

/**
    変形
 */
void
OCV_zoom( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ZOOM *menu_input )
{
    IplImage *my_img = ocv_param->img_handler.img;

    // ズーム後のサイズに合わせたイメージを作成する
    IplImage *img_aft = cvCreateImage( cvSize(
        ( int )( my_img->width * menu_input->zoom_ratio_width ),
        ( int )( my_img->height * menu_input->zoom_ratio_height ) ),
        IPL_DEPTH_8U, 3 );
    cvResize( my_img, img_aft, CV_INTER_LINEAR );

    cvShowImage( ocv_param->img_name, img_aft );
    waitKey( wait_time_def ); // 再描画を促す

    cvReleaseImage( &my_img );
    ocv_param->img_handler.img = img_aft;
} // OCV_zoom()

void
OCV_resize( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_RESIZE *menu_input )
{
    IplImage *my_img = ocv_param->img_handler.img;

    // ズーム後のサイズに合わせたイメージを作成する
    IplImage *img_aft = cvCreateImage( cvSize( menu_input->width, menu_input->height ), IPL_DEPTH_8U, 3 );
    cvResize( my_img, img_aft, CV_INTER_LINEAR );

    cvShowImage( ocv_param->img_name, img_aft );
    waitKey( wait_time_def ); // 再描画を促す

    cvReleaseImage( &my_img );
    ocv_param->img_handler.img = img_aft;
} // OCV_resize()

void
OCV_trim( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_TRIM *menu_input )
{
    IplImage *my_img = ocv_param->img_handler.img;

    // トリミング用領域確保
    int calc_width = menu_input->x_end - menu_input->x_start;
    int calc_height = menu_input->y_end - menu_input->y_start;
    IplImage *img_aft = cvCreateImage( cvSize( calc_width, calc_height ), IPL_DEPTH_8U, 3 );

    // トリミング領域の中心点算出
    CvPoint2D32f center;
    center.x = ( menu_input->x_start + menu_input->x_end ) / 2;
    center.y = ( menu_input->y_start + menu_input->y_end ) / 2;
    cvGetRectSubPix( my_img, img_aft, center );

    cvShowImage( ocv_param->img_name, img_aft );
    waitKey( wait_time_def ); // 再描画を促す

    cvReleaseImage( &my_img );
    ocv_param->img_handler.img = img_aft;
} // OCV_trim()

void
OCV_Demo( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ *menu_input )
{
    IplImage *img_ptr = cvLoadImage( "red-panda-731987_960_720.jpg", CV_LOAD_IMAGE_COLOR );
    if( img_ptr == nullptr )
    {
        printf( "画像ファイルが見つかりません\n" );
        CV_Assert( 0 );
    }

    const char* window_name = "pic_windows";
    cvNamedWindow( window_name );
    cvShowImage( window_name, img_ptr );
    cvWaitKey( wait_time_def );
    cvDestroyAllWindows();

    if( img_ptr )
    {
        cvReleaseImage( &img_ptr );
    }
} // OCV_Demo()

void
OCV_Unknown( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ *menu_input )
{
    CV_Assert( 0 );
} // OCV_Unknown()

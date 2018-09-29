#include "pch.h"
#include "ocv.h"

#include <cassert>

using namespace cv;

void OCV_Demo( void )
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
}
void ocv_main( void )
{
}

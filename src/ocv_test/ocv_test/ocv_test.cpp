#include "pch.h" // 各ソース先頭は必ずこの行で始める事

#include "UI_input_result.h"

bool UI_menu_main( OCV_Param_T *ocv_param );

int main()
{
    bool apl_end = false;
    OCV_Param_T ocv_param;

    ocv_param.img_name = "test_img";
    ocv_param.img_name2 = "test_img2";
    do
    {
        apl_end = UI_menu_main( &ocv_param );
    } while( apl_end == false );
    printf( "アプリケーションを終了します\n" );
}

#include "pch.h" // 各ソース先頭は必ずこの行で始める事

#include "UI_input_result.h"

void UI_menu_main( eMenuParam *menu_param );
void OCV_main( eMenuParam *menu_param );

int main()
{
    eMenuParam param;

    for( ;; )
    {
        param.menu_id = eMENU_ID_END;
        UI_menu_main( &param );
        if( param.menu_id == eMENU_ID_END )
        {
            printf( "アプリケーションを終了します\n" );
            break;
        }
        else
        {
            OCV_main( &param );
        }
    }
}

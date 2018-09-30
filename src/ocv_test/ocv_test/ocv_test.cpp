#include "pch.h" // 各ソース先頭は必ずこの行で始める事

#include "ui_input_result.h"

void OCV_Demo( void );
void UI_menu_main( eMenuParam *menu_param );

int main()
{
    eMenuParam param;
    UI_menu_main( &param );
    OCV_Demo();
}

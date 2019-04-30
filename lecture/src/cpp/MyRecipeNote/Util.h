#pragma once

#include "SearchType.h"

class Util{
    Util(); // ���[�e�B���e�B�N���X�Ȃ̂Ŏ��̒�`��}�~����

public:
    static char *Trim_C_String( char *c_str );

    static const char *ConvEnum2String( eFoodGenre genre );
    static const char *ConvEnum2String( eSeason season );
    static const char *ConvEnum2String( eDifficulty genre );
    static const char *ConvEnum2String( eFoodStyle type );

    static void WaitEnterKey( const char *message );
};


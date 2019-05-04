#pragma once

#include "recipe.h"

struct KeyCode{
    int key1st;
    int key2nd;
};

class Menu
{
protected:
    Recipe *recipe_;

    int input_num( const char *message );
    void input_string( const char *message, char* output, int len );

public:
	Menu();
    virtual ~Menu();

    virtual void Show( Recipe *recipe ) = 0;

    KeyCode InputKey();
    void Clear();
};

class SearchMenu : public Menu {
    void cursor_key_proc( int key );
    void show_search_result( SearchCondition *cond );
    void menu_search();

    eFoodGenre input_genre( const char *message );
    eSeason input_season( const char *message );
    eDifficulty input_difficulty( const char *message );
    eFoodStyle input_type( const char *message );

public:
    virtual void Show( Recipe *recipe ) override;
};

class TotalCostMenu : public Menu {

};
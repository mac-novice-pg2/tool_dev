#include <cstdio>

#include "Menu.h"
#include "recipe.h"
int main()
{
    Menu *menu = new SearchMenu();
    Recipe recipe( "recipe.txt" );

    menu->Show( &recipe );

    delete menu;
}

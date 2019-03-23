#pragma once

#include "calendar_def.h"

void Menu_ClearScreen( void );
void Menu_ShowInstruction( void );
bool Menu_ChangeCalendar( DateInfo *date );
bool Menu_Main( DateInfo *date );

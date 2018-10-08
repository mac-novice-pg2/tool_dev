#pragma once

void OCV_file_open( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_OPEN *menu_input );
void OCV_file_close( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_CLOSE *menu_input );
void OCV_file_save( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_FILE_SAVE *menu_input );
void OCV_color_change_mono( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_COLOR_CHG_MONO *menu_input );
void OCV_color_change_sepia( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_COLOR_CHG_SEPIA *menu_input );
void OCV_rotate( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ROTATE *menu_input );
void OCV_zoom( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ZOOM *menu_input );
void OCV_resize( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_RESIZE *menu_input );
void OCV_trim( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_TRIM *menu_input );

void OCV_Demo( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ *menu_input );
void OCV_Unknown( OCV_Param_T *ocv_param, eMENU_INPUT_RESULT_ *menu_input );
#include "pch.h"

#include "UI_input_result.h"

#include <vector>
#include <cassert>

using namespace cv;

/**
    ファイル操作
 */
static void
ocv_file_open(eMenuParam *param)
{
    auto p = (eMENU_INPUT_RESULT_FILE_OPEN*)param->input_param;

    auto img = imread(p->filename, CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        printf("%sの読み込みに失敗しました。ファイルの有無を確認して下さい\n", p->filename);
        return;
    }
    param->ocv_param.mat_handler.mat = img;
    param->ocv_param.mat_handler.is_open = true;

    imshow(param->ocv_param.img_name, param->ocv_param.mat_handler.mat);
    waitKey(1); // 再描画を促す
}

static void
ocv_file_save(eMenuParam *param)
{
    auto p = (eMENU_INPUT_RESULT_FILE_SAVE*)param->input_param;
    auto save_img = param->ocv_param.mat_handler.mat;

    imwrite(p->filename, save_img);
    printf("%sとして保存しました\n", p->filename);
}

/**
    色変換
 */
static void
ocv_color_change_mono(eMenuParam *param)
{
    auto p = (eMENU_INPUT_RESULT_COLOR_CHG_MONO*)param->input_param;
    auto my_img = param->ocv_param.mat_handler.mat;
    Mat dst;

    cvtColor(my_img, dst, COLOR_RGB2GRAY);
    imshow(param->ocv_param.img_name, dst);
    waitKey(1); // 再描画を促す
    my_img = dst;
}

static void
ocv_color_change_sepia(eMenuParam *param)
{
    const int sepia_hue = 22;
    const int sepia_sat = 90;

    auto p = (eMENU_INPUT_RESULT_COLOR_CHG_SEPIA*)param->input_param;
    auto my_img = param->ocv_param.mat_handler.mat;

    //	画像を生成する
    Size img_size = my_img.size();
    Mat img_HSV(img_size, CV_8UC3);

    // 表色系変換( BGR -> HSV )
    // HSV画像 -> [Hue][Sat][Val]分解
    std::vector< Mat > vec_hsv(3);
    split(img_HSV, vec_hsv);

    Scalar hue(sepia_hue);
    Scalar sat(sepia_sat);
    vec_hsv[0] = hue; // Hueの値をセピア用の値へ
    vec_hsv[1] = sat; // Satの値をセピア用の値へ

    // [Hue][Sat][Val] => img_mergeとして統合
    Mat img_merge(img_size, CV_8UC3);
    merge(vec_hsv, img_merge);

    //	HSVからBGRに変換する
    Mat img_aft(img_size, CV_8UC3);
    cvtColor(img_merge, img_aft, CV_HSV2BGR);
    imshow(param->ocv_param.img_name, img_aft);
    cvWaitKey(1);
    my_img = img_merge;
}

/**
    回転
 */
static void
ocv_rotate(eMenuParam *param)
{
    auto p = (eMENU_INPUT_RESULT_ROTATE*)param->input_param;
    auto my_img = param->ocv_param.mat_handler.mat;
    Point2f center(my_img.cols * 0.5, my_img.rows * 0.5 );

    auto rotate_matrix = getRotationMatrix2D(center, p->angle, 1);

    Mat img_aft;
    warpAffine(my_img, img_aft, rotate_matrix, my_img.size());
    imshow(param->ocv_param.img_name, img_aft);
    cvWaitKey(1);
    my_img = img_aft;
}

/**
    変形
 */
static void
ocv_zoom(eMenuParam *param)
{
    auto p = (eMENU_INPUT_RESULT_ZOOM*)param->input_param;
    auto my_img = param->ocv_param.mat_handler.mat;
    Mat img_aft;

    resize(my_img, img_aft, Size(), p->zoom_ratio_width, p->zoom_ratio_height);
    imshow(param->ocv_param.img_name, img_aft);
    waitKey(1); // 再描画を促す
    my_img = img_aft;
}

static void
ocv_resize(eMenuParam *param)
{
    auto p = (eMENU_INPUT_RESULT_RESIZE*)param->input_param;
    auto my_img = param->ocv_param.mat_handler.mat;
    Mat img_aft(p->width, p->height, my_img.type());

    resize(my_img, img_aft, img_aft.size(), INTER_CUBIC);
    imshow(param->ocv_param.img_name, img_aft);
    waitKey(1); // 再描画を促す
    my_img = img_aft;
}

static void
ocv_trim(eMenuParam *param)
{
    auto p = (eMENU_INPUT_RESULT_TRIM*)param->input_param;

    printf("not implemented\n");
#if 0
    Mat dst(param->ocv_param.mat, Rect(p->x_start, p->y_start, p->x_end, p->y_end));
    imshow(param->ocv_param.img_name, dst);
    waitKey(1); // 再描画を促す
    param->ocv_param.mat_aft = dst;
#endif
}

static void
OCV_Demo(eMenuParam *param)
{
    param; // warning C4100対策の有名なテクニック( 引数は関数の本体部で参照されません )
    IplImage *img_ptr = cvLoadImage("red-panda-731987_960_720.jpg", CV_LOAD_IMAGE_COLOR);
    if (img_ptr == nullptr)
    {
        printf("画像ファイルが見つかりません\n");
        assert(0);
    }

    const char* window_name = "pic_windows";
    cvNamedWindow(window_name);
    cvShowImage(window_name, img_ptr);
    cvWaitKey();
    cvDestroyAllWindows();

    if (img_ptr)
    {
        cvReleaseImage(&img_ptr);
    }
} // OCV_Demo()

static void
OCV_Unknown(eMenuParam *param)
{
    param; // warning C4100対策の有名なテクニック( 引数は関数の本体部で参照されません )
    assert(0);
}

/*
 OpenCV処理関数テーブル
*/
typedef void(*funt_t)(eMenuParam *param);
typedef struct {
    eMenuID menu_id;
    funt_t func;
}ocv_func_tbl_t;
ocv_func_tbl_t ocv_func_tbl[] = {
    { eMENU_FILE_OPEN, ocv_file_open },
    { eMENU_FILE_SAVE, ocv_file_save },
    { eMENU_COLOR_CHG_MONO, ocv_color_change_mono },
    { eMENU_COLOR_CHG_SEPIA, ocv_color_change_sepia },
    { eMENU_ROTATE, ocv_rotate },
    { eMENU_ZOOM, ocv_zoom },
    { eMENU_RESIZE, ocv_resize },
    { eMENU_TRIM, ocv_trim },
    { eMENU_ID_DEMO, OCV_Demo },
    { eMENU_ID_END, OCV_Unknown },
};

void OCV_main(eMenuParam *param)
{
    int idx = (int)eMENU_START;
    // 呼び出すOpenCV処理関数を探す
    while (idx != eMENU_ID_END)
    {
        if (param->menu_id == ocv_func_tbl[idx].menu_id)
        {
            ocv_func_tbl[idx].func(param);
            break;
        }
        idx++;
    }
}

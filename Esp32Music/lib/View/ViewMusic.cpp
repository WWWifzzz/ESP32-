/*******************************************************************************
* Description        : 显示界面
*******************************************************************************/
#define _VIEWMUSIC_C_

#include "ViewMusic.h"
#include "lvgl.h"
#include "Audio2.h"
#include <Adafruit_NeoPixel.h>
#include "ui_helpers.h"

extern Adafruit_NeoPixel strip;

LV_FONT_DECLARE(my_font_24)
LV_IMG_DECLARE(lv_img_BaBaLa)

static lv_obj_t * tabview;

static lv_obj_t * img;
static lv_obj_t * spinner;
static lv_obj_t * sliderRed;
static lv_obj_t * sliderGreen;
static lv_obj_t * sliderBlue;
static lv_obj_t * ui_Image1;

extern bool l_LastMusic;
extern bool l_NextMusic;
extern bool l_MusicSwitch;
extern bool l_MusicPlayLoop;
extern Audio2 audio2;

static lv_obj_t * play_obj;
static lv_obj_t * prev_obj;
static lv_obj_t * next_obj;
//static lv_obj_t * loop_obj;
static lv_obj_t * volum_obj;


/****************************************************************
// Summary: 滑杆回调函数
****************************************************************/
static void slider_event_cb(lv_event_t * e)
{
    uint32_t color;
    int ValueRed = lv_slider_get_value(sliderRed);
    int ValueGreen = lv_slider_get_value(sliderGreen);
    int ValueBlue = lv_slider_get_value(sliderBlue);

    color = strip.Color(ValueRed, ValueGreen, ValueBlue);
    strip.setPixelColor(0, color);
    strip.show(); 
}

/****************************************************************
// Summary: 滑杆
****************************************************************/
void Light_CreateEX(lv_obj_t * parent)
{
    sliderRed = lv_slider_create(parent);
    lv_slider_set_range(sliderRed, 0,255);
    lv_obj_set_width( sliderRed, 200);
    lv_obj_set_height( sliderRed, 20);
    lv_obj_align_to(sliderRed, parent, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_set_style_bg_color(sliderRed, lv_color_hex(0xFF0053), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderRed, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sliderRed, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderRed, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sliderRed, lv_color_hex(0xFF0000), LV_PART_KNOB | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderRed, 255, LV_PART_KNOB| LV_STATE_DEFAULT);

    sliderGreen = lv_slider_create(parent);
    lv_slider_set_range(sliderGreen, 0,255);
    lv_obj_set_width( sliderGreen, 200);
    lv_obj_set_height( sliderGreen, 20);
    lv_obj_align_to(sliderGreen, sliderRed, LV_ALIGN_BOTTOM_MID, 0, 50); 

    lv_obj_set_style_bg_color(sliderGreen, lv_color_hex(0x00FFA6), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderGreen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sliderGreen, lv_color_hex(0x00FF00), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderGreen, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sliderGreen, lv_color_hex(0x00FF00), LV_PART_KNOB | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderGreen, 255, LV_PART_KNOB| LV_STATE_DEFAULT);

    sliderBlue = lv_slider_create(parent);
    lv_slider_set_range(sliderBlue, 0,255);
    lv_obj_set_width( sliderBlue, 200);
    lv_obj_set_height( sliderBlue, 20);
    lv_obj_align_to(sliderBlue, sliderGreen, LV_ALIGN_BOTTOM_MID, 0, 50);

    lv_obj_set_style_bg_color(sliderBlue, lv_color_hex(0x0073FF), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderBlue, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sliderBlue, lv_color_hex(0x0000FF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderBlue, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(sliderBlue, lv_color_hex(0x0000FF), LV_PART_KNOB | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(sliderBlue, 255, LV_PART_KNOB| LV_STATE_DEFAULT);

    lv_obj_add_event_cb(sliderBlue, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(sliderRed, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(sliderGreen, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

/****************************************************************
// Summary: 	创建旋转环
****************************************************************/
static void Spinner_Creat(lv_obj_t * parent)
{
	spinner = lv_spinner_create(parent, 8000, 60);
	lv_obj_set_size(spinner, 100, 100);
	lv_obj_center(spinner);
}

/****************************************************************
// Summary: 	动画
****************************************************************/
void Image1_Animation( lv_obj_t *TargetObject, int delay)
{
    ui_anim_user_data_t *PropertyAnimation_0_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 20000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle );
    lv_anim_set_values(&PropertyAnimation_0, 0, 3600 );
    lv_anim_set_path_cb( &PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay( &PropertyAnimation_0, delay + 0 );
    lv_anim_set_deleted_cb( &PropertyAnimation_0, _ui_anim_callback_free_user_data );
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 100);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply( &PropertyAnimation_0, false );
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_angle );
    lv_anim_start(&PropertyAnimation_0);
}

/****************************************************************
// Summary: 	音乐回调函数
****************************************************************/
static void music_event_cb(lv_event_t * e)
{ 
    lv_obj_t * Temp_obj = lv_event_get_target(e);
    if(Temp_obj == prev_obj)
    {
        l_LastMusic = true - l_LastMusic;        
    }
    else if(Temp_obj == next_obj)
    {
        l_NextMusic = true - l_NextMusic;
    }
    else if(Temp_obj == play_obj)
    {
        l_MusicSwitch = true - l_MusicSwitch;
        l_LastMusic = true - l_LastMusic;
        if(l_MusicSwitch)
        {
            Image1_Animation(ui_Image1, 0);
        }
        else
        {
            lv_anim_del_all();      
        }
    }
    else if(Temp_obj == volum_obj)
    {
        int Value = lv_slider_get_value(volum_obj);
        audio2.setVolume(Value);
    }
    // else if(Temp_obj == ui_Checkbox2)
    // {
    //     l_MusicPlayLoop = true - l_MusicPlayLoop;
    // }


}

/****************************************************************
// Summary: 	创建图片
****************************************************************/
static void Picture_Creat(lv_obj_t * parent)
{
	img = lv_img_create(parent);
	lv_img_set_src(img, &lv_img_BaBaLa);
	lv_obj_set_pos(img, 10, -10);//设置图片位置
//	lv_img_set_angle(img, 0);//设置图片旋转角
//	lv_img_set_zoom(img,500);//设置图片缩放
}

/****************************************************************
// Summary: 	创建音乐
****************************************************************/
static void music_creat(lv_obj_t * parent)
{
    LV_IMG_DECLARE(img_lv_demo_music_btn_loop);
    LV_IMG_DECLARE(img_lv_demo_music_btn_rnd);
    LV_IMG_DECLARE(img_lv_demo_music_btn_next);
    LV_IMG_DECLARE(img_lv_demo_music_btn_prev);
    LV_IMG_DECLARE(img_lv_demo_music_btn_play);
    LV_IMG_DECLARE(img_lv_demo_music_btn_pause);
    LV_IMG_DECLARE(ui_img_asd_png);

    ui_Image1 = lv_img_create(parent);
    lv_img_set_src(ui_Image1, &ui_img_asd_png);
    lv_obj_set_width( ui_Image1, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_Image1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( ui_Image1, LV_ALIGN_CENTER );
    lv_obj_add_flag( ui_Image1, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( ui_Image1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_align_to(ui_Image1, parent, LV_ALIGN_TOP_MID, 0, -10);

    /*Create the control box*/
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_height(cont, LV_SIZE_CONTENT);
    lv_obj_set_width(cont, 80);    
    lv_obj_set_style_pad_bottom(cont, 8, 0);

    static const lv_coord_t grid_col[] = {LV_GRID_FR(2), LV_GRID_FR(3),LV_GRID_FR(5), LV_GRID_FR(5), LV_GRID_FR(5), LV_GRID_FR(3), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t grid_row[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont, grid_col, grid_row);
    lv_obj_align_to(cont, parent, LV_ALIGN_TOP_MID, 0, 85);

    play_obj = lv_imgbtn_create(cont);
    lv_imgbtn_set_src(play_obj, LV_IMGBTN_STATE_RELEASED, NULL, &img_lv_demo_music_btn_play, NULL);
    lv_imgbtn_set_src(play_obj, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &img_lv_demo_music_btn_pause, NULL);
    lv_obj_add_flag(play_obj, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_grid_cell(play_obj, LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_CENTER, 0, 1);


    lv_obj_add_event_cb(play_obj, music_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(play_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_width(play_obj, img_lv_demo_music_btn_play.header.w);

    prev_obj = lv_img_create(parent);
    lv_img_set_src(prev_obj, &img_lv_demo_music_btn_prev);
    lv_obj_set_grid_cell(prev_obj, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_align_to(prev_obj, cont, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(prev_obj, music_event_cb, LV_EVENT_CLICKED, NULL);

    next_obj = lv_img_create(parent);
    lv_img_set_src(next_obj, &img_lv_demo_music_btn_next);
    lv_obj_set_grid_cell(next_obj, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_align_to(next_obj, cont, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(next_obj, music_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(next_obj, LV_OBJ_FLAG_CLICKABLE);

    volum_obj = lv_slider_create(parent);
    lv_slider_set_range(volum_obj, 0,30);
    lv_slider_set_value(volum_obj, 8, LV_ANIM_OFF); 
    lv_obj_set_width( volum_obj, 200);
    lv_obj_set_height( volum_obj, 10);
    lv_obj_align_to(volum_obj, cont, LV_ALIGN_BOTTOM_MID, 0, 15);

    lv_obj_set_style_bg_color(volum_obj, lv_color_hex(0xea57ee), LV_PART_KNOB | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(volum_obj, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(volum_obj, lv_color_hex(0xea57ee), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(volum_obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(volum_obj, music_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

/****************************************************************
// Summary: 	显示函数
****************************************************************/
void ViewDisplay(void)
{
	tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 30);

	lv_obj_set_style_text_font(tabview, &my_font_24, LV_PART_MAIN| LV_STATE_DEFAULT);
	lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "图片");
	lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "音乐");
	lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "灯光");

	Picture_Creat(tab1);
	music_creat(tab2);
	Light_CreateEX(tab3);
    lv_obj_set_style_bg_color(tab1, lv_color_hex(0xDFFFFE), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(tab1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tab2, lv_color_hex(0xDFFFFE), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(tab2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tab3, lv_color_hex(0xDFFFFE), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(tab3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(tab1), lv_color_hex(0xFF0031),  LV_PART_MAIN | LV_STATE_DEFAULT );
    // lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(tab1), 255,  LV_PART_MAIN| LV_STATE_DEFAULT);
}
/*******************************************************************************
* Description : LVGL显示触摸控制程序
*******************************************************************************/
#include "lv_port_disp_indev.h"
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "Touch.h"

static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight ];
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

/****************************************************************
// Summary: 	lvgl显示函数
****************************************************************/
static void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

/****************************************************************
// Summary: 	获取触摸坐标
****************************************************************/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/
    (*x) = TPR_Structure.x[0];
    (*y) = TPR_Structure.y[0];
}
/****************************************************************
// Summary: 	lvgl触摸函数
****************************************************************/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;
    uint8_t a = 0;
    FT6236_Scan();

    a= TPR_Structure.TouchSta;
    /*Save the pressed coordinates and the state*/
    if((a)>0) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    data->point.x = last_x;
    data->point.y = last_y;
}

/****************************************************************
// Summary: LVGL显示触摸初始化函数
****************************************************************/
void InitLvglDispIndev(void)
{
    tft.begin();          /* TFT init */
    tft.setRotation( 0 ); /* Landscape orientation, flipped */
    FT6236_Init();

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
}
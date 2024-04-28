#include <Arduino.h>
#include "WiFi.h"
#include <lvgl.h>
#include "lv_port_disp_indev.h"
#include <Adafruit_NeoPixel.h>
#include "Music.h"
#include "ViewMusic.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t TaskMusicHandle, TaskLvglHandle;

void TaskMusic(void *pvParameters)
{
  while (1) {
      MusicLoop();
      vTaskDelay(2); 
  }
}

void TaskLvgl(void *pvParameters) 
{
  while (1) {
      lv_timer_handler(); 
      vTaskDelay(5);
  }
}

void setup() {
  Serial.begin(9600);
  MusicInit();

  lv_init();
  InitLvglDispIndev();
  ViewDisplay();

  xTaskCreate(TaskMusic, "Task1", 10000, NULL, 2, &TaskMusicHandle);
  xTaskCreate(TaskLvgl, "Task2", 10000, NULL, 1, &TaskLvglHandle);
}

void loop() {

}


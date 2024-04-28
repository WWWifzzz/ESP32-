/****************************************************************
// Summary: 音乐驱动文件（还包含了按键和彩灯）
****************************************************************/
#define _MUSIC_C_

#include <Arduino.h>
#include "WiFi.h"
#include "Audio2.h"
#include "Music.h"

#define I2S_DOUT      38
#define I2S_BCLK      2
#define I2S_LRC       1
#define KEY           47
#define LED_PIN       48 

Audio2 audio2(false, 3, I2S_NUM_1);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);

const char *wifiData[][2] = {
    {"WIFI3", "wifi密码2"},
    {"wifi1", "wifi密码1"}, 
    {"wifi2", "wifi密码2"}, 
};

int l_music_num = -1;
int l_music_id = 460542191;
String l_Ask = "";
bool l_InitMusic = true;

bool l_LastMusic = false;
bool l_NextMusic = false;
bool l_MusicSwitch = false;
bool l_MusicPlayLoop = true;

/****************************************************************
// Summary: WIFI连接
****************************************************************/
void wifiConnect(const char *wifiData[][2], int numNetworks)
{
    uint32_t color;
    WiFi.disconnect(true);
    for (int i = 0; i < numNetworks; ++i)
    {
        const char *ssid = wifiData[i][0];
        const char *password = wifiData[i][1];

        Serial.print("Connecting to ");
        Serial.println(ssid);

        WiFi.begin(ssid, password);
        uint8_t count = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            count++;
            if (count >= 30)
            {
                Serial.printf("\r\n-- wifi connect fail! --");
                break;
            }
            vTaskDelay(100);
        }

        if (WiFi.status() == WL_CONNECTED)
        {
          color = strip.Color(0, 0, 50);
          strip.setPixelColor(0, color);
          strip.show();
          return;
        }
        else
        {
          color = strip.Color(50, 0, 0);
          strip.setPixelColor(0, color);
          strip.show();
        }
    }
}

/****************************************************************
// Summary: 音乐播放
****************************************************************/
void Music_Play(int * pmusic_num)
{
  int music_num = *pmusic_num;
  audio2.stopSong();
  switch (music_num)
  {
    case 0:
        l_music_id = 2124731026;//5:20AM
      break;
    case 1:
        l_music_id = 1999642609;//给你一瓶魔法药水
      break;
    case 2:
        l_music_id = 30431367;//走马
      break;
    case 3:
        l_music_id = 1930045752;//后来
      break;
    case 4:
        l_music_id = 2005708456;//志铭
      break;
    case 5:
        l_music_id = 1886715284;//遇见克莱因蓝
      break;
    case 6:
        l_music_id = 1423527466;//白山茶
      break;
    case 7:
        l_music_id = 526464145;//喜欢
      break;
    case 8:
        l_music_id = 1882041083;//Love Story
      break;
    case 9:
        l_music_id = 447281999;//闪耀
      break;
    case 10:
        l_music_id = 569724852;//唱腔另类
      break;
    case 11:
        l_music_id = 2132193789;//寂寞烟火
      break;
    case 12:
        l_music_id = 1824918397;//EveryBody
      break;
    case 13:
        l_music_id = 26494698;//米店
      break;
    case 14:
        l_music_id = 1409329655;//最甜情歌
      break;
    case 15:
        l_music_id = 465921195;//还是分开
      break;
    case 16:
        l_music_id = 1805639523;//奇迹再现
      break;
    case 17:
        l_music_id = 1859236179;//仙儿
      break;
    case 18:
        l_music_id = 31311695;//shot
      break;
    case 19:
        l_music_id = 1842927464;//Waiting For Love
      break;
    case 20:
        l_music_id = 473120817;//Payphone
      break;
    case 21:
        l_music_id = 1373641092;//我走后
      break;
    case 22:
        l_music_id = 1383023012;//逍遥叹
      break;
    case 23:
        l_music_id = 1830754188;//宿敌
      break;
    case 24:
        l_music_id = 1363948882;//世界美好与你环环相扣
      break;
    default:
        l_music_id = 460542191;//一腔诗意喂了狗
        if(music_num < 0)
        {
          music_num = 25;
        }
        else
        {
          music_num = 0;
        } 
        *pmusic_num = music_num;
      break;
  }
  String music_url = "https://music.163.com/song/media/outer/url?id=" + String(l_music_id) + ".mp3";//网易 
  audio2.connecttohost(music_url.c_str()); 
  Serial.println(music_url);
  Serial.println(l_music_num);
}

/****************************************************************
// Summary: 初始化函数
****************************************************************/
void MusicInit(void) 
{
  pinMode(KEY, INPUT_PULLUP);
  strip.begin(); 

  int numNetworks = sizeof(wifiData) / sizeof(wifiData[0]);
  wifiConnect(wifiData, numNetworks);

  audio2.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio2.setVolume(8);
  l_Ask = "你好，我是小吴同学，欢迎收听我的音乐   ";
  audio2.connecttospeech(l_Ask.c_str(), "zh");
}

/****************************************************************
// Summary: 执行函数函数
****************************************************************/
void MusicLoop(void) 
{
  audio2.loop();
  if(l_InitMusic && audio2.isplaying == 0)
  {
    l_InitMusic = false;
  }

  if(Serial.available())// put streamURL in serial monitor
  { 
      audio2.stopSong();
      String r = Serial.readString(); 
      r.trim();
      if(r.length()>5) 
        audio2.connecttohost(r.c_str());
      log_i("free heap=%i", ESP.getFreeHeap());
  }

 if(l_MusicSwitch)
 {
  if(digitalRead(KEY) == LOW)
  {
      vTaskDelay(5);
      if(digitalRead(KEY) == LOW)
      {
        l_music_num += 1;
        Music_Play(&l_music_num);
        while(digitalRead(KEY) == LOW);
      }
  }

  if(l_LastMusic)
  {
    l_music_num += 1;
    l_LastMusic = false;
    Music_Play(&l_music_num);
  }

  if(l_NextMusic)
  {
    l_music_num -= 1;
    l_NextMusic = false;
     Music_Play(&l_music_num);
  }

  if(l_MusicPlayLoop &&  audio2.isplaying == 0)
  {
    l_music_num += 1;
    Music_Play(&l_music_num);
  }
  
 }
 else
 {
  if(l_InitMusic == false)
  {
   audio2.stopSong();
   audio2.isplaying = 0;
  }
 }

}

#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifndef _MUSIC_C_
	#define MUSIC_EXT  extern
#else
	#define MUSIC_EXT 
#endif




void MusicInit(void);
void MusicLoop(void);









#endif




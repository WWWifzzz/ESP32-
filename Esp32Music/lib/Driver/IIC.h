/*******************************************************************************
* Description : I2C控制程序头文件
*******************************************************************************/

#ifndef _I2C_H_
#define _I2C_H_

#include <Arduino.h>


void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_WaitAck(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void I2C_WriteByte(uint8_t uint8_tData);
uint8_t I2C_ReadByte(uint8_t uint8_tAck);


#endif
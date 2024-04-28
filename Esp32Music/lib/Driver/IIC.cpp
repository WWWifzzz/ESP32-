/*******************************************************************************
* Description        : I2C控制程序
*******************************************************************************/

#include "IIC.h"

//延时参数定义
#define I2C_DELAY 6

#define SDA_PIN 12
#define SCL_PIN 11

#define I2C0_CLK_0			digitalWrite(SCL_PIN, LOW)
#define I2C0_CLK_1			digitalWrite(SCL_PIN, HIGH)
#define I2C0_SDA_0			digitalWrite(SDA_PIN, LOW)
#define I2C0_SDA_1			digitalWrite(SDA_PIN, HIGH)
#define I2C0_READSDA		digitalRead(SDA_PIN)


/**********************************本地函数声明***********************************/
static void I2C_SetSDAOut(void);
static void I2C_SetSDAIn(void);


/****************************************************************
// Summary: 	使数据引脚为输出状态
// Parameter: 	[uint8_t ]对应I2C
//
// return:		[void]
****************************************************************/
static void I2C_SetSDAOut(void)
{
    pinMode(SDA_PIN, OUTPUT);
}

/****************************************************************
// Summary: 	使数据引脚为输入状态
// Parameter: 	[uint8_t ]对应I2C
//
// return:		[void]
****************************************************************/
static void I2C_SetSDAIn(void)
{
    pinMode(SDA_PIN, INPUT_PULLDOWN);
}

/****************************************************************
// Summary: 	产生I2C START信号
// Parameter: 	[void]
//
// return:		[void]
****************************************************************/
void I2C_Start(void)
{
	I2C_SetSDAOut();
	I2C0_SDA_1;
	I2C0_CLK_1;
	delayMicroseconds(I2C_DELAY);
	I2C0_SDA_0;;
	delayMicroseconds(I2C_DELAY);
	I2C0_CLK_0;
}

/****************************************************************
// Summary: 	产生I2C STOP信号
// Parameter: 	[void]
//
// return:		[void]
****************************************************************/
void I2C_Stop(void)
{
	I2C_SetSDAOut();
	I2C0_CLK_0;
	I2C0_SDA_0;
	delayMicroseconds(I2C_DELAY);
	I2C0_CLK_1;
	I2C0_SDA_1;
	delayMicroseconds(I2C_DELAY);
}

/****************************************************************
// Summary: 	等待应答信号
// Parameter: 	[void]
//
// return:		[out/uint8_t] 成功与否
****************************************************************/
uint8_t I2C_WaitAck(void)
{
	uint8_t u8ErrTime = 0;

	I2C_SetSDAIn();
	I2C0_SDA_1;
	delayMicroseconds(I2C_DELAY);
	I2C0_CLK_1;;
	delayMicroseconds(I2C_DELAY);
	while(I2C0_READSDA == HIGH)
	{
		u8ErrTime ++;
		delayMicroseconds(I2C_DELAY);
		if(u8ErrTime > 250)
		{
			I2C_Stop();
			return false;
		}
	}
	I2C0_CLK_0;

	return true;
}

/****************************************************************
// Summary: 	主机产生ACK应答
// Parameter: 	[void]
//
// return:		[void]
****************************************************************/
void I2C_Ack(void)
{
	I2C0_CLK_0;
	I2C_SetSDAOut();
	I2C0_SDA_0;
	delayMicroseconds(I2C_DELAY);
	I2C0_CLK_1;
	delayMicroseconds(I2C_DELAY);
	I2C0_CLK_0;
}

/****************************************************************
// Summary: 	主机不产生ACK应答
// Parameter: 	[void]
//
// return:		[void]
****************************************************************/
void I2C_NoAck(void)
{
	I2C0_CLK_0;
	I2C_SetSDAOut();
	I2C0_SDA_1;
	delayMicroseconds(I2C_DELAY);
	I2C0_CLK_1;
	delayMicroseconds(I2C_DELAY);
	I2C0_CLK_0;
}

/****************************************************************
// Summary: 	I2C发送一个字节
// Parameter: 	[in/uint8_t]u8Data 发送的字节
//
// return:		[void]
****************************************************************/
void I2C_WriteByte(uint8_t u8Data)
{
	uint8_t i;

	I2C_SetSDAOut();
	I2C0_CLK_0;
	for(i = 0; i < 8; i ++)
	{
		if((u8Data & 0x80) >> 7)
			I2C0_SDA_1;
		else
			I2C0_SDA_0;

		u8Data <<= 1;
		delayMicroseconds(I2C_DELAY);
		I2C0_CLK_1;;
		delayMicroseconds(I2C_DELAY);
		I2C0_CLK_0;
		delayMicroseconds(I2C_DELAY);
	}
}

/****************************************************************
// Summary: 	读一个字节
// Parameter: 	[in/uint8_t]u8Ack u8Ack=1时,发送ACK; u8Ack=0,发送nACK
//
// return:		[out/uint8_t]返回得到的字节
****************************************************************/ 
uint8_t I2C_ReadByte(uint8_t u8Ack)
{
	uint8_t i, u8Recv = 0;

	I2C_SetSDAIn();
	for(i = 0; i < 8; i ++)
	{
		I2C0_CLK_0;
		delayMicroseconds(I2C_DELAY);
		I2C0_CLK_1;;
		u8Recv <<= 1;
		if(I2C0_READSDA == HIGH)
			u8Recv ++;

		delayMicroseconds(I2C_DELAY); 
	}
	if(!u8Ack)
		I2C_NoAck();
	else
		I2C_Ack();

	return u8Recv;
}

/****************************************************************
// Summary: 	i2c初始化
// Parameter: 	[void]
//
// return:		[void]
****************************************************************/ 
void I2C_Init(void)
{
    pinMode(SCL_PIN, OUTPUT);
    pinMode(SDA_PIN, OUTPUT);

	I2C0_SDA_1;
	I2C0_CLK_1;
}


/*******************************************************************************
*Summary: 触摸驱动文件
*******************************************************************************/
#define _TOUCH_C_

#include "Touch.h"
#include "IIC.h"

#define IIC_RST	14
#define LCD_INT	13

#define	IIC_RES_Clr()	digitalWrite(IIC_RST, LOW);
#define	IIC_RES_Set()	digitalWrite(IIC_RST, HIGH);

uint8_t initbuf1[2]={FT_DEVIDE_MODE, 0};
uint8_t initbuf2[2]={FT_ID_G_THGROUP, 8};			/* 触摸有效值,越小越灵敏*/
uint8_t initbuf3[2]={FT_ID_G_PERIODACTIVE, 12};		/* 激活周期，不能小于12，最大14  */

const uint16_t FT6236_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};

/****************************************************************
// Summary: 	引脚初始化
****************************************************************/
void InitGPIO(void)
{
    pinMode(IIC_RST, OUTPUT);
    pinMode(LCD_INT, INPUT_PULLDOWN);
}

/****************************************************************
// Summary: 	发送
****************************************************************/
void I2C_Master_Transmit( uint8_t* buf, uint16_t count) 
{
	I2C_Start();
	I2C_WriteByte(FT_CMD_WR);					//写模式
	I2C_Ack();
	for (int i = 0; i < count; i++)
	{
		I2C_WriteByte(buf[i]);					//逐个字节写入数据
		I2C_Ack();
	}
	I2C_Stop();
}

/****************************************************************
// Summary: 	接收
****************************************************************/
void I2C_Master_Receive(uint8_t reg, uint8_t* buf, uint16_t len) 
{
	I2C_Start();
	I2C_WriteByte(FT_CMD_WR);					//写模式
	I2C_Ack();
	I2C_WriteByte(reg);							//写入寄存器地址
	I2C_Ack();
	I2C_Stop();

	I2C_Start();
	I2C_WriteByte(FT_CMD_RD);					//读模式
	I2C_Ack();
	for (int i = 0; i < len; i++) 
	{
		buf[i] = I2C_ReadByte(1);				//逐个字节读取数据
	}
	I2C_Stop();
}

/****************************************************************
// Summary: 	写
****************************************************************/
void FT6236_WR_Reg(uint8_t *buf)
{
	I2C_Master_Transmit(buf, sizeof(buf));
}

/****************************************************************
// Summary: 	读
****************************************************************/
void FT6236_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	I2C_Master_Receive(reg, buf, len);
}

/****************************************************************
// Summary: 	初始化
****************************************************************/
void FT6236_Init(void)
{
	I2C_Init();
	InitGPIO();
	IIC_RES_Clr();
	delay(50);
	IIC_RES_Set();
	delay(100);
	FT6236_WR_Reg(initbuf1);
	delay(5);
	FT6236_WR_Reg(initbuf2);
	delay(5);
	FT6236_WR_Reg(initbuf3);
	delay(5);
	IIC_RES_Set();
}

/****************************************************************
// Summary: 	触摸扫描
****************************************************************/
void FT6236_Scan(void)
{
	uint8_t i=0;
	static uint8_t sta = 0;
	static uint8_t buf[4] = {0};

	I2C_Master_Receive(0x02, &sta, 1);
	if(sta & 0x0f)													//判断是否有触摸点按下，0x02寄存器的低4位表示有效触点个数
	{
		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));			//~(0xFF << (sta & 0x0F))将点的个数转换为触摸点按下有效标志
		for(i=0;i<5;i++)											//分别判断触摸点1-5是否被按下
		{
			if(TPR_Structure.TouchSta & (1<<i))						//读取触摸点坐标
			{														//被按下则读取对应触摸点坐标数
				I2C_Master_Receive(FT6236_TPX_TBL[i], buf, 4);
				delay(50);
				TPR_Structure.x[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1];
				TPR_Structure.y[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
				if((buf[0]&0XC0)!=0X80)
				{
					TPR_Structure.x[i]=TPR_Structure.y[i] = 0;		//必须是contact事件，才认为有效
					return;
				}
			}
		}
		TPR_Structure.TouchSta |= TP_PRES_DOWN;						//触摸按下标记
	}
	else
	{
		if(TPR_Structure.TouchSta &TP_PRES_DOWN) 					//之前是被按下的
			TPR_Structure.TouchSta &= ~0x80;						//触摸松开标记
		else
		{
			TPR_Structure.x[0] = 0;
			TPR_Structure.y[0] = 0;
			TPR_Structure.TouchSta &=0xe0;							//清楚触摸点有效标记
		}
	} 
}



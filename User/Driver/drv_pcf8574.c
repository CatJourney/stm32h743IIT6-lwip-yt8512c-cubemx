/**
  ******************************************************************************
  * @file       user\driver\drv_pcf8574.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-10
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */
/* includes -------------------------------------------------------------------*/
#include "drv_pcf8574.h"

/* macro ----------------------------------------------------------------------*/


/* variables ------------------------------------------------------------------*/


/* struct ---------------------------------------------------------------------*/


/* Private function prototypes ------------------------------------------------*/


/* code -----------------------------------------------------------------------*/

/**
  * @brief   pcf8574_Init：检查PCF8574是否在位
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
uint8_t pcf8574_Init(void)
{
    uint8_t temp = 0;
	i2c_Start(&PCF5837_IIC);
	
    i2c_SendByte(&PCF5837_IIC, PCF8574_ADDR);	/* 写地址 */
	
    temp = i2c_WaitAck(&PCF5837_IIC);     		/* 等待应答,通过判断是否有ACK应答,来判断PCF8574的状态 */
	
    i2c_Stop(&PCF5837_IIC);               		/* 产生一个停止条件 */
	
    pcf8574_write_byte(0XFF);  					/* 默认情况下所有IO输出高电平 */
	
    return temp;
}


/**
  * @brief   pcf8574_write_byte：向PCF8574写入8位IO值
  *
  * @note    none
  *
  * @param   data:要写入的数据
  *
  * @retval  none
  */
void pcf8574_write_byte(uint8_t data)
{
    i2c_Start(&PCF5837_IIC); 
    i2c_SendByte(&PCF5837_IIC, PCF8574_ADDR | 0X00);   /* 发送器件地址0X40,写数据 */
    i2c_WaitAck(&PCF5837_IIC);
    i2c_SendByte(&PCF5837_IIC, data);                  /* 发送字节 */
    i2c_WaitAck(&PCF5837_IIC);
    i2c_Stop(&PCF5837_IIC);                           /* 产生一个停止条件  */
}


/**
  * @brief   pcf8574_read_byte：读取PCF8574的8位IO值
  *
  * @note    none
  *
  * @param   data:要写入的数据
  *
  * @retval  读到的数据
  */
uint8_t pcf8574_read_byte(void)
{ 
    uint8_t temp = 0;

    i2c_Start(&PCF5837_IIC); 
    i2c_SendByte(&PCF5837_IIC, PCF8574_ADDR | 0X01);                   /* 进入接收模式 */
    i2c_WaitAck(&PCF5837_IIC);
	temp = i2c_ReadByte(&PCF5837_IIC, 0);
    i2c_Stop(&PCF5837_IIC);                                           /* 产生一个停止条件 */

    return temp;
}


/**
  * @brief   pcf8574_write_bit：设置PCF8574某个IO的高低电平
  *
  * @note    none
  *
  * @param   bit  : 要设置的IO编号,0~7
  *
  * @param   sta  : IO的状态;0或1
  *
  * @retval  读到的数据
  */
void pcf8574_write_bit(uint8_t bit, uint8_t sta)
{
    uint8_t data;

    data = pcf8574_read_byte();     /* 先读出原来的设置 */
    if (sta == 0)
    {
        data &= ~(1 << bit);
    }
    else
    {
        data |= 1 << bit;
    }
    pcf8574_write_byte(data);       /* 写入新的数据 */
}


/**
  * @brief   pcf8574_read_bit：读取PCF8574的某个IO的值
  *
  * @note    none
  *
  * @param   bit：要读取的IO编号,0~7
  *
  * @retval  此IO的值,0或1
  */
uint8_t pcf8574_read_bit(uint8_t bit)
{
    uint8_t data;

    data = pcf8574_read_byte();     /* 先读取这个8位IO的值  */
    if (data & (1 << bit))
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}  


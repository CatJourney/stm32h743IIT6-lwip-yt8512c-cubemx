#ifndef __HALMYI2C_H
#define __HALMYI2C_H




/* includes ----------------------------------------------------------------------------------------------*/
#include "global.h"


/* macro ------------------------------------------------------------------------------------------------*/
#define	I2C_ACK   	1 		// 应答
#define	I2C_NACK  	0


#define I2C_WR		0x00	// 写
#define I2C_RD		0x01	// 读



#define I2C_SCL_GPIO_Port	I2C2_SCL_GPIO_Port
#define I2C_SCL_Pin			I2C2_SCL_Pin

#define I2C_SDA_GPIO_Port	I2C2_SDA_GPIO_Port
#define I2C_SDA_Pin			I2C2_SDA_Pin



/* typedef ------------------------------------------------------------------------------------------------*/
typedef struct 
{
	GPIO_TypeDef * SCL_GPIO_Port;
	uint32_t SCL_Pin;

	GPIO_TypeDef * SDA_GPIO_Port;
	uint32_t SDA_Pin;
	
}soft_i2c_TypeDef;


/* Exported functions prototypes ------------------------------------------------------------------------*/
void i2c_Ack(soft_i2c_TypeDef *soft_i2c);
void i2c_NAck(soft_i2c_TypeDef *soft_i2c);
void i2c_Start(soft_i2c_TypeDef *soft_i2c);
void i2c_Stop(soft_i2c_TypeDef *soft_i2c);
void _hal_soft_i2c_Init(void);
void i2c_SendByte(soft_i2c_TypeDef *soft_i2c, uint8_t _ucByte);
static void i2c_Delay(void);
uint8_t i2c_ReadByte(soft_i2c_TypeDef *soft_i2c, uint8_t ack);
uint8_t i2c_WaitAck(soft_i2c_TypeDef *soft_i2c);



//extern soft_i2c_TypeDef soft_i2c1;
extern soft_i2c_TypeDef soft_i2c2;


#endif /* __HALMYI2C_H */


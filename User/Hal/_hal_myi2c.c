
/**
  ------------------------------------------------------------------------------
  * @file    _hal_myi2c.c
  * @brief   软件IIC 驱动（参考野火、原子哥）
  ------------------------------------------------------------------------------

	应用说明：
	在访问i2c设备前，请先调用 i2c_CheckDevice() 检测i2c设备是否正常，该函数会配置GPIO。
	1）将需要模拟的端口在 stm32cubemx 中 Enter User Label -> i2c1_SCL,i2c1_SDA。
	2) 配置gpio输出模式为 开漏输出【前提是芯片外部或者模块有上拉电阻，负责配置为推挽输出】，高速。
	3) SCL -> 推挽输出、上拉、快速
	4）SDA -> 开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平
	3）在主函数中调用 soft_i2c_Init(void)即可。
*/

/* Includes -------------------------------------------------------------------*/
#include "_hal_myi2c.h"


//soft_i2c_TypeDef soft_i2c1;
soft_i2c_TypeDef soft_i2c2;
/* code -----------------------------------------------------------------------*/
/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_GPIO_Config
*	@brief: 配置i2c总线的GPIO，采用模拟IO的方式实现
*	@param：无
*	@retval: 无
---------------------------------------------------------------------------------------------------------
*/
void _hal_soft_i2c_Init(void)
{

    soft_i2c2.SCL_GPIO_Port = I2C_SCL_GPIO_Port;
    soft_i2c2.SCL_Pin = I2C_SCL_Pin;

    soft_i2c2.SDA_GPIO_Port = I2C_SDA_GPIO_Port;
    soft_i2c2.SDA_Pin = I2C_SDA_Pin;


    /* 给一个停止信号, 复位i2c总线上的所有设备到待机模式 */
    i2c_Stop(&soft_i2c2);
}


void i2c_SCL(soft_i2c_TypeDef *soft_i2c, uint8_t level)
{
	level ? HAL_GPIO_WritePin(soft_i2c->SCL_GPIO_Port, soft_i2c->SCL_Pin, GPIO_PIN_SET) :
			HAL_GPIO_WritePin(soft_i2c->SCL_GPIO_Port, soft_i2c->SCL_Pin, GPIO_PIN_RESET); 
}


void i2c_SDA(soft_i2c_TypeDef *soft_i2c, uint8_t level)
{
	level ? HAL_GPIO_WritePin(soft_i2c->SDA_GPIO_Port, soft_i2c->SDA_Pin, GPIO_PIN_SET) :
			HAL_GPIO_WritePin(soft_i2c->SDA_GPIO_Port, soft_i2c->SDA_Pin, GPIO_PIN_RESET); 
}


/* SCL ：推挽输出、SDA ：开漏输出    -->  读取SDA状态无须重新配置为输入模式 */

GPIO_PinState i2c_SDA_READ(soft_i2c_TypeDef *soft_i2c)
{
    return HAL_GPIO_ReadPin(soft_i2c->SDA_GPIO_Port, soft_i2c->SDA_Pin);
}

/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_Start
*	@brief: CPU发起i2c总线启动信号
*	@param：无
*	@retval: 无
---------------------------------------------------------------------------------------------------------
*/
void i2c_Start(soft_i2c_TypeDef *soft_i2c)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示i2c总线启动信号 */
    i2c_SDA(soft_i2c, 1);
    i2c_SCL(soft_i2c, 1);
    i2c_Delay();

    i2c_SDA(soft_i2c, 0);
    i2c_Delay();

    i2c_SCL(soft_i2c, 0);
    i2c_Delay();
}



/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_Stop
*	@brief: CPU发起i2c总线停止信号
*	@param：无
*	@retval: 无
---------------------------------------------------------------------------------------------------------
*/
void i2c_Stop(soft_i2c_TypeDef *soft_i2c)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示i2c总线停止信号 */
    i2c_SDA(soft_i2c, 0);
	i2c_Delay();
	    
    i2c_SCL(soft_i2c, 1);
	i2c_Delay();
	
    i2c_SDA(soft_i2c, 1);
    i2c_Delay();
}







/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_SendByte
*	@brief: CPU向i2c总线设备发送8bit数据
*	@param：_ucByte ： 等待发送的字节
*	@retval: 无
---------------------------------------------------------------------------------------------------------
*/
void i2c_SendByte(soft_i2c_TypeDef *soft_i2c, uint8_t _ucByte)
{

    for (uint8_t i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            i2c_SDA(soft_i2c, 1);
        }
        else
        {
            i2c_SDA(soft_i2c, 0);
        }

        i2c_Delay();
        i2c_SCL(soft_i2c, 1);
        i2c_Delay();
        i2c_SCL(soft_i2c, 0);

        if (i == 7)
        {
            i2c_SDA(soft_i2c, 1); // 释放总线
        }

        _ucByte <<= 1;	/* 左移一个bit */
        i2c_Delay();

    }
}


/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_ReadByte
*	@brief: CPU从i2c总线设备读取8bit数据
*	@param：无
*	@retval: 读到的数据
---------------------------------------------------------------------------------------------------------
*/
uint8_t i2c_ReadByte(soft_i2c_TypeDef *soft_i2c, uint8_t ack)
{
    uint8_t value = 0;

    /* 读到第1个bit为数据的bit7 */
    for (uint8_t i = 0; i < 8; i++)
    {
        value <<= 1;
		
        i2c_SCL(soft_i2c, 1);
        i2c_Delay();

        if (i2c_SDA_READ(soft_i2c))
            value++;

        i2c_SCL(soft_i2c, 0);
        i2c_Delay();

    }

    if (!ack)
        i2c_NAck(soft_i2c);
    else
        i2c_Ack(soft_i2c);

    return value;
}


/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_WaitAck
*	@brief: CPU产生一个时钟，并读取器件的ACK应答信号
*	@param：无
*	@retval: 返回0接收应答成功，1表示无器件响应
---------------------------------------------------------------------------------------------------------
*/
uint8_t i2c_WaitAck(soft_i2c_TypeDef *soft_i2c)
{
    uint8_t waittime = 0;
	uint8_t ret = 0;
	
    i2c_SDA(soft_i2c, 1);	/* CPU释放SDA总线 */
    i2c_Delay();
    i2c_SCL(soft_i2c, 1);	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    i2c_Delay();

    while(i2c_SDA_READ(soft_i2c))
    {
        waittime++;

        if(waittime > 250)
        {
            i2c_Stop(soft_i2c);
            ret = 1;
        }
    }

    i2c_SCL(soft_i2c, 0);
	i2c_Delay();
	
    return ret;
}

/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_Ack
*	@brief: CPU产生一个ACK信号
*	@param：无
*	@retval: 无
---------------------------------------------------------------------------------------------------------
*/
void i2c_Ack(soft_i2c_TypeDef *soft_i2c)
{
    i2c_SDA(soft_i2c, 0);	/* CPU驱动SDA = 0 */
    i2c_Delay();

    i2c_SCL(soft_i2c, 1);	/* CPU产生1个时钟 */
    i2c_Delay();
	
    i2c_SCL(soft_i2c, 0);
    i2c_Delay();
	
    i2c_SDA(soft_i2c, 1);	/* CPU释放SDA总线 */
	i2c_Delay();
}

/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_NAck
*	@brief: CPU产生1个NACK信号
*	@param：无
*	@retval: 无
---------------------------------------------------------------------------------------------------------
*/
void i2c_NAck(soft_i2c_TypeDef *soft_i2c)
{
    i2c_SDA(soft_i2c, 1);	/* CPU驱动SDA = 1 */
    i2c_Delay();
	
    i2c_SCL(soft_i2c, 1);	/* CPU产生1个时钟 */
    i2c_Delay();
	
    i2c_SCL(soft_i2c, 0);
    i2c_Delay();
}
/**
---------------------------------------------------------------------------------------------------------
*	@funNm: i2c_Delay
*	@brief: i2c总线位延迟，最快400KHz
*	@param：无
*	@retval: 无
---------------------------------------------------------------------------------------------------------
*/
static void i2c_Delay(void)//大概3us
{
    for (uint8_t i = 0; i < 120; i++)__NOP();
}



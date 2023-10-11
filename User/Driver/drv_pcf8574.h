#ifndef __DRVPCF8574_H
#define __DRVPCF8574_H


/* includes ----------------------------------------------------------------------------------------------*/
#include "global.h"


/* macro ------------------------------------------------------------------------------------------------*/
#define PCF5837_IIC                 soft_i2c2

#define PCF8574_ADDR  				0X40      /* PCF8574地址(左移了一位) */

/* 正点原子 h7 v2：PCF8574各个IO的功能 */
#define BEEP_IO         			0       /* 蜂鸣器控制引脚        P0 */
#define AP_INT_IO       			1       /* AP3216C中断引脚       P1 */
#define DCMI_PWDN_IO    			2       /* DCMI的电源控制引脚    P2 */
#define USB_PWR_IO      			3       /* USB电源控制引脚       P3 */
#define EX_IO           			4       /* 扩展IO,自定义使用     P4 */
#define MPU_INT_IO      			5       /* SH3001中断引脚        P5 */
#define RS485_RE_IO     			6       /* RS485_RE引脚          P6 */
#define ETH_RESET_IO    			7       /* 以太网复位引脚        P7 */

/* typedef ------------------------------------------------------------------------------------------------*/


/* Exported functions prototypes ------------------------------------------------------------------------*/
uint8_t pcf8574_Init(void);
uint8_t pcf8574_read_byte(void);
void pcf8574_write_byte(uint8_t data);
void pcf8574_write_bit(uint8_t bit, uint8_t sta);
uint8_t pcf8574_read_bit(uint8_t bit);


#endif /* __DRVPCF8574_H */


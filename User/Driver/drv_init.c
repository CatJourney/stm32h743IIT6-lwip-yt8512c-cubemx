/**
  ******************************************************************************
  * @file       user\driver\drv_init.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */
/* Includes -------------------------------------------------------------------*/
#include "drv_init.h"



/* code -----------------------------------------------------------------------*/

/**
  * @brief   drv_all_Init：所有传感器、外设芯片、外部设备初始化
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
  
void drv_all_Init(void)
{
	pcf8574_Init();
}

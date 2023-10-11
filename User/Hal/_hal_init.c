/**
  ******************************************************************************
  * @file       user\hal\_hal_init.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "_hal_init.h"


/* code -----------------------------------------------------------------------*/

/**
 * @funNm   
 * @brief   
 * @param	
*/
/**
  * @brief   _hal_all_Init：所有外设接口初始化
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
void _hal_all_Init(void)
{    
	_hal_soft_i2c_Init();
    _hal_usart_Init();
}

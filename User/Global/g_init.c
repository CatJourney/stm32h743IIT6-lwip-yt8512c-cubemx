/**
  ******************************************************************************
  * @file       user\global\g_init.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */

#include "g_init.h"

/* Includes -------------------------------------------------------------------*/
#include "drv_init.h"
#include "_hal_init.h"



/**
  * @brief   g_Init：所有初始化汇总：DLL、Driver
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
void g_Init(void)
{
    _hal_all_Init(); 
    drv_all_Init();        
}


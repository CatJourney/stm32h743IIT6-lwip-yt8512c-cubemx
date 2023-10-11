/**
  ******************************************************************************
  * @file       user\os\os_init.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */


/* Includes -------------------------------------------------------------------*/
#include "os_init.h"


/* code -----------------------------------------------------------------------*/
/**
  * @brief   Os_Init：所有系统相关初始化
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
void Os_Init(void)
{
    Os_Queue_Init();
    Os_Semaphore_Init();
    Os_Task_Init();
}


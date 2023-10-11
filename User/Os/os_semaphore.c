/**
  ******************************************************************************
  * @file       user\os\os_queue.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "os_semaphore.h"


/*-内核对象句柄-信号量-*/
SemaphoreHandle_t PC_tx_xMutex_Handle = NULL;


/*-函数声明-*/
void xMutex_Semaphore_Create(void);


/* code -----------------------------------------------------------------------*/
/**
  * @brief   Os_Semaphore_Init：所有信号量相关初始化
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
void Os_Semaphore_Init(void)
{
//    xMutex_Semaphore_Create();
}



/**
  * @brief   xMutex_Semaphore_Create：互斥信号量初始化
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
void xMutex_Semaphore_Create(void)
{
    do{
		PC_tx_xMutex_Handle = xSemaphoreCreateMutex();
	}while(PC_tx_xMutex_Handle == NULL);
}



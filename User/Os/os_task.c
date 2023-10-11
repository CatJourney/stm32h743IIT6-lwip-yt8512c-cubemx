/**
  ******************************************************************************
  * @file       user\os\os_task.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "os_task.h"



/*-任务句柄-*/
osThreadId  HeartbeatTaskHandle;    /* 心跳任务句柄 */

osThreadId  UPLinkTaskHandle;       /* 消息任务句柄 */

osThreadId  DownLinkTaskHandle;		/* 消息任务句柄 */



/*-函数声明-*/
void HeartbeatTask_Function(void const * argument);

void UPLinkTask_Function(void const * argument);

void DownLinkTask_Function(void const * argument);



/* code -----------------------------------------------------------------------*/

/**
  * @brief   Os_Task_Init：所有任务的创建
  *
  * @note    definition and creation of xxxTask
  *
  * @param   none
  *
  * @retval  none
  */
  
void Os_Task_Init(void)/*任务入口函数、任务名字、任务栈大小、任务入口函数参数、任务优先级、任务控制块指针*/
{
	BaseType_t xReturn = pdPASS;

    /* 心跳任务 */
    xReturn = xTaskCreate((TaskFunction_t)HeartbeatTask_Function, "HeartbeatTask", 128, NULL, (UBaseType_t)osPriorityLow, &HeartbeatTaskHandle);
	

    /* 消息任务 */
    xReturn = xTaskCreate((TaskFunction_t)DownLinkTask_Function, "DownLinkTask", 1024, NULL, osPriorityAboveNormal, &DownLinkTaskHandle);
	
	
    /* 消息任务 */
    xReturn = xTaskCreate((TaskFunction_t)UPLinkTask_Function, "UPLinkTask", 1024, NULL, osPriorityAboveNormal, &UPLinkTaskHandle);
	
	
	
	if (xReturn == pdPASS)
	{
		;
	}
}


/*---------------------------------------Function implementing the xxxTask thread----------------------------------------*/

__weak void HeartbeatTask_Function(void const * argument)
{
    for(;;)
    {
        osDelay(1);
    }
}



__weak void DownLinkTask_Function(void const * argument)
{
    for(;;)
    {
        osDelay(1);
    }
}

__weak void UPLinkTask_Function(void const * argument)
{
    for(;;)
    {
        osDelay(1);
    }
}


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
#include "os_queue.h"



/*-内核对象句柄-队列-*/
QueueHandle_t Sensor_Queue		= NULL;




/*-函数声明-*/
void Sensor_Queue_Create(void);



/* code --------------------------------------------------------------------------------------------------------*/

/**
  * @brief   Os_Queue_Init：所有队列相关初始化
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
  
void Os_Queue_Init(void)
{
    //Sensor_Queue_Create();
}





void Sensor_Queue_Create(void)
{
    do {
        Sensor_Queue = xQueueCreate(1, sizeof(int));		
    } while(Sensor_Queue == NULL);
}



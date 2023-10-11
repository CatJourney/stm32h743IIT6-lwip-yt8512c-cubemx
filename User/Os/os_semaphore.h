#ifndef __OSSEMAPHORE_H
#define __OSSEMAPHORE_H


/* Private includes ----------------------------------------------------------*/

#include "global.h"



/* Exported functions prototypes ------------------------------------------------------------------------*/
void Os_Semaphore_Init(void);



/* Exported constants --------------------------------------------------------*/
extern SemaphoreHandle_t PC_tx_xMutex_Handle;
extern SemaphoreHandle_t Usart6_xMutex_Handle;

#endif



#ifndef __OSTASK_H
#define __OSTASK_H

/* Private includes ----------------------------------------------------------*/

#include "global.h"



/* Exported functions prototypes ------------------------------------------------------------------------*/
void Os_Task_Init(void);



/* Exported constants --------------------------------------------------------*/
extern osThreadId  HeartbeatTaskHandle;    /* 心跳任务句柄 */

extern osThreadId  UPLinkTaskHandle;       /* 消息任务句柄 */

extern osThreadId  DownLinkTaskHandle;		/* 消息任务句柄 */

#endif


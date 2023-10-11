/**
  ******************************************************************************
  * @file       user\global\global.h
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */
#ifndef __GLOBAL_H
#define __GLOBAL_H

/* Private includes ----------------------------------------------------------*/

/* C 语言标准库 */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>


/* CubeMX生成 */
#include "main.h"
//#include "iwdg.h"
#include "usart.h"
#include "gpio.h"

//#include "tim.h"


/* freeRTOS 相关 */
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "stream_buffer.h"

/* Os */
#include "os_task.h"
#include "os_queue.h"
#include "os_semaphore.h"

/* GL */
#include "g_runtime.h"



/* _hal */
#include "_hal_usart.h"
#include "_hal_myi2c.h"

/* driver */
#include "drv_fifo.h"
#include "drv_pcf8574.h"




/* macro ------------------------------------------------------------------------------------------------*/
#define ON                          1
#define OFF                         0
#define TRUE 						1
#define FALSE 						0


#define PI                          3.1415926f


/*- I/O 输出-*/

#define System_Run_Led(x)        	do{ x ? \
										HAL_GPIO_WritePin(System_Run_Led_GPIO_Port, System_Run_Led_Pin, GPIO_PIN_SET) : \
										HAL_GPIO_WritePin(System_Run_Led_GPIO_Port, System_Run_Led_Pin, GPIO_PIN_RESET); \
									}while(0)		/* System Run Led */

								
								
								
/*- I/O 输入-*/

#define get_sys_time_msec() 		HAL_GetTick()
								
								
								
#define CONSTRAIN(x,max,min)    	(x > max ? max : (x < min ? min : x))



#endif /* __GLOBAL_H */

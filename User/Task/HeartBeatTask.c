/**
  ******************************************************************************
  * @file       user\task\HeartBeatTask.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "HeartBeatTask.h"


void HeartBeat_Sign (void);



/**
 * @funNm : HeartbeatTask_Function
 * @brief : 心跳
 * @param : argument
 * @retval: void
 */
void HeartbeatTask_Function (void const* argument)
{
	
    while (1)
    {
        TaskRunTimeStat.HeartBeatTask.threads_runtime   = GetTask_RunTime (HeartBeatTaskID);
        TaskRunTimeStat.HeartBeatTask.threads_counter   = GetTask_Beatcnt (HeartBeatTaskID);
        TaskRunTimeStat.HeartBeatTask.threads_freestack = Get_Free_Stack (HeartBeatTaskID);

        HeartBeat_Sign();               // 指示灯、蜂鸣器

//        HAL_IWDG_Refresh(&hiwdg);       //喂狗

        osDelay (1000);
    }
}


void HeartBeat_Sign (void)
{
    System_Run_Led (OFF);

    osDelay (50);

    System_Run_Led (ON);
}




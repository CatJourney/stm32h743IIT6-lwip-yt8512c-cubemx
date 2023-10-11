/**
  ******************************************************************************
  * @file       user\global\g_runtime.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "g_runtime.h"




/* variables ------------------------------------------------------------------*/
TaskRunTimeTypeDef  TaskRunTimeStat;



/* code -----------------------------------------------------------------------*/

/**
  * @brief   GetRunTime：计算线程运行间隔时间
  *
  * @note    none
  *
  * @param   taskID  : 任务ID
  *
  * @retval  runtime : 任务周期
  */
  
uint32_t GetTask_RunTime(uint8_t taskID)
{
    static uint32_t lasttime[Task_combined] = {0};

    uint32_t runtime = 0;
    uint32_t curtime = HAL_GetTick();

    runtime = curtime - lasttime[taskID]; //计算线程运行间隔时间  runtime运行一次

    lasttime[taskID] = curtime;

    return runtime;
}

/**
  * @brief   GetTask_Beatcnt：线程运行计数,用于判断线程是否在运行
  *
  * @note    none
  *
  * @param   taskID  : 任务ID
  *
  * @retval  beatcnt[taskID] : 线程运行计数
  */

uint32_t GetTask_Beatcnt(uint8_t taskID)
{
    static uint8_t beatcnt[Task_combined] = {0};

    beatcnt[taskID]++;

    if(beatcnt[taskID] >= 10)
    {
        beatcnt[taskID] = 0;
    }

    return beatcnt[taskID];
}



/**
  * @brief   Get_Free_Stack：获取剩余任务栈大小
  *
  * @note    none
  *
  * @param   taskID  : 任务ID
  *
  * @retval  free_stack[taskID] : 剩余任务栈大小
  */

uint32_t Get_Free_Stack(uint8_t taskID)
{
    static uint32_t free_stack[Task_combined] = {0};

    free_stack[taskID] = uxTaskGetStackHighWaterMark(NULL);

    return free_stack[taskID];
}

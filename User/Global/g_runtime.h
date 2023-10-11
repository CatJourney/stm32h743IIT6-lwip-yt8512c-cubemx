
#ifndef __GRUNTIME_H
#define __GRUNTIME_H


/* includes ----------------------------------------------------------------------------------------------*/
#include "global.h"



/* macro ------------------------------------------------------------------------------------------------*/
#define     Task_combined           8




/* struct ------------------------------------------------------------------------------------------------*/
typedef struct TaskRunTime
{
    uint32_t    threads_runtime;    // 运行时间
    uint32_t    threads_counter;    // 任务计数
    uint32_t    threads_freestack;   // 剩余栈

} TaskRunTime;



enum Task_ID
{
     HeartBeatTaskID = 0,
     DownLinkTaskID,
	 UPLinkTaskID,
           
};



typedef struct TaskRunTimeTypeDef
{
    TaskRunTime   HeartBeatTask;
    TaskRunTime   DownLinkTask;
	TaskRunTime   UPLinkTask;

} TaskRunTimeTypeDef;



/* Exported functions prototypes ------------------------------------------------------------------------*/
uint32_t GetTask_RunTime(uint8_t taskID);
uint32_t GetTask_Beatcnt(uint8_t taskID);
uint32_t Get_Free_Stack(uint8_t taskID);

/* Exported constants -----------------------------------------------------------------------------------*/
extern TaskRunTimeTypeDef  TaskRunTimeStat;


#endif /* __GRUNTIME_H */

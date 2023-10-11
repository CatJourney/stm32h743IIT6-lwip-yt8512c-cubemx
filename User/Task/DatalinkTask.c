/**
  ******************************************************************************
  * @file       user\task\DataLinkTask.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DataLinkTask.h"

#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"


/* typedef --------------------------------------------------------------------*/
#define DATA_LINK_UART_ID		uart1_id
#define TEST_RX_BUF_SIZE     	(1024+256)


static uint8_t TESTRxBuf[TEST_RX_BUF_SIZE];		// 数传数据
/* variables ------------------------------------------------------------------*/



/* code -----------------------------------------------------------------------*/
static struct netconn* datalink_conn;

#define SET_LINK_REMOTER_IP(addr) 	IP4_ADDR(&addr, 10, 12, 19, 218)
#define LINK_RMOTER_PORT        	6001
#define LINK_SERVER_PORT        	6001



/**
  * @brief   DownLinkTask_Function：串 -> 网
  *
  * @note    none
  *
  * @param   taskID  : 任务ID
  *
  * @retval  runtime : 任务周期
  */
  
void DownLinkTask_Function (void const* argument)
{
    struct netbuf* datalink_buf;
	ip_addr_t snd_ip_addr;

	int16_t count;
	
	/* 等待 lwip 初始化完成 */
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	
	datalink_conn = netconn_new (NETCONN_UDP);
    netconn_bind (datalink_conn, IP_ADDR_ANY, LINK_SERVER_PORT);
	
	datalink_buf = netbuf_new();
	SET_LINK_REMOTER_IP (snd_ip_addr);

	/*datalink_conn 初始化完成 发送任务通知*/
	xTaskNotifyGive(UPLinkTaskHandle);
    while (1)
    {
        /*---------------------------------------------------获取任务运行状态---------------------------------------------------*/

        TaskRunTimeStat.DownLinkTask.threads_runtime   = GetTask_RunTime (DownLinkTaskID);
        TaskRunTimeStat.DownLinkTask.threads_counter   = GetTask_Beatcnt (DownLinkTaskID);
        TaskRunTimeStat.DownLinkTask.threads_freestack = Get_Free_Stack (DownLinkTaskID);


        /*recv 1: 飞控----------------------------------------------------------------*/
		count = Uart_ReceiveBuffer(DATA_LINK_UART_ID, TESTRxBuf, TEST_RX_BUF_SIZE, 20) ;

        if (count > 0)
        {			
            netbuf_ref (datalink_buf, TESTRxBuf, count);

            netconn_sendto (datalink_conn, datalink_buf, &snd_ip_addr, LINK_RMOTER_PORT);
        }
	}
}





/**
  * @brief   UPLinkTask_Function：网 -> 串
  *
  * @note    none
  *
  * @param   taskID  : 任务ID
  *
  * @retval  runtime : 任务周期
  */
  
void UPLinkTask_Function (void const* argument)
{
    struct netbuf* datalink_buf;

    err_t recv_err;

    uint8_t* data;
    uint16_t len;

	/* 等待 datalink_conn 初始化完成 */
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	
    while (1)
    {
        /*---------------------------------------------------获取任务运行状态---------------------------------------------------*/
        TaskRunTimeStat.UPLinkTask.threads_runtime   = GetTask_RunTime (UPLinkTaskID);
        TaskRunTimeStat.UPLinkTask.threads_counter   = GetTask_Beatcnt (UPLinkTaskID);
        TaskRunTimeStat.UPLinkTask.threads_freestack = Get_Free_Stack (UPLinkTaskID);

        /*recv : 网口----------------------------------------------------------------*/
		recv_err = netconn_recv (datalink_conn, &datalink_buf);
        if (recv_err == ERR_OK)
        {
            do
            {
                netbuf_data (datalink_buf, (void*) &data, &len);

				Uart_SendBuffer(DATA_LINK_UART_ID, data, len, 50);				
            }
            while (netbuf_next (datalink_buf) >= 0);

            netbuf_delete (datalink_buf);
        }
	}
}




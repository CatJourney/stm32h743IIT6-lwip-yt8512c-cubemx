
#ifndef __HALUSART_H
#define __HALUSART_H

/* Suppress warning messages */
#if defined(__CC_ARM)
// Suppress warning message: extended constant initialiser used
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#endif

/* includes ----------------------------------------------------------------------------------------------*/
#include "global.h"
#include "drv_fifo.h"



/* macro ------------------------------------------------------------------------------------------------*/
#define UART_DMA_MODE				0	// 0:Circular	1:  Normal
#define UART_SYSTEM_SUPPORT_OS		1	// 0:裸机		1:OS  


#define UART1_TX_BUFFER_SIZE    4096
#define UART1_RX_BUFFER_SIZE    4096




/* struct ------------------------------------------------------------------------------------------------*/
typedef struct  uart_dev_t
{
    uint32_t 			Rx_Timestamp;	// 时间戳
    
    uint16_t 			Rx_Size;		// receive length

	t_fifo_buffer 		rxbuf;			// 环形缓冲区	
    t_fifo_buffer 		txbuf;
	
#if UART_SYSTEM_SUPPORT_OS
	SemaphoreHandle_t 	rx_sem;			// 二值信号量
    SemaphoreHandle_t 	tx_sem;
    SemaphoreHandle_t 	sendbuffer_sem;
#endif
	
    uint16_t 			tx_rd;
    bool 				tx_busy;
	
	UART_HandleTypeDef  *uart_handle;	// 串口句柄
} uart_dev_t;





/* Exported functions prototypes ------------------------------------------------------------------------*/
void _hal_usart_Init (void);
int16_t Uart_ReceiveBuffer (uint32_t id, uint8_t* buf, uint16_t size, uint32_t timeout_ms);
int32_t Uart_SendBuffer (uint32_t id, const uint8_t* buffer, uint16_t len,  uint32_t timeout_ms);


/* Exported constants --------------------------------------------------------*/
extern const uint32_t uart1_id;




#endif /* __HALUSART_H */


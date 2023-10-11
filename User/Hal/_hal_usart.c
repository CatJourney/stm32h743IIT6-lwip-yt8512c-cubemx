/**
  ******************************************************************************
  * @file       user\hal\_hal_usart.c
  * @author     liangky
  * @version    v0.1
  * @date       2023-10-11
  * @brief      Briefly describe the function of your function
  ******************************************************************************
  */

/* Includes -------------------------------------------------------------------*/
#include "_hal_usart.h"


/* variables ------------------------------------------------------------------*/
static uint8_t uart_rx_buffer[UART1_RX_BUFFER_SIZE];
static uint8_t uart_tx_buffer[UART1_TX_BUFFER_SIZE];

/* struct ---------------------------------------------------------------------*/
static struct uart_dev_t uart_dev1 =
{
    .uart_handle = &huart1,
};

const uint32_t uart1_id = (uint32_t)& uart_dev1;



/* Private function prototypes ------------------------------------------------*/
HAL_StatusTypeDef _hal_uart_Init(uint32_t id, uint8_t *rxbuf_ptr, uint16_t rxbuf_size, uint8_t *txbuf_ptr, uint16_t txbuf_size);
static int32_t Uart_SendBufferNonBlockingInternal (struct uart_dev_t* dev, const uint8_t* buffer, uint16_t len);
static int32_t Uart_StartTX (struct uart_dev_t* dev);




/* code -----------------------------------------------------------------------*/
/**
  * @brief   _hal_usart_Init：所有串口初始化
  *
  * @note    none
  *
  * @param   none
  *
  * @retval  none
  */
  
void _hal_usart_Init(void)
{
    _hal_uart_Init(uart1_id, uart_rx_buffer, UART1_RX_BUFFER_SIZE, uart_tx_buffer, UART1_TX_BUFFER_SIZE);	// xxxS
}


/**
  * @brief   _hal_usart_Init：所有串口初始化
  *
  * @note    none
  *
  * @param   uart_dev_t  : 串口 uart_dev(x)
  *
  * @retval  dev 是否为 NULL
  */

static bool Uart_Validate (struct uart_dev_t* dev)
{
    return (dev != NULL);
}



/**
  * @brief   _hal_usart_Init：串口初始化
  *
  * @note    none
  *
  * @param   id：uart(x)_dev_t addr
  *
  * @param   *(t/r)buf_ptr：缓存地址、(t/r)buf_size：缓存长度
  *
  * @retval  runtime : 任务周期
  */

HAL_StatusTypeDef _hal_uart_Init(uint32_t id, uint8_t *rxbuf_ptr, uint16_t rxbuf_size, uint8_t *txbuf_ptr, uint16_t txbuf_size)
{
    struct uart_dev_t* dev = (struct uart_dev_t*) id;

    if (!Uart_Validate (dev))
        return HAL_ERROR;

    HAL_UARTEx_ReceiveToIdle_DMA(dev->uart_handle, rxbuf_ptr, rxbuf_size);	//  1、启动 DMA 接收   2、清除中断标志位     3、使能 IDLE 中断

    fifoBuf_init(&dev->rxbuf, rxbuf_ptr, rxbuf_size);						// 环形缓冲区初始化

    fifoBuf_init(&dev->txbuf, txbuf_ptr, txbuf_size);

    #if UART_SYSTEM_SUPPORT_OS
    dev->rx_sem = xSemaphoreCreateBinary();									// 信号量 -> 超时处理
	
    dev->tx_sem = xSemaphoreCreateBinary();

    dev->sendbuffer_sem = xSemaphoreCreateBinary();

    xSemaphoreGive(dev->sendbuffer_sem);
    #endif
	
    return HAL_OK;
}



/**
  * @brief   USER_UARTEx_RxEventCallback：用户串口接收回调
  *
  * @note    none
  *
  * @param   id：uart(x)_dev_t addr
  *
  * @param   rx_size: 数据接收长度、*rbuf_ptr：缓存地址、rxbuf_size：缓存长度
  *
  * @retval  none
  */

void USER_UARTEx_RxEventCallback(uint32_t id, uint16_t rx_size, uint8_t *rxbuf_ptr, uint16_t rxbuf_size)
{
    #if UART_SYSTEM_SUPPORT_OS
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    #endif

    struct uart_dev_t* dev = (struct uart_dev_t*) id;

    dev->Rx_Timestamp = get_sys_time_msec();

    dev->Rx_Size = rx_size;

    // 放入环形环形缓冲区
	#if UART_DMA_MODE
    fifoBuf_putData(&dev->rxbuf, rxbuf_ptr, rx_size);
	#else
	dev->rxbuf.wr = rx_size;
	#endif
	
    #if UART_SYSTEM_SUPPORT_OS
    xSemaphoreGiveFromISR(dev->rx_sem, &xHigherPriorityTaskWoken);
    #endif

    /* DMA是正常模式，则需要重新开启 DMA 接收 -> 每次进入中断结束后，接收模式会被置为标准模式 */
	#if UART_DMA_MODE
    HAL_UARTEx_ReceiveToIdle_DMA(dev->uart_handle, buf_ptr, buf_size);
	#endif
}


/**
  * @brief   HAL_UARTEx_RxEventCallback（系统虚函数）
  *
  * @note    需要关闭 uart rx dma 中断、不然一包数据会进两次中断
  *
  * @param   *huart  串口句柄、Size：数据接收长度
  *
  * @retval  none
  */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)		// xxx
    {
        USER_UARTEx_RxEventCallback(uart1_id, Size, uart_rx_buffer, UART1_RX_BUFFER_SIZE);
    }
}


/**
  * @brief   USER_UART_TxCpltCallback：用户串口发送回调
  *
  * @note    none
  *
  * @param   id：uart(x)_dev_t addr
  *
  * @retval  none
  */

void USER_UART_TxCpltCallback(uint32_t id)
{
    struct uart_dev_t* dev = (struct uart_dev_t*) id;
	#if UART_SYSTEM_SUPPORT_OS
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	#endif

    if (dev->tx_busy == TRUE)
    {
        dev->tx_busy = FALSE;
        dev->txbuf.rd = dev->tx_rd;
		
		#if UART_SYSTEM_SUPPORT_OS
        xSemaphoreGiveFromISR(dev->tx_sem, &xHigherPriorityTaskWoken);
		#else
		
		#endif
        Uart_StartTX(dev);
    }
}


/**
  * @brief   HAL_UART_TxCpltCallback（系统虚函数）
  *
  * @note    需要打开 uart tx dma 中断
  *
  * @param   *huart  串口句柄
  *
  * @retval  none
  */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)		
    {
        USER_UART_TxCpltCallback(uart1_id);
    }
}

/**
  * @brief   Uart_ReceiveBuffer：对外接口：串口读API（本质上是从环形缓冲区读出数据）
  *
  * @note    none
  *
  * @param   id：uart(x)_dev_t addr
  *
  * @param   *buf: 缓存地址、size：缓存长度、timeout_ms：超时时间
  *
  * @retval  Return received byte
  */

int16_t Uart_ReceiveBuffer (uint32_t id, uint8_t* buf, uint16_t size, uint32_t timeout_ms)
{
    uint16_t bytes_from_fifo;

    struct uart_dev_t* dev = (struct uart_dev_t*) id;

    bytes_from_fifo = fifoBuf_getData (&dev->rxbuf, buf, size);

    if (bytes_from_fifo == 0)
    {
        /* No more bytes in receive buffer */
        /* Make sure the receiver is running while we wait */
        #if UART_SYSTEM_SUPPORT_OS
        if (xSemaphoreTake (dev->rx_sem, timeout_ms) == pdTRUE)
        {
            bytes_from_fifo = fifoBuf_getData (&dev->rxbuf, buf, size);
        }
        else
        {
            return -1;
        }

        #else
        uint32_t Basetime = HAL_GetTick();
        uint32_t nowtime = Basetime;
        Basetime = HAL_GetTick();
        do
        {
            bytes_from_fifo = fifoBuf_getData (&dev->rxbuf, buf, size);
            nowtime = HAL_GetTick();
        }
        while(bytes_from_fifo == 0 && (nowtime - Basetime) < timeout_ms);

		if (!bytes_from_fifo)
			return -1;
		else
			return bytes_from_fifo;
        #endif
    }

    /* Return received byte */
    return bytes_from_fifo;
}


/**
  * @brief   Uart_SendBuffer：对外接口：串口发API（本质上是从环形缓冲区写入数据）
  *
  * @note    none
  *
  * @param   id：uart(x)_dev_t addr、buffer character buffer、len buffer length
  *
  * @retval  -1 if port not available
  *
  * @retval  -2 if mutex can't be taken;
  *
  * @retval  number of bytes transmitted on success
  *
  */
int32_t Uart_SendBuffer (uint32_t id, const uint8_t* buffer, uint16_t len,  uint32_t timeout_ms)
{
    struct uart_dev_t* dev = (struct uart_dev_t*) id;
    uint32_t ticks, elapse;
    int32_t ret = 0;
	#if UART_SYSTEM_SUPPORT_OS
    BaseType_t xEntryTimeSet = pdFALSE;
	#endif
    if (!Uart_Validate (dev))
    {
        /* Undefined COM port for this board (see pios_board.c) */
        return -1;
    }

    ticks = timeout_ms;

    uint32_t start_time = get_sys_time_msec();
	
	#if UART_SYSTEM_SUPPORT_OS
    if (xSemaphoreTake (dev->sendbuffer_sem, timeout_ms) != pdTRUE)
        return -2;
	#else
	
	#endif
	
    uint32_t max_frag_len  = fifoBuf_getSize (&dev->txbuf);
    uint32_t bytes_to_send = len;

    while (bytes_to_send)
    {
        uint32_t frag_size;

        if (bytes_to_send > max_frag_len)
        {
            frag_size = max_frag_len;
        }
        else
        {
            frag_size = bytes_to_send;
        }

        int32_t rc = Uart_SendBufferNonBlockingInternal (dev, buffer, frag_size);

        if (rc >= 0)
        {
            bytes_to_send -= rc;
            buffer += rc;
            ret += rc;
        }
        else
        {
            //case -2:
            /* Device is busy, wait for the underlying device to free some space and retry */
            elapse = get_sys_time_msec() - start_time;

            if ( (ticks != 0) && (elapse >= ticks))
                break;
			
			#if UART_SYSTEM_SUPPORT_OS
            if (ticks == portMAX_DELAY)
                xEntryTimeSet = xSemaphoreTake (dev->tx_sem, portMAX_DELAY);
            else
                xEntryTimeSet = xSemaphoreTake (dev->tx_sem, ticks - elapse);

            if (xEntryTimeSet != pdTRUE)
                break;
			#else
			
			#endif
        }
    }

	#if UART_SYSTEM_SUPPORT_OS
    xSemaphoreGive(dev->sendbuffer_sem);
	#else
	
	#endif
    return ret;
}


static int32_t Uart_SendBufferNonBlockingInternal (struct uart_dev_t* dev, const uint8_t* buffer, uint16_t len)
{

    if (len > fifoBuf_getFree (&dev->txbuf))
    {
        /* Buffer cannot accept all requested bytes (retry) */
        return -2;
    }

    uint16_t bytes_into_fifo = fifoBuf_putData (&dev->txbuf, buffer, len);

    if (bytes_into_fifo > 0)
    {
        /* More data has been put in the tx buffer, make sure the tx is started */
        Uart_StartTX (dev);
    }

    return bytes_into_fifo;
}


static int32_t Uart_StartTX (struct uart_dev_t* dev)
{
    if(dev->tx_busy)
        return -1;

    t_fifo_buffer* buf = & (dev->txbuf);
    uint16_t rd = buf->rd;

    uint16_t wr = buf->wr;
    uint16_t rd1 = wr;
    uint16_t buf_size  = buf->buf_size;
    uint8_t* buff      = buf->buf_ptr;
    uint16_t num_bytes = wr - rd;

    if (wr < rd)
    {
        num_bytes = (buf_size - rd);// + wr;
        rd1 = 0;
    }


    if (num_bytes == 0)
        return -1;

    HAL_UART_Transmit_DMA(dev->uart_handle, &buff[rd], num_bytes);

    dev->tx_rd = rd1;
    dev->tx_busy = TRUE;

    return 0;
}

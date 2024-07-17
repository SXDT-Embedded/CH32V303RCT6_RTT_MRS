/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */

#include "board.h"
#include <rtdevice.h>
#include <drv_usart.h>
#include "bsp_hr.h"
#include "lwutil.h"


#ifdef RT_USING_SERIAL

//#define DRV_DEBUG
#define LOG_TAG              "drv.uart"
#include <drv_log.h>


#define ARRAY_LEN(arry)                         (sizeof(arry) / sizeof(arry[0]))
#define UART2_RX_DMA_BUFFER_LEN (32u)
#define UART4_RX_DMA_BUFFER_LEN (64u)


lwrb_t uart2_rx_rb;
lwrb_t uart4_rx_rb;

uint8_t uart2_rx_rb_data[UART2_RX_BUFFER_LENGTH];
uint8_t uart4_rx_rb_data[UART4_RX_BUFFER_LENGTH];

uint8_t uart2_rx_dma_buffer[UART2_RX_DMA_BUFFER_LEN];
uint8_t uart4_rx_dma_buffer[UART4_RX_DMA_BUFFER_LEN];

rt_inline void _UART2_ProcessData(const void* data, rt_size_t len)
{
    lwrb_write(&uart2_rx_rb, data, len);
}

rt_inline void _UART4_ProcessData(const void* data, rt_size_t len)
{
    lwrb_write(&uart4_rx_rb, data, len);
}

void UART2_RxCheck(void)
{
    static uint16_t old_pos;
    uint16_t pos;

    /* Calculate current position in buffer and check for new data available */
    pos = ARRAY_LEN(uart2_rx_dma_buffer) - DMA_GetCurrDataCounter(UART2_DMA_RX_CHANNEL);
    //     printf("old_pos = %d, pos = %d\r\n", old_pos, pos);
    if (pos != old_pos)
    { /* Check change in received data */
        if (pos > old_pos)
        { /* Current position is over previous one */
            /*
             * Processing is done in "linear" mode.
             *
             * Application processing is fast with single data block,
             * length is simply calculated by subtracting pointers
             *
             * [   0   ]
             * [   1   ] <- old_pos |------------------------------------|
             * [   2   ]            |                                    |
             * [   3   ]            | Single block (len = pos - old_pos) |
             * [   4   ]            |                                    |
             * [   5   ]            |------------------------------------|
             * [   6   ] <- pos
             * [   7   ]
             * [ N - 1 ]
             */

            _UART2_ProcessData(&uart2_rx_dma_buffer[old_pos], pos - old_pos);
        }
        else
        {
            /*
             * Processing is done in "overflow" mode..
             *
             * Application must process data twice,
             * since there are 2 linear memory blocks to handle
             *
             * [   0   ]            |---------------------------------|
             * [   1   ]            | Second block (len = pos)        |
             * [   2   ]            |---------------------------------|
             * [   3   ] <- pos
             * [   4   ] <- old_pos |---------------------------------|
             * [   5   ]            |                                 |
             * [   6   ]            | First block (len = N - old_pos) |
             * [   7   ]            |                                 |
             * [ N - 1 ]            |---------------------------------|
             */
            _UART2_ProcessData(&uart2_rx_dma_buffer[old_pos], ARRAY_LEN(uart2_rx_dma_buffer) - old_pos);
            if (pos > 0)
            {
                _UART2_ProcessData(&uart2_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos; /* Save current position as old for next transfers */
    }
}



void UART4_RxCheck(void)
{
    static uint16_t old_pos;
    uint16_t pos;

    /* Calculate current position in buffer and check for new data available */
    pos = ARRAY_LEN(uart4_rx_dma_buffer) - DMA_GetCurrDataCounter(UART4_DMA_RX_CHANNEL);
    //     printf("old_pos = %d, pos = %d\r\n", old_pos, pos);
    if (pos != old_pos)
    { /* Check change in received data */
        if (pos > old_pos)
        { /* Current position is over previous one */
            /*
             * Processing is done in "linear" mode.
             *
             * Application processing is fast with single data block,
             * length is simply calculated by subtracting pointers
             *
             * [   0   ]
             * [   1   ] <- old_pos |------------------------------------|
             * [   2   ]            |                                    |
             * [   3   ]            | Single block (len = pos - old_pos) |
             * [   4   ]            |                                    |
             * [   5   ]            |------------------------------------|
             * [   6   ] <- pos
             * [   7   ]
             * [ N - 1 ]
             */

            _UART4_ProcessData(&uart4_rx_dma_buffer[old_pos], pos - old_pos);
            //            lwrb_write(&usart1_rx_rb, &usart1_rx_dma_buffer[old_pos], pos - old_pos);
        }
        else
        {
            /*
             * Processing is done in "overflow" mode..
             *
             * Application must process data twice,
             * since there are 2 linear memory blocks to handle
             *
             * [   0   ]            |---------------------------------|
             * [   1   ]            | Second block (len = pos)        |
             * [   2   ]            |---------------------------------|
             * [   3   ] <- pos
             * [   4   ] <- old_pos |---------------------------------|
             * [   5   ]            |                                 |
             * [   6   ]            | First block (len = N - old_pos) |
             * [   7   ]            |                                 |
             * [ N - 1 ]            |---------------------------------|
             */
            _UART4_ProcessData(&uart4_rx_dma_buffer[old_pos], ARRAY_LEN(uart4_rx_dma_buffer) - old_pos);
            //            lwrb_write(&usart1_rx_rb, &usart1_rx_dma_buffer[old_pos], ARRAY_LEN(usart1_rx_dma_buffer) - old_pos);

            if (pos > 0)
            {
                _UART4_ProcessData(&uart4_rx_dma_buffer[0], pos);
                //                lwrb_write(&usart1_rx_rb, &usart1_rx_dma_buffer[0], pos);
            }
        }
        old_pos = pos; /* Save current position as old for next transfers */
    }
}



#if !defined(BSP_USING_UART1) && !defined(BSP_USING_UART2) && !defined(BSP_USING_UART3) && !defined(BSP_USING_UART4) && \
    !defined(BSP_USING_UART5) && !defined(BSP_USING_UART6) && !defined(BSP_USING_UART7) && !defined(BSP_USING_UART8) 
    #error "Please define at least one BSP_USING_UARTx"
    /* this driver can be disabled at menuconfig -> RT-Thread Components -> Device Drivers */
#endif

enum
{
#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif
#ifdef BSP_USING_UART2
    UART2_INDEX,
#endif
#ifdef BSP_USING_UART3
    UART3_INDEX,
#endif
#ifdef BSP_USING_UART4
    UART4_INDEX,
#endif
#ifdef BSP_USING_UART5
    UART5_INDEX,
#endif
#ifdef BSP_USING_UART6
    UART6_INDEX,
#endif
#ifdef BSP_USING_UART7
    UART7_INDEX,
#endif
#ifdef BSP_USING_UART8
    UART8_INDEX,
#endif
};

static struct ch32_uart_config uart_config[] =
{
#ifdef BSP_USING_UART1
    {
        "uart1",
        USART1,
        USART1_IRQn,
    },
#endif
#ifdef BSP_USING_UART2
    {
        "uart2",
        USART2,
        USART2_IRQn,
    },
#endif
#ifdef BSP_USING_UART3
    {
        "uart3",
        USART3,
        USART3_IRQn,
    },
#endif
#ifdef BSP_USING_UART4
    {
        "uart4",
        UART4,
        UART4_IRQn,
    },
#endif
#ifdef BSP_USING_UART5
    {
        "uart5",
        UART5,
        UART5_IRQn,
    },
#endif
#ifdef BSP_USING_UART6
    {
        "uart6",
        UART6,
        UART6_IRQn,
    },
#endif
#ifdef BSP_USING_UART7
    {
        "uart7",
        UART7,
        UART7_IRQn,
    },
#endif
#ifdef BSP_USING_UART8
    {
        "uart8",
        UART8,
        UART8_IRQn,
    },
#endif
};

static struct ch32_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};

rt_err_t uart_dma_rx_configure(struct rt_serial_device *serial,
                                uint8_t *buf,
                                DMA_Channel_TypeDef *DMAy_Channelx,
                                uint32_t RCC_AHBPeriph)
{
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (struct ch32_uart *) serial->parent.user_data;

    DMA_InitTypeDef DMA_InitStructure = {0};

    if (RCC_AHBPeriph == RCC_AHBPeriph_DMA1)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    }
    if (RCC_AHBPeriph == RCC_AHBPeriph_DMA2)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    }

    DMA_DeInit(DMAy_Channelx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&uart->config->Instance->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    if (uart->config->Instance == USART2)
    {
        DMA_InitStructure.DMA_BufferSize = UART2_RX_DMA_BUFFER_LEN;
    }
    else if (uart->config->Instance == UART4)
    {
        DMA_InitStructure.DMA_BufferSize = UART4_RX_DMA_BUFFER_LEN;
    }
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMAy_Channelx, &DMA_InitStructure);

    DMA_Cmd(DMAy_Channelx, ENABLE);
    USART_DMACmd(uart->config->Instance, USART_DMAReq_Rx, ENABLE);

    return RT_EOK;
}
static rt_err_t ch32_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (struct ch32_uart *) serial->parent.user_data;

    uart->Init.USART_BaudRate             = cfg->baud_rate;
    uart->Init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    uart->Init.USART_Mode                 = USART_Mode_Rx|USART_Mode_Tx;

    switch (cfg->data_bits)
    {
    case DATA_BITS_8:
        uart->Init.USART_WordLength = USART_WordLength_8b;
        break;
    case DATA_BITS_9:
        uart->Init.USART_WordLength = USART_WordLength_9b;
        break;
    default:
        uart->Init.USART_WordLength = USART_WordLength_8b;
        break;
    }

    switch (cfg->stop_bits)
    {
    case STOP_BITS_1:
        uart->Init.USART_StopBits   = USART_StopBits_1;
        break;
    case STOP_BITS_2:
        uart->Init.USART_StopBits   = USART_StopBits_2;
        break;
    default:
        uart->Init.USART_StopBits   = USART_StopBits_1;
        break;
    }
    switch (cfg->parity)
    {
    case PARITY_NONE:
        uart->Init.USART_Parity    = USART_Parity_No;
        break;
    case PARITY_ODD:
        uart->Init.USART_Parity    = USART_Parity_Odd;
        break;
    case PARITY_EVEN:
        uart->Init.USART_Parity    = USART_Parity_Even;
        break;
    default:
        uart->Init.USART_Parity     = USART_Parity_No;
        break;
    }

    if(uart->config->Instance==USART1)
       {
           GPIO_InitTypeDef GPIO_InitStructure={0};
           RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

           GPIO_InitStructure.GPIO_Pin = UART1_TX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
           GPIO_Init(UART1_TX_GPIO_PORT, &GPIO_InitStructure);

           GPIO_InitStructure.GPIO_Pin = UART1_RX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
           GPIO_Init(UART1_RX_GPIO_PORT, &GPIO_InitStructure);

           USART_Init(uart->config->Instance, &uart->Init);
           USART_Cmd(uart->config->Instance, ENABLE);
       }
       if(uart->config->Instance==USART2)
       {
           GPIO_InitTypeDef GPIO_InitStructure={0};
           RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

           GPIO_InitStructure.GPIO_Pin = UART2_TX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
           GPIO_Init(UART2_TX_GPIO_PORT, &GPIO_InitStructure);

           GPIO_InitStructure.GPIO_Pin = UART2_RX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
           GPIO_Init(UART2_RX_GPIO_PORT, &GPIO_InitStructure);

           uart_dma_rx_configure(serial, uart2_rx_dma_buffer, UART2_DMA_RX_CHANNEL, RCC_AHBPeriph_DMA1);

           USART_Init(uart->config->Instance, &uart->Init);
           USART_Cmd(uart->config->Instance, ENABLE);

       }
       if(uart->config->Instance == USART3)
       {
           GPIO_InitTypeDef GPIO_InitStructure={0};
           RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

           GPIO_InitStructure.GPIO_Pin = UART3_TX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
           GPIO_Init(UART3_TX_GPIO_PORT, &GPIO_InitStructure);

           GPIO_InitStructure.GPIO_Pin = UART3_RX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
           GPIO_Init(UART3_RX_GPIO_PORT, &GPIO_InitStructure);

           USART_Init(uart->config->Instance, &uart->Init);
           USART_Cmd(uart->config->Instance, ENABLE);

       }
       if(uart->config->Instance==UART4)
       {
           GPIO_InitTypeDef GPIO_InitStructure={0};
           RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

//           GPIO_InitStructure.GPIO_Pin = UART4_TX_GPIO_PIN;
//           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//           GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStructure);

           GPIO_InitStructure.GPIO_Pin = UART4_RX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
           GPIO_Init(UART4_RX_GPIO_PORT, &GPIO_InitStructure);

           uart_dma_rx_configure(serial, uart4_rx_dma_buffer, UART4_DMA_RX_CHANNEL, RCC_AHBPeriph_DMA2);


           USART_Init(uart->config->Instance, &uart->Init);
           USART_Cmd(uart->config->Instance, ENABLE);

       }
       if(uart->config->Instance==UART5)
       {
           GPIO_InitTypeDef GPIO_InitStructure={0};
           RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);

           GPIO_InitStructure.GPIO_Pin = UART5_TX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
           GPIO_Init(UART5_TX_GPIO_PORT, &GPIO_InitStructure);

           GPIO_InitStructure.GPIO_Pin = UART5_RX_GPIO_PIN;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
           GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
           GPIO_Init(UART5_RX_GPIO_PORT, &GPIO_InitStructure);

           USART_Init(uart->config->Instance, &uart->Init);
           USART_Cmd(uart->config->Instance, ENABLE);

       }
    return RT_EOK;
}

static rt_err_t ch32_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct ch32_uart *)serial->parent.user_data;
    switch (cmd)
    {
    /* disable interrupt */
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        NVIC_DisableIRQ(uart->config->irq_type);
        /* disable interrupt */

        if ((uart->config->name == "uart2")||(uart->config->name == "uart4"))//开启串口空闲中断
        {
            USART_ITConfig(uart->config->Instance, USART_IT_IDLE, DISABLE);
        }
        else
        {
            USART_ITConfig(uart->config->Instance, USART_IT_RXNE, DISABLE);
        }
        break;
    /* enable interrupt */
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        NVIC_EnableIRQ(uart->config->irq_type);
        /* enable interrupt */

        if ((uart->config->name == "uart2")||(uart->config->name == "uart4"))//开启串口空闲中断
        {
            USART_ITConfig(uart->config->Instance, USART_IT_IDLE, ENABLE);
        }
        else
        {
            USART_ITConfig(uart->config->Instance, USART_IT_RXNE, ENABLE);
        }

        break;
    }
    return RT_EOK;
}

static int ch32_putc(struct rt_serial_device *serial, char c)
{
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct ch32_uart *)serial->parent.user_data;
    while (USART_GetFlagStatus(uart->config->Instance, USART_FLAG_TC) == RESET);
    uart->config->Instance->DATAR = c;
    return 1;
}

static int ch32_getc(struct rt_serial_device *serial)
{
    int ch;
    struct ch32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = (struct ch32_uart *)serial->parent.user_data;
    ch = -1;
    if (USART_GetFlagStatus(uart->config->Instance, USART_FLAG_RXNE) != RESET)
    {
        ch = uart->config->Instance->DATAR & 0xff;
    }
    return ch;
}

//static rt_size_t _uart_dma_transmit(struct ch32_uart *instance, rt_uint8_t *buf, rt_size_t size, int direction)
//{
//    rt_enter_critical();
//
//       if(instance->config->name == "uart1")
//       {
//           if (_uart1_tx_dma_current_len == 0
//                   && (_uart1_tx_dma_current_len = lwrb_get_linear_block_read_length(&uart1_tx_rb)) > 0)
//           {
//
//               /* Disable channel if enabled */
//               DMA_Cmd(instance->dma_tx->Instance, DISABLE);
//
//               /* Clear all flags */
//               DMA_ClearFlag(DMA1_FLAG_TC4);
//               DMA_ClearFlag(DMA1_FLAG_HT4);
//               DMA_ClearFlag(DMA1_FLAG_GL4);
//               DMA_ClearFlag(DMA1_FLAG_TE4);
//
//               instance->dma_tx->Instance->CNTR = _uart1_tx_dma_current_len;
//               instance->dma_tx->Instance->MADDR = (uint32_t)lwrb_get_linear_block_read_address(&uart1_tx_rb);
//
//               /* Start transfer */
//               DMA_Cmd(instance->dma_tx->Instance, ENABLE);
//           }
//       }
//
//       rt_exit_critical();
//
//    return RT_EOK;
//}

static rt_size_t ch32dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
//    struct ch32_uart *instance;
//    RT_ASSERT(serial != RT_NULL);
//
//    instance = rt_container_of(serial, struct ch32_uart, serial);
//    RT_ASSERT(instance != RT_NULL);
//    RT_ASSERT(buf != RT_NULL);
//
//    if (size == 0)
//    {
//        return 0;
//    }
//
//    if (RT_SERIAL_DMA_TX == direction)
//    {
//        _uart_dma_transmit(instance, buf, size);
//    }

    return size;
}


static void uart_isr(struct rt_serial_device *serial)
{
    rt_uint8_t data;
    struct ch32_uart *uart = (struct ch32_uart *) serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);
    if (USART_GetITStatus(uart->config->Instance, USART_IT_RXNE) != RESET)
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
        USART_ClearITPendingBit(uart->config->Instance, USART_IT_RXNE);
    }
    else if (USART_GetITStatus(uart->config->Instance, USART_IT_IDLE) != RESET)
    {
        data = uart->config->Instance->STATR;//清除标志位
        data = uart->config->Instance->DATAR;
        data &= 0;
        serial->parent.rx_indicate(&serial->parent, 1);//这个1没有任何意义,仅表示产生空闲中断,产生空闲回调
    }
}
//static void uart_isr(struct rt_serial_device *serial) {
//    struct ch32_uart *instance;
//#ifdef RT_SERIAL_USING_DMA
//    rt_size_t recv_total_index, recv_len;
//    rt_base_t level;
//#endif
//    RT_ASSERT(serial != RT_NULL);
//
//    instance = rt_container_of(serial, struct ch32_uart, serial);
//    RT_ASSERT(instance != RT_NULL);
//
//    if (USART_GetITStatus(instance->config->Instance, USART_IT_RXNE) != RESET) {
//        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
//    }
//#ifdef RT_SERIAL_USING_DMA
//    else if (USART_GetITStatus(instance->config->Instance, USART_IT_IDLE) != RESET)
//    {
//        /* clear idle flag */
//        USART_ReceiveData(instance->config->Instance);
//
//        level = rt_hw_interrupt_disable();
//        recv_total_index = serial->config.bufsz - DMA_GetCurrDataCounter(instance->dma_rx->Instance);
//        recv_len = recv_total_index - instance->last_index;
//        instance->last_index = recv_total_index;
//        rt_hw_interrupt_enable(level);
//
//        if (recv_len)
//        {
//            rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));
//        }
//    }
//#endif
//    else
//    {
//        if (USART_GetITStatus(instance->config->Instance, USART_IT_PE) != RESET)
//        {
//            USART_GetITStatus(instance->config->Instance, USART_IT_PE);
//        }
//    }
//}


static const struct rt_uart_ops ch32_uart_ops =
{
    ch32_configure,
    ch32_control,
    ch32_putc,
    ch32_getc,
    ch32dma_transmit
};


#ifdef BSP_USING_UART1
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART1_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_USING_UART1)
void UART1_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

//    dma_rx_isr(&uart_config[UART1_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_USING_UART1) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_USING_UART1)
void UART1_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART1_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA) */
#endif

#ifdef BSP_USING_UART2
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART2_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART3
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART3_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART4
void UART4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART4_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART4_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART5
void UART5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART5_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART5_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART6
void UART6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART6_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART6_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif

#ifdef BSP_USING_UART7
void UART7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART7_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART7_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif


#ifdef BSP_USING_UART8
void UART8_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART8_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    uart_isr(&(uart_obj[UART8_INDEX].serial));
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif



int rt_hw_usart_init(void)
{
    rt_size_t obj_num = sizeof(uart_obj) / sizeof(struct ch32_uart);
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t result = 0;

        /*在此修改不属于默认参数的值*/
    for (int i = 0; i < obj_num; i++)
    {
        /* init UART object */
        uart_obj[i].config        = &uart_config[i];
        uart_obj[i].serial.ops    = &ch32_uart_ops;
        uart_obj[i].serial.config = config;
        if (uart_obj[i].config->name == "uart2")
        {
            lwrb_init(&uart2_rx_rb, uart2_rx_rb_data, sizeof(uart2_rx_rb_data)); // 设置接收缓冲区
        }
        if (uart_obj[i].config->name == "uart4")
        {
            lwrb_init(&uart4_rx_rb, uart4_rx_rb_data, sizeof(uart4_rx_rb_data));
        }
        /* register UART device */
        result = rt_hw_serial_register(&uart_obj[i].serial, uart_obj[i].config->name,
                                       RT_DEVICE_FLAG_RDWR
                                       | RT_DEVICE_FLAG_INT_RX
                                       , &uart_obj[i]);
        RT_ASSERT(result == RT_EOK);
    }

    return result;
}



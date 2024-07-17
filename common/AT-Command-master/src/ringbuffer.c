/******************************************************************************
 * @brief    环形缓冲区管理(参考linux/kfifo)
 *
 * Copyright (c) 2016~2021, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2016-05-30     roger.luo    Initial version
 * 2021-02-05     roger.luo    增加空闲空间获取接口.
 ******************************************************************************/
#include "ringbuffer.h"
#include <string.h>
#include <stddef.h>

#define min(a,b) ( (a) < (b) )? (a):(b)     
     
/*
 *@brief      构造一个空环形缓冲区
 *@param[in]  r    - 环形缓冲区管理器
 *@param[in]  buf  - 数据缓冲区
 *@param[in]  len  - buf长度(必须是2的N次幂)
 *@retval     bool
 */
bool ring_buf_init(ring_buf_t *r,unsigned char *buf, unsigned int len)
{
    r->buf    = buf;
    r->size   = len;
    r->front  = r->rear = 0;
    return buf != NULL && (len & len -1) == 0;
}

/*
 *@brief      清空环形缓冲区 
 *@param[in]  r - 待清空的环形缓冲区
 *@retval     none
 */
void ring_buf_clr(ring_buf_t *r)
{
    r->front = r->rear = 0;
}

/*
 *@brief      获取环形缓冲区数据长度
 *@retval     环形缓冲区中有效字节数 
 */
unsigned int ring_buf_len(ring_buf_t *r)
{
    return r->rear - r->front;
}

/*
 *@brief      获取环形缓冲空闲空间
 *@retval     空闲空间
 */
unsigned int ring_buf_free_space(ring_buf_t *r)
{
    return r->size - (unsigned int)(r->rear - r->front);
}

/*
 *@brief       将指定长度的数据放到环形缓冲区中 
 *@param[in]   buf - 数据缓冲区
 *             len - 缓冲区长度 
 *@retval      实际放到中的数据 
 */

unsigned int ring_buf_put(ring_buf_t *r, unsigned char *buf, unsigned int len)
{
    unsigned int i;
    unsigned int left;

    left = r->size + r->front - r->rear;//计算本次接收剩余空间大小，但是不考虑上一帧数据是否处理，如果新来数据大于剩余空间大小，则将其从头开始覆写，遵循FIFO原则
    len  = min(len , left);//判断这个剩余空间是否满足本次要写入的长度，返回一个最小值
    i    = min(len, r->size - (r->rear & r->size - 1));//计算上一帧数据尾指针的位置到缓冲区后一个字节数据的长度。

    memcpy(r->buf + (r->rear & r->size - 1), buf, i);//将数据从上一帧尾开始写入缓冲区，
    memcpy(r->buf, buf + i, len - i);//如果数据长度小于上一帧尾到最后一个字节的数据长度，则应该len = i,这时，不会往缓冲区写入，否则从头开始写入

    r->rear += len;     
    return len;
    
}

/*
 *@brief       从环形缓冲区中读取指定长度的数据 
 *@param[in]   len - 读取长度 
 *@param[out]  buf - 输出数据缓冲区
 *@retval      实际读取长度 
 */
unsigned int ring_buf_get(ring_buf_t *r,unsigned char *buf,unsigned int len)
{
    unsigned int i;
    unsigned int left;   
    if (ring_buf_len(r) == 0)
        return 0;
    left = r->rear - r->front;//判断缓冲区中数据大小
    len  = min(len , left);      //判断读取的数据长度和收到的收据长度，如果要读取的数据长度小于缓冲区中存在的，则len = len，否则，len = 缓冲区中存在的数据长度大小
    i    = min(len, r->size - (r->front & r->size - 1));//再次判断数据的开始位置和所要读取的数据长度到缓冲区最后一个字节的长度，如果len小于其长度，则len = len，否则返回从头指针到最后一个字节的长度
    memcpy(buf, r->buf + (r->front & r->size - 1), i);    //先赋值从r->front到最后一个字节的数据
    memcpy(buf + i, r->buf, len - i);   //如果要读取的数据长度小于上一帧尾到最后一个字节的数据长度，则应该len = i,这时，不会往缓冲区头部开始读取数据，否则会从头部开始继续读取数据
    r->front += len;
    return len;
}

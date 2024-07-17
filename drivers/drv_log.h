/***
 * @Author: mbw
 * @Date: 2024-06-27 10:57:13
 * @LastEditors: mbw && 1600520629@qq.com
 * @LastEditTime: 2024-06-27 10:59:06
 * @FilePath: \SCV_ControlBox_CH32V303RCT6_BC260\drivers\drv_log.h
 * @Description:
 * @
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef LOG_TAG
#define DBG_TAG               "drv"
#else
#define DBG_TAG               LOG_TAG
#endif /* LOG_TAG */

#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */

#include <rtdbg.h>

/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-08     obito0   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <sys\time.h>

#include "stdStruct.h"

#include "sys_task.h"

#include "control.h"
/*-------------------------------------------定义变量----------------------------------------------------*/
/*********************************************************************************************************
  定义任务句柄
*********************************************************************************************************/
static rt_thread_t sys_task = RT_NULL;//系统任务句柄

Struct_SysData SysData;//系统参数
Struct_monitorData monitorData;//监控数据

int main(void)
{
//led初始化	
	led_init();
//relay初始化
	relay_init();
	
//创建sys_task
	sys_task = rt_thread_create( "sys_task",
															  sys_task_entry,
																RT_NULL,
																1024,
																11,
																5
	);
	if (sys_task != RT_NULL)
	{
		rt_thread_startup(sys_task);
	}
	
	return RT_EOK;
}






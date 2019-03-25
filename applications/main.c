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
/*-------------------------------------------�������----------------------------------------------------*/
/*********************************************************************************************************
  ����������
*********************************************************************************************************/
static rt_thread_t sys_task = RT_NULL;//ϵͳ������

Struct_SysData SysData;//ϵͳ����
Struct_monitorData monitorData;//�������

int main(void)
{
//led��ʼ��	
	led_init();
//relay��ʼ��
	relay_init();
	
//����sys_task
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






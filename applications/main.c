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

#include "control.h"

#include "modbus.h"
/*-------------------------------------------定义变量----------------------------------------------------*/
uint16_t tab_reg[64]={0};

int main(void)
{
	modbus_t *mb;
	
//led初始化	
	led_init();
//relay初始化
	relay_init();
	
	mb = modbus_new_rtu("/dev/uart2", 115200, 'N', 8, 1);
	modbus_set_debug(mb, 1);
	modbus_rtu_set_serial_mode(mb,MODBUS_RTU_RS232);
	modbus_set_slave(mb,1);
	modbus_connect(mb);
	modbus_set_response_timeout(mb,0,1000000);
	
	int num = 0;
	while(1)
	{
		memset(tab_reg,0,64*2);
		int regs=modbus_read_registers(mb, 0, 20, tab_reg); 
		
		rt_kprintf("-------------------------------------------\n");
		rt_kprintf("[%4d][read num = %d]",20 - num,regs);
		num --;
		
		for(uint8_t Loop_i = 0; Loop_i < 20; Loop_i ++)
		{
			rt_kprintf("<%#x>",tab_reg[Loop_i]);
		}
		rt_kprintf("\n");
		rt_kprintf("-------------------------------------------\n");
		
		rt_thread_mdelay(1000);
	}
	modbus_close(mb); 
	modbus_free(mb);
	
	return RT_EOK;
}



//获取设备ID
void GetDeviceSN(int argc, char**argv)
{
	rt_int32_t temp;
	
	temp = *(__IO uint32_t*)(0x1FFFF7E8);//获取STM32的唯一ID
	rt_kprintf("SN:%x\n", temp);
}
MSH_CMD_EXPORT(GetDeviceSN, Get the Device SN);


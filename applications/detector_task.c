/*********************************************************************************************************
//程序名：				detector_task.c
//编写人：				高宵
//编写时间：			2019年2月26日
//版本号：				1.0.0
//备注：				  None
*********************************************************************************************************/
/*-----------------------------------------引入头文件----------------------------------------------------*/
#include "detector_task.h"
#include "stdStruct.h"

#include "modbus.h"
/*------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------定义变量----------------------------------------------------*/
modbus_t *mb;//modbus句柄

extern Struct_SysData SysData;//系统参数
extern Struct_monitorData monitorData;//监控数据

rt_uint8_t Detector_ReceiveNum;//探测器数据接收计数器
rt_uint8_t Detector_ReceiveBuffer[50];//探测器数据接收缓冲器


/*-------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------函数声明----------------------------------------------------*/
/*********************************************************************************************************
**函数描述：设备初始化
*********************************************************************************************************/
void Detector_Init(void);

/*********************************************************************************************************
**函数描述：制氮机操作
*********************************************************************************************************/
int8_t N2_Device_Op(rt_bool_t sw,uint8_t *p);

/*------------------------------------------------------------------------------------------------------*/

/*********************************************************************************************************
void detector_task_entry(void * parameter)
**函数描述：传感器任务入口函数
**输入参数：无
**返回值：  无
*********************************************************************************************************/
void detector_task_entry(void * parameter)
{
	modbus_t *mb;//句柄
	int temp;//临时变量
	
	Detector_Init();//探测器初始化
	
	mb = modbus_new_rtu("/dev/uart2", 9600, 'N', 8, 1);
	while(1)
	{
		modbus_set_slave(mb,1);
		modbus_connect(mb);
		modbus_set_response_timeout(mb,0,1000000);	
		
//获取数据
		uint16_t val[8] = {0,0,0,0,0,0,0,0};
		
		if(-1 != modbus_read_input_registers(mb, 0x00, 8, val))
		{
//处理值
			if(RT_TRUE == monitorData.updatestate)//允许本地更新数据
			{
				if(3 != SysData.Config.DeviceModel)//如果不是氮气机房设备，才有氮气传感器
				{
					int16_t *pconc = &monitorData.WareData[SysData.Config.addr - 1].conc1;
          
					
//5个氮气传感器					
					for(uint8_t Loop_i = 0;Loop_i < 5;Loop_i ++)
					{
						if(1 == SysData.Config.ChannelConfig[Loop_i].attr1)//KY-2N
						{
//工程值 = ((量程上限 - 量程下限)/40000) * (寄存器值 - 10000) + 量程下限
//这里的0.00052就是((量程上限 - 量程下限)/40000)
//量程下限是0，所以省略
//*10 是放大10倍保存							
							temp = (((val[Loop_i] - 10000) * 0.00052) * 10);
							if(temp < 0) temp = 0;
							else if(temp > 500) temp = 500;
							*pconc = 1000 - temp;
							
						}
						else if(2 == SysData.Config.ChannelConfig[Loop_i].attr1)//HT-N2
						{
//工程值 = ((量程上限 - 量程下限)/40000) * (寄存器值 - 10000) + 量程下限
//这里的0.0025就是((量程上限 - 量程下限)/40000)
//量程下限是0，所以省略
//*10 是放大10倍保存									
							temp = (((val[Loop_i] - 10000) * 0.0025) * 10);
							if(temp < 0) temp = 1000;
							else if(temp > 1000) temp = 1000; 

              *pconc = temp;
						}
						pconc ++;
					}				
//平均氮气值
					pconc = &monitorData.WareData[SysData.Config.addr - 1].conc1;
					temp = 0;
					for(uint8_t Loop_i = 0;Loop_i < 5;Loop_i ++)
					{
						temp = temp + *pconc;
						pconc ++;
					}
					monitorData.WareData[SysData.Config.addr - 1].conc_avg = temp/5;					
//最小氮气值
					pconc = &monitorData.WareData[SysData.Config.addr - 1].conc1;
					temp = *pconc;
					for(uint8_t Loop_i = 0;Loop_i < 5;Loop_i ++)
					{
						if(temp > *pconc)
						{
							temp = *pconc;
						}
						pconc ++;
					}
					monitorData.WareData[SysData.Config.addr - 1].conc_low = temp;
//仓内压力
//工程值 = ((量程上限 - 量程下限)/40000) * (寄存器值 - 10000) + 量程下限
//这里的0.0025就是((量程上限 - 量程下限)/40000)
//量程下限是-500
//*10 是放大10倍保存							
//					temp = (((BUSF[5] - 10000) * 0.025) - 500);
//					if(temp < -500) temp = -500;
//					else if(temp > 500) temp = 500;
//					monitorData.WareData[SysData.Config.addr - 1].wasePre = temp;
					
					temp = (((val[5] - 10000) * 0.025) - 500) * 10;
					if(temp < -5000) temp = -5000;
					else if(temp > 5000) temp = 5000;
					monitorData.WareData[SysData.Config.addr - 1].wasePre = temp;					
			
					if(2 == SysData.Config.DeviceModel)//如果设备是分管道设备
					{
//分管道压力								
//工程值 = ((量程上限 - 量程下限)/40000) * (寄存器值 - 10000) + 量程下限
//这里的0.0025就是((量程上限 - 量程下限)/40000)
//量程下限是-500
//*10 是放大10倍保存								
//						temp = (((BUSF[6] - 10000) * 0.025) - 500);
//						if(temp < -500) temp = -500;
//						else if(temp > 500) temp = 500;
//						monitorData.SlaveData[SysData.Config.loop - 1].SlavePreVal = temp;						
					}
				}
				else
				{
//主管道压力
//工程值 = ((量程上限 - 量程下限)/40000) * (寄存器值 - 10000) + 量程下限
//这里的0.0025就是((量程上限 - 量程下限)/40000)
//量程下限是0
//*1000 是放大1000倍保存，由于主管道阀精确到小数点后三位	

					temp = ((val[7] - 10000) * 0.000003125) * 1000;
					monitorData.MasterData.MasterPreVal = temp;					
				}
			}			
		}

		rt_thread_mdelay(100);
		
//操作设备
		uint8_t relay[8] = {0,0,0,0,0,0,0,0};
		
		if(3 != SysData.Config.DeviceModel)//如果不是氮气机房设备，才有进气阀，出气阀，环流风机
		{		
			relay[4] = monitorData.SelfWareData.valve1;//进气阀
			relay[5] = monitorData.SelfWareData.valve2;//出气阀
			relay[6] = monitorData.SelfWareData.valve3;//环流风机
			if(2 == SysData.Config.DeviceModel)//如果设备是分管道设备
			{
				relay[7] = monitorData.SelfSlaveData.SlaveValveSwitch;//分管道				
			} 				
		}
		else
		{
			relay[0] = monitorData.MasterData.MasterValveSwitch;//主管道			
		}
		modbus_write_bits(mb, 0x0000, 8, relay);

		modbus_close(mb);	
		
		rt_thread_mdelay(100);
		
		
//制氮机操作
		if(3 == SysData.Config.DeviceModel)//如果是氮气机房设备，才有制氮机
		{
			uint8_t data[20];
			
			if(1 == monitorData.MasterData.DeviceSwitch)
			{
				if(1 == N2_Device_Op(RT_TRUE,data))
        {
          monitorData.MasterData.DevicePreVal = data[4];
        }
			}
			else
			{
        if(1 == N2_Device_Op(RT_FALSE,data))
        {
          monitorData.MasterData.DevicePreVal = data[4];
        }
			}
		}
		rt_thread_mdelay(100);
	}
	
}

/*********************************************************************************************************
**函数名称：void Detector_Init(void)
**函数描述：探测器初始化
**输入参数：无
**返回值：  无
*********************************************************************************************************/
void Detector_Init(void)
{
	rt_thread_mdelay(1000);//延时1s，等待所有传感器电源稳定
}

/*********************************************************************************************************
**函数名称：uint8_t N2_Device_Op(rt_bool_t sw,uint8_t *p)
**函数描述：制氮机操作
**输入参数：无
**返回值：  无
*********************************************************************************************************/
int8_t N2_Device_Op(rt_bool_t sw,uint8_t *p)
{
	rt_device_t serial;
	struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 配置参数 */
	
  uint8_t sendbuf[11];
  
  sendbuf[0] = 0x5A;
  sendbuf[1] = 0x5B;
  sendbuf[2] = 0xCC;
  sendbuf[3] = 0x44;
  sendbuf[4] = 0xaa;
  sendbuf[5] = 0;
  sendbuf[6] = 0;
  sendbuf[7] = 0;
	if(RT_TRUE == sw)
	{
		sendbuf[8] = 0xEE;
	}
	else
	{
		sendbuf[8] = 0x11;
	}
  sendbuf[9] = 0x03;
  sendbuf[10] = 0x03;
	
	serial = rt_device_find("/dev/uart2");
  rt_device_open(serial,0);

	config.baud_rate = BAUD_RATE_19200;
	config.data_bits = DATA_BITS_9;
	config.stop_bits = STOP_BITS_1;
	config.parity = PARITY_ODD;
/* 打开设备后才可修改串口配置参数 */
	rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);	

//发送数据
	Detector_ReceiveNum = 0;
	rt_device_write(serial, 0, sendbuf, 11);
//等待接收数据
	do
  {
    rt_thread_mdelay(200);
  }while(0 == rt_device_read(serial, 0, &Detector_ReceiveBuffer[Detector_ReceiveNum], 1));	
	
//关闭串口	
	rt_device_close(serial);
	
//提取数据
  if(
    (0x5A == Detector_ReceiveBuffer[0])&&
    (0x5B == Detector_ReceiveBuffer[1])&&
    (0xDD == Detector_ReceiveBuffer[2])&&
    (0x03 == Detector_ReceiveBuffer[39])&&
    (0x03 == Detector_ReceiveBuffer[40])
    )
  {
		if(p != RT_NULL)
		{
			for(uint8_t Loop_i  = 0;Loop_i < 20;Loop_i ++)
			{
				p[Loop_i] = Detector_ReceiveBuffer[3 + Loop_i];
			}
		}
    
    return 1;
  }
	
  return 0;
}

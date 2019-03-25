/*********************************************************************************************************
//��������				sys_task.c
//��д�ˣ�				����
//��дʱ�䣺			2019��2��14��
//�汾�ţ�				1.0.0
//��ע��				  None
*********************************************************************************************************/
/*-----------------------------------------����ͷ�ļ�----------------------------------------------------*/
#include "board.h"

#include "sys_task.h"
#include "stdStruct.h"
#include <sys/time.h>

#include "detector_task.h"
/*------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------�������----------------------------------------------------*/
/*********************************************************************************************************
  ����������
*********************************************************************************************************/
static rt_thread_t detector_task = RT_NULL;//̽����������

static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C�����豸��� */


extern Struct_SysData SysData;//ϵͳ����
extern Struct_monitorData monitorData;//�������

/*-------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------��������----------------------------------------------------*/
/*********************************************************************************************************
**����������eeprom��ʼ������
*********************************************************************************************************/
static void eeprom_init(void);

/*********************************************************************************************************
**����������eeprom������
*********************************************************************************************************/
static rt_err_t eeprom_Read(uint8_t ReadAddr,uint8_t len,uint8_t *pBuffer);

/*********************************************************************************************************
**����������eepromд�뺯��
*********************************************************************************************************/
static rt_err_t eeprom_Write(uint8_t WriteAddr,uint8_t len,uint8_t *pBuffer);

/*********************************************************************************************************
**�����������豸��ʼ��
*********************************************************************************************************/
int sys_Init(void);

/*********************************************************************************************************
**�������ƣ�void sys_task_entry(void * parameter)
**����������ϵͳ������ں���
**�����������
**����ֵ��  ��
*********************************************************************************************************/
void sys_task_entry(void * parameter)
{
	sys_Init();//�豸��ʼ��
	
//����detector_task
	detector_task = rt_thread_create( "detector_task",
															  detector_task_entry,
																RT_NULL,
																2048,
																13,
																5
	);
	if (detector_task != RT_NULL)
	{
		rt_thread_startup(detector_task);
	}		

	while(1)
	{
		rt_thread_mdelay(100);
	}
}


/*********************************************************************************************************
**�������ƣ�void sys_Init(void)
**�����������豸��ʼ��
**�����������
**����ֵ��  ��
*********************************************************************************************************/
int sys_Init(void)
{
	uint8_t buf[30];
	
	eeprom_init();
  
	SysData.link = RT_FALSE;
entry:	
	
	eeprom_Read(0,2,buf);
	if((0x5A == buf[0])&&(0xA5 == buf[1]))//��ʼ�����豸
	{
		eeprom_Read(2,2,buf);
		SysData.Config.DeviceSeries = buf[0];
		SysData.Config.DeviceModel = buf[1];
		
		eeprom_Read(4,2,buf);
		SysData.Config.com_BaudRate = buf[0];
    SysData.Config.com_Parity = buf[1];
		
    eeprom_Read(10,30,buf);
    memcpy(SysData.Config.ServiceHost,buf,30);
    
    eeprom_Read(40,20,buf);
    memcpy(SysData.Config.IpAddr,buf,20);  

    eeprom_Read(60,20,buf);
    memcpy(SysData.Config.GWAddr,buf,20);    

    eeprom_Read(80,20,buf);
    memcpy(SysData.Config.MSKAddr,buf,20);  

		eeprom_Read(100,5,buf);		
		SysData.Config.network = buf[0];
		SysData.Config.panel = buf[1];
		SysData.Config.loop = buf[2];
		SysData.Config.addr = buf[3] * 256 + buf[4];
		
		for(uint8_t Loop_i = 0;Loop_i < 20;Loop_i ++)
		{
			eeprom_Read(105 + Loop_i * 5,5,buf);
			SysData.Config.ChannelConfig[Loop_i].ChannelSwitch = buf[0];
			SysData.Config.ChannelConfig[Loop_i].attr1 = buf[1];
			SysData.Config.ChannelConfig[Loop_i].attr2 = buf[2];
			SysData.Config.ChannelConfig[Loop_i].attr3 = buf[3];
			SysData.Config.ChannelConfig[Loop_i].attr4 = buf[4];
		}
		
		eeprom_Read(210,8,buf);
		SysData.Config.DeviceState = buf[0];
		SysData.Config.Deviceprocess = buf[1];
		SysData.Config.DeviceSwitch = buf[2];
		SysData.Config.MasterValveSwitch = buf[3];
		SysData.Config.SlaveValveSwitch = buf[4];
		SysData.Config.valve1 = buf[5];
		SysData.Config.valve2 = buf[6];
		SysData.Config.valve3 = buf[7];
	}
	else
	{
		buf[0] = 0x5A;
		buf[1] = 0xA5;
		eeprom_Write(0,2,buf);
		
		buf[0] = 1;
		buf[1] = 1;
		eeprom_Write(2,2,buf);		
		
		buf[0] = 1;
		buf[1] = 1;
		buf[2] = 1;
		buf[3] = 0;		
		buf[4] = 1;
		eeprom_Write(100,5,buf);

		for(uint8_t Loop_i = 0;Loop_i < 5;Loop_i ++)
		{
			buf[0] = 1;
			buf[1] = 1;
			buf[2] = 0;
			buf[3] = 0;		
			buf[4] = 0;
			eeprom_Write(105 + Loop_i * 5,5,buf);
		}	
		
		goto entry;
	}
	
	if(3 != SysData.Config.DeviceModel)
	{
		monitorData.WareData[SysData.Config.addr - 1].valve1 = SysData.Config.valve1;
		monitorData.WareData[SysData.Config.addr - 1].valve2 = SysData.Config.valve2;
		monitorData.WareData[SysData.Config.addr - 1].valve3 = SysData.Config.valve3;
	}
	else
	{
	}
	SysData.sign = RT_TRUE;
	
	
  if(3 == SysData.Config.DeviceModel)
	{
		monitorData.first = 0;
		monitorData.end = 0;
		monitorData.busy = RT_FALSE;
		for(uint8_t Loop_i = 0;Loop_i < _MAX_DEVICE_;Loop_i ++)
		{
			monitorData.DeviceState[Loop_i] = -1;
		}
		monitorData.MasterData.panel = SysData.Config.panel;
		monitorData.MasterData.DevicePreVal = 0;
		monitorData.MasterData.MasterPreVal = 0;
		monitorData.MasterData.DeviceSwitch = 0;
		monitorData.MasterData.MasterValveSwitch = 0;		
	}
	else
	{
		monitorData.SelfWareData.conc1 = 790;
		monitorData.SelfWareData.conc2 = 790;
		monitorData.SelfWareData.conc3 = 790;
		monitorData.SelfWareData.conc4 = 790;
		monitorData.SelfWareData.conc5 = 790;

		monitorData.SelfWareData.conc_low = 790;
		monitorData.SelfWareData.conc_avg = 790;		

		monitorData.SelfWareData.wasePre = 0;

		monitorData.SelfWareData.valve1 = 0;
		monitorData.SelfWareData.valve2 = 0;
		monitorData.SelfWareData.valve3 = 0;	
		
		if(2 == SysData.Config.DeviceModel)
		{
			monitorData.SelfSlaveData.SlaveValveSwitch = 0;
		}
	}
	
	return 0;
}

/*********************************************************************************************************
**�������ƣ�static void eeprom_init(void)
**����������eeprom��ʼ������
**�����������
**����ֵ��  ��
*********************************************************************************************************/
static void eeprom_init(void)
{
	/* ����I2C�����豸����ȡI2C�����豸��� */
	i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c1");

	if (i2c_bus == RT_NULL)
	{
			rt_kprintf("can't find %s device!\n", "i2c1");
	}
}

/*********************************************************************************************************
**�������ƣ�static void eeprom_Read(uint16_t ReadAddr,uint8_t *pBuffer)
**����������eeprom�����ֽں���
**���������
**ReadAddr :�����ĵ�ַ ��24c02Ϊ0~255
**len :��ȡ����
**pBuffer  :���ݽ��յ�ַ
**����ֵ��  ��
*********************************************************************************************************/
static rt_err_t eeprom_Read(uint8_t ReadAddr,uint8_t len,uint8_t *pBuffer)
{
	struct rt_i2c_msg msgs[2];
	uint8_t addr = ReadAddr;

	msgs[0].addr  = EEPROM_ADDR;    		/* �ӻ���ַ */
	msgs[0].flags = RT_I2C_WR;       	/* д��־ */	
	msgs[0].buf  = &addr; 						/* �ӻ��Ĵ�����ַ*/
	msgs[0].len   = 1 ;   						/* ���������ֽ��� */	
	
	
	msgs[1].addr  = EEPROM_ADDR;    /* �ӻ���ַ */
	msgs[1].flags = RT_I2C_RD;       /* ����־ */
	msgs[1].buf   = pBuffer;         /* ��ȡ����ָ�� */
	msgs[1].len   = len;             /* ��ȡ�����ֽ��� */

	/* ����I2C�豸�ӿڴ������� */
	if (rt_i2c_transfer(i2c_bus, msgs, 2) == 1)
	{
		rt_thread_mdelay(10);
		return RT_EOK;
	}
	else
	{
		return -RT_ERROR;
	}	
} 

/*********************************************************************************************************
**�������ƣ�static void eeprom_Write(uint16_t WriteAddr,uint8_t Buffer)
**����������eepromд�뵥�ֽں���
**���������
**WriteAddr :д��ĵ�ַ ��24c02Ϊ0~255
**len :д�볤��
**Buffer  :д�������
**����ֵ��  ��
*********************************************************************************************************/
static rt_err_t eeprom_Write(uint8_t WriteAddr,uint8_t len,uint8_t *pBuffer)
{
	struct rt_i2c_msg msgs;
	uint8_t *p = pBuffer;
	uint8_t buf[9];
	
	uint8_t Loop_i = 0;
	
	buf[0] = WriteAddr;
	while(len --)
	{
		buf[Loop_i + 1] = *p;
		Loop_i ++;
		p ++;
		if(((Loop_i + WriteAddr)%8) == 0)
		{
			msgs.addr  = EEPROM_ADDR;    		/* �ӻ���ַ */
			msgs.flags = RT_I2C_WR;       	 	/* д��־ */	
			msgs.buf  = buf; 							/* �ӻ��Ĵ�����ַ*/
			msgs.len   = Loop_i + 1;   	/* ���������ֽ��� */						
			/* ����I2C�豸�ӿڴ������� */
			if (rt_i2c_transfer(i2c_bus, &msgs, 1) == 1)
			{
				rt_thread_mdelay(10);
			}
			WriteAddr = WriteAddr + Loop_i;
			buf[0] = WriteAddr;
			Loop_i = 0;
		}
	}
	msgs.addr  = EEPROM_ADDR;    		/* �ӻ���ַ */
	msgs.flags = RT_I2C_WR;       	 	/* д��־ */	
	msgs.buf  = buf; 							/* �ӻ��Ĵ�����ַ*/
	msgs.len   = Loop_i + 1;   	/* ���������ֽ��� */		
	/* ����I2C�豸�ӿڴ������� */
	if (rt_i2c_transfer(i2c_bus, &msgs, 1) == 1)
	{
		rt_thread_mdelay(10);
	}
	
	return RT_EOK;
}
	
//���������
static void network(int argc, char**argv)
{
	if (argc == 1)
	{
		rt_kprintf("network:%d\n", SysData.Config.network);
	}
	else if (argc > 1)
	{
		uint8_t network = atoi(argv[1]);
		if (network > 0 || network < 100)
		{
			eeprom_Write(100,1,&network);
			
			eeprom_Read(100,1,&network);
			SysData.Config.network = network;
		}		
	}
}
MSH_CMD_EXPORT(network, get network or set [network]);
	
//����������
static void panel(int argc, char**argv)
{
	if (argc == 1)
	{
		rt_kprintf("panel:%d\n", SysData.Config.panel);
	}
	else if (argc > 1)
	{
		uint8_t panel = atoi(argv[1]);
		if (panel > 0 || panel < 100)
		{
			eeprom_Write(101,1,&panel);
			
			eeprom_Read(101,1,&panel);
			SysData.Config.panel = panel;
		}		
	}
}
MSH_CMD_EXPORT(panel, get panel or set [panel]);

//���û�·��
static void loop(int argc, char**argv)
{
	if (argc == 1)
	{
		rt_kprintf("loop:%d\n", SysData.Config.loop);
	}
	else if (argc > 1)
	{
		uint8_t loop = atoi(argv[1]);
		if (loop > 0 || loop < 100)
		{
			eeprom_Write(102,1,&loop);
			
			eeprom_Read(102,1,&loop);
			SysData.Config.loop = loop;
		}		
	}
}
MSH_CMD_EXPORT(loop, get loop or set [loop]);

//���õ�ַ��
static void addr(int argc, char**argv)
{
	if (argc == 1)
	{
		rt_kprintf("addr:%d\n", SysData.Config.addr);
	}
	else if (argc > 1)
	{
		uint16_t addr = atoi(argv[1]);
		if (addr > 0 || addr < 100)
		{
			uint8_t buf[2];
			
			buf[0] = addr/256%256;
			buf[1] = addr/1%256;
			eeprom_Write(103,2,buf);
			
			eeprom_Read(103,2,buf);
			SysData.Config.addr = buf[0] * 256 + buf[1];
		}		
	}
}
MSH_CMD_EXPORT(addr, get addr or set [addr]);


//����ϵ��
static void DeviceSeries(int argc, char**argv)
{
	if (argc == 1)
	{
		rt_kprintf("DeviceSeries:%d\n", SysData.Config.DeviceSeries);
	}
	else if (argc > 1)
	{
		uint8_t DeviceSeries = atoi(argv[1]);
		if (DeviceSeries > 0 || DeviceSeries < 100)
		{
			eeprom_Write(2,1,&DeviceSeries);
			
			eeprom_Read(2,1,&DeviceSeries);
			SysData.Config.DeviceSeries = DeviceSeries;
		}		
	}
}
MSH_CMD_EXPORT(DeviceSeries, get addr or set [DeviceSeries]);

//�����ͺ�
static void DeviceModel(int argc, char**argv)
{
	if (argc == 1)
	{
		rt_kprintf("DeviceModel:%d\n", SysData.Config.DeviceModel);
	}
	else if (argc > 1)
	{
		uint8_t DeviceModel = atoi(argv[1]);
		if (DeviceModel > 0 || DeviceModel < 100)
		{
			eeprom_Write(3,1,&DeviceModel);
			
			eeprom_Read(3,1,&DeviceModel);
			SysData.Config.DeviceModel = DeviceModel;
		}		
	}
}
MSH_CMD_EXPORT(DeviceModel, get addr or set [DeviceModel]);

//����com��BaudRate����
static void ComBaudRate(int argc, char**argv)
{
	if (argc == 1)
	{
		if(1 == SysData.Config.com_BaudRate)
		{
			rt_kprintf("ComBaudRate:1200\n");
		}
		else if(2 == SysData.Config.com_BaudRate)
		{
			rt_kprintf("ComBaudRate:2400\n");
		}
		else if(3 == SysData.Config.com_BaudRate)
		{
			rt_kprintf("ComBaudRate:4800\n");
		}	
		else if(4 == SysData.Config.com_BaudRate)
		{
			rt_kprintf("ComBaudRate:9600\n");
		}	
		else if(5 == SysData.Config.com_BaudRate)
		{
			rt_kprintf("ComBaudRate:19200\n");
		}	
		else if(6 == SysData.Config.com_BaudRate)
		{
			rt_kprintf("ComBaudRate:115200\n");
		}			
	}
	else if (argc > 1)
	{
		uint8_t com_BaudRate = atoi(argv[1]);
		if (com_BaudRate > 0 || com_BaudRate < 7)
		{
			eeprom_Write(4,1,&com_BaudRate);
			
			eeprom_Read(4,1,&com_BaudRate);
			SysData.Config.com_BaudRate = com_BaudRate;
		}		
	}
}
MSH_CMD_EXPORT(ComBaudRate, get addr or set [ComBaudRate]);

//����com��Parity����
static void ComParity(int argc, char**argv)
{
	if (argc == 1)
	{
		if(0 == SysData.Config.com_Parity)
		{
			rt_kprintf("ComParity:None\n");
		}
		else if(1 == SysData.Config.com_Parity)
		{
			rt_kprintf("ComParity:ODD\n");
		}
		else if(2 == SysData.Config.com_Parity)
		{
			rt_kprintf("ComParity:EVEN\n");
		}	
	}
	else if (argc > 1)
	{
		uint8_t com_Parity = atoi(argv[1]);
		if (com_Parity >= 0 || com_Parity < 3)
		{
			eeprom_Write(5,1,&com_Parity);
			
			eeprom_Read(5,1,&com_Parity);
			SysData.Config.com_Parity = com_Parity;
		}		
	}
}
MSH_CMD_EXPORT(ComParity, get addr or set [ComParity]);


//���ô������ͺ�
static void sermodel(int argc, char**argv)
{
	if (argc == 2)
	{
		uint8_t channel = atoi(argv[1]);
		rt_kprintf("sermodel:%d\n", SysData.Config.ChannelConfig[channel - 1].attr1);
	}
	else if (argc > 2)
	{
		uint8_t channel = atoi(argv[1]);
		uint8_t model = atoi(argv[2]);
		if (channel > 1 || channel < 6)
		{
			eeprom_Write(106 + (channel - 1) * 5,1,&model);
			
			eeprom_Read(106 + (channel - 1) * 5,1,&model);
			SysData.Config.ChannelConfig[channel - 1].attr1 = model;
		}		
	}
	else
	{
		rt_kprintf("please input: sermodel [channel] or sermodel[sermodel model]\n");
	}
}
MSH_CMD_EXPORT(sermodel, get addr or set [channel model]);

//����Service
static void Service(int argc, char**argv)
{
  uint8_t buf[30] = {0};
  
	if (argc == 1)
	{
		rt_kprintf("ServiceHost:%s\n", &SysData.Config.ServiceHost[1]);
	}
	else if (argc > 1)
	{
    memcpy(&buf[1],argv[1],30);
    buf[0] = 0x5A;
    eeprom_Write(10,30,buf);

    eeprom_Read(10,30,buf);
    memcpy(SysData.Config.ServiceHost,buf,30);
	}
}
MSH_CMD_EXPORT(Service, get addr or set [Service Host]);

//����Ip��ַ
static void IpAddr(int argc, char**argv)
{
  uint8_t buf[20];
  
	if (argc == 1)
	{
		rt_kprintf("IpAddr:%s\n", &SysData.Config.IpAddr[1]);
	}
	else if (argc > 1)
	{
    memcpy(&buf[1],argv[1],20);
    buf[0] = 0x5A;
    eeprom_Write(40,20,buf);

    eeprom_Read(40,20,buf);
    memcpy(SysData.Config.IpAddr,buf,20);  
	}
}
MSH_CMD_EXPORT(IpAddr, get Ip or set [IpAddr]);

//�������ص�ַ
static void GWAddr(int argc, char**argv)
{
  uint8_t buf[20];
  
	if (argc == 1)
	{
		rt_kprintf("GwAddr:%s\n", &SysData.Config.GWAddr[1]);
	}
	else if (argc > 1)
	{
    memcpy(&buf[1],argv[1],20);
    buf[0] = 0x5A;
    eeprom_Write(60,20,buf);

    eeprom_Read(60,20,buf);
    memcpy(SysData.Config.GWAddr,buf,20);  
	}
}
MSH_CMD_EXPORT(GWAddr, get Ip or set [GWAddr]);

//�������������ַ
static void MSKAddr(int argc, char**argv)
{
  uint8_t buf[20];
  
	if (argc == 1)
	{
		rt_kprintf("MskAddr:%s\n", &SysData.Config.MSKAddr[1]);
	}
	else if (argc > 1)
	{
    memcpy(&buf[1],argv[1],20);
    buf[0] = 0x5A;
    eeprom_Write(80,20,buf);

    eeprom_Read(80,20,buf);
    memcpy(SysData.Config.MSKAddr,buf,20);  
	}
}
MSH_CMD_EXPORT(MSKAddr, get Ip or set [MSKAddr]);



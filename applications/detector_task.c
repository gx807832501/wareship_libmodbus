/*********************************************************************************************************
//��������				detector_task.c
//��д�ˣ�				����
//��дʱ�䣺			2019��2��26��
//�汾�ţ�				1.0.0
//��ע��				  None
*********************************************************************************************************/
/*-----------------------------------------����ͷ�ļ�----------------------------------------------------*/
#include "detector_task.h"
#include "stdStruct.h"

#include "modbus.h"
/*------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------�������----------------------------------------------------*/
modbus_t *mb;//modbus���

extern Struct_SysData SysData;//ϵͳ����
extern Struct_monitorData monitorData;//�������

rt_uint8_t Detector_ReceiveNum;//̽�������ݽ��ռ�����
rt_uint8_t Detector_ReceiveBuffer[50];//̽�������ݽ��ջ�����


/*-------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------��������----------------------------------------------------*/
/*********************************************************************************************************
**�����������豸��ʼ��
*********************************************************************************************************/
void Detector_Init(void);

/*********************************************************************************************************
**�����������Ƶ�������
*********************************************************************************************************/
int8_t N2_Device_Op(rt_bool_t sw,uint8_t *p);

/*------------------------------------------------------------------------------------------------------*/

/*********************************************************************************************************
void detector_task_entry(void * parameter)
**����������������������ں���
**�����������
**����ֵ��  ��
*********************************************************************************************************/
void detector_task_entry(void * parameter)
{
	modbus_t *mb;//���
	int temp;//��ʱ����
	
	Detector_Init();//̽������ʼ��
	
	mb = modbus_new_rtu("/dev/uart2", 9600, 'N', 8, 1);
	while(1)
	{
		modbus_set_slave(mb,1);
		modbus_connect(mb);
		modbus_set_response_timeout(mb,0,1000000);	
		
//��ȡ����
		uint16_t val[8] = {0,0,0,0,0,0,0,0};
		
		if(-1 != modbus_read_input_registers(mb, 0x00, 8, val))
		{
//����ֵ
			if(RT_TRUE == monitorData.updatestate)//�����ظ�������
			{
				if(3 != SysData.Config.DeviceModel)//������ǵ��������豸�����е���������
				{
					int16_t *pconc = &monitorData.WareData[SysData.Config.addr - 1].conc1;
          
					
//5������������					
					for(uint8_t Loop_i = 0;Loop_i < 5;Loop_i ++)
					{
						if(1 == SysData.Config.ChannelConfig[Loop_i].attr1)//KY-2N
						{
//����ֵ = ((�������� - ��������)/40000) * (�Ĵ���ֵ - 10000) + ��������
//�����0.00052����((�������� - ��������)/40000)
//����������0������ʡ��
//*10 �ǷŴ�10������							
							temp = (((val[Loop_i] - 10000) * 0.00052) * 10);
							if(temp < 0) temp = 0;
							else if(temp > 500) temp = 500;
							*pconc = 1000 - temp;
							
						}
						else if(2 == SysData.Config.ChannelConfig[Loop_i].attr1)//HT-N2
						{
//����ֵ = ((�������� - ��������)/40000) * (�Ĵ���ֵ - 10000) + ��������
//�����0.0025����((�������� - ��������)/40000)
//����������0������ʡ��
//*10 �ǷŴ�10������									
							temp = (((val[Loop_i] - 10000) * 0.0025) * 10);
							if(temp < 0) temp = 1000;
							else if(temp > 1000) temp = 1000; 

              *pconc = temp;
						}
						pconc ++;
					}				
//ƽ������ֵ
					pconc = &monitorData.WareData[SysData.Config.addr - 1].conc1;
					temp = 0;
					for(uint8_t Loop_i = 0;Loop_i < 5;Loop_i ++)
					{
						temp = temp + *pconc;
						pconc ++;
					}
					monitorData.WareData[SysData.Config.addr - 1].conc_avg = temp/5;					
//��С����ֵ
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
//����ѹ��
//����ֵ = ((�������� - ��������)/40000) * (�Ĵ���ֵ - 10000) + ��������
//�����0.0025����((�������� - ��������)/40000)
//����������-500
//*10 �ǷŴ�10������							
//					temp = (((BUSF[5] - 10000) * 0.025) - 500);
//					if(temp < -500) temp = -500;
//					else if(temp > 500) temp = 500;
//					monitorData.WareData[SysData.Config.addr - 1].wasePre = temp;
					
					temp = (((val[5] - 10000) * 0.025) - 500) * 10;
					if(temp < -5000) temp = -5000;
					else if(temp > 5000) temp = 5000;
					monitorData.WareData[SysData.Config.addr - 1].wasePre = temp;					
			
					if(2 == SysData.Config.DeviceModel)//����豸�Ƿֹܵ��豸
					{
//�ֹܵ�ѹ��								
//����ֵ = ((�������� - ��������)/40000) * (�Ĵ���ֵ - 10000) + ��������
//�����0.0025����((�������� - ��������)/40000)
//����������-500
//*10 �ǷŴ�10������								
//						temp = (((BUSF[6] - 10000) * 0.025) - 500);
//						if(temp < -500) temp = -500;
//						else if(temp > 500) temp = 500;
//						monitorData.SlaveData[SysData.Config.loop - 1].SlavePreVal = temp;						
					}
				}
				else
				{
//���ܵ�ѹ��
//����ֵ = ((�������� - ��������)/40000) * (�Ĵ���ֵ - 10000) + ��������
//�����0.0025����((�������� - ��������)/40000)
//����������0
//*1000 �ǷŴ�1000�����棬�������ܵ�����ȷ��С�������λ	

					temp = ((val[7] - 10000) * 0.000003125) * 1000;
					monitorData.MasterData.MasterPreVal = temp;					
				}
			}			
		}

		rt_thread_mdelay(100);
		
//�����豸
		uint8_t relay[8] = {0,0,0,0,0,0,0,0};
		
		if(3 != SysData.Config.DeviceModel)//������ǵ��������豸�����н����������������������
		{		
			relay[4] = monitorData.SelfWareData.valve1;//������
			relay[5] = monitorData.SelfWareData.valve2;//������
			relay[6] = monitorData.SelfWareData.valve3;//�������
			if(2 == SysData.Config.DeviceModel)//����豸�Ƿֹܵ��豸
			{
				relay[7] = monitorData.SelfSlaveData.SlaveValveSwitch;//�ֹܵ�				
			} 				
		}
		else
		{
			relay[0] = monitorData.MasterData.MasterValveSwitch;//���ܵ�			
		}
		modbus_write_bits(mb, 0x0000, 8, relay);

		modbus_close(mb);	
		
		rt_thread_mdelay(100);
		
		
//�Ƶ�������
		if(3 == SysData.Config.DeviceModel)//����ǵ��������豸�������Ƶ���
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
**�������ƣ�void Detector_Init(void)
**����������̽������ʼ��
**�����������
**����ֵ��  ��
*********************************************************************************************************/
void Detector_Init(void)
{
	rt_thread_mdelay(1000);//��ʱ1s���ȴ����д�������Դ�ȶ�
}

/*********************************************************************************************************
**�������ƣ�uint8_t N2_Device_Op(rt_bool_t sw,uint8_t *p)
**�����������Ƶ�������
**�����������
**����ֵ��  ��
*********************************************************************************************************/
int8_t N2_Device_Op(rt_bool_t sw,uint8_t *p)
{
	rt_device_t serial;
	struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* ���ò��� */
	
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
/* ���豸��ſ��޸Ĵ������ò��� */
	rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);	

//��������
	Detector_ReceiveNum = 0;
	rt_device_write(serial, 0, sendbuf, 11);
//�ȴ���������
	do
  {
    rt_thread_mdelay(200);
  }while(0 == rt_device_read(serial, 0, &Detector_ReceiveBuffer[Detector_ReceiveNum], 1));	
	
//�رմ���	
	rt_device_close(serial);
	
//��ȡ����
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

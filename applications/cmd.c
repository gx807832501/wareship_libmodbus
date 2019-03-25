/*********************************************************************************************************
//��������				cmd.c
//��д�ˣ�				����
//��дʱ�䣺			2019��3��25��
//�汾�ţ�				1.0.0
//��ע��				  None
*********************************************************************************************************/
/*-----------------------------------------����ͷ�ļ�----------------------------------------------------*/
#include "board.h"

#include "sys_task.h"
#include "stdStruct.h"
/*------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------�������----------------------------------------------------*/
/*********************************************************************************************************
  ����������
*********************************************************************************************************/
extern Struct_monitorData monitorData;//�������

//��ȡ�豸ID
void GetDeviceSN(int argc, char**argv)
{
	rt_int32_t temp;
	
	temp = *(__IO uint32_t*)(0x1FFFF7E8);//��ȡSTM32��ΨһID
	rt_kprintf("SN:%x\n", temp);
}
MSH_CMD_EXPORT(GetDeviceSN, Get the Device SN);

//���ý�����״̬
static void IntakeValve(int argc, char**argv)
{
	if (argc == 1)
	{
		if(1 == monitorData.SelfWareData.valve1)
		{
			rt_kprintf("Intake Valve is Open!\n");
		}
		else
		{
			rt_kprintf("Intake Valve is Close!\n");
		}
		
	}
	else if (argc > 1)
	{
		uint8_t state = atoi(argv[1]);
		if ((0 == state)||(1 == state))
		{
			monitorData.SelfWareData.valve1 = state;
		}		
	}
}
MSH_CMD_EXPORT(IntakeValve, get IntakeValve state or set IntakeValve state[0 or 1]);

//���ó�����״̬
static void OuttakeValve(int argc, char**argv)
{
	if (argc == 1)
	{
		if(1 == monitorData.SelfWareData.valve2)
		{
			rt_kprintf("Outtake Valve is Open!\n");
		}
		else
		{
			rt_kprintf("Outtake Valve is Close!\n");
		}
		
	}
	else if (argc > 1)
	{
		uint8_t state = atoi(argv[1]);
		if ((0 == state)||(1 == state))
		{
			monitorData.SelfWareData.valve2 = state;
		}		
	}
}
MSH_CMD_EXPORT(OuttakeValve, get OuttakeValve state or set OuttakeValve state[0 or 1]);

//���ó������״̬
static void OutFan(int argc, char**argv)
{
	if (argc == 1)
	{
		if(1 == monitorData.SelfWareData.valve3)
		{
			rt_kprintf("OutFan is Open!\n");
		}
		else
		{
			rt_kprintf("OutFan is Close!\n");
		}
		
	}
	else if (argc > 1)
	{
		uint8_t state = atoi(argv[1]);
		if ((0 == state)||(1 == state))
		{
			monitorData.SelfWareData.valve3 = state;
		}		
	}
}
MSH_CMD_EXPORT(OutFan, get OutFan state or set OutFan state[0 or 1]);

//���÷ֹܵ�״̬
static void SlaveDeviceValue(int argc, char**argv)
{
	if (argc == 1)
	{
		if(1 == monitorData.SelfSlaveData.SlaveValveSwitch)
		{
			rt_kprintf("Slave Device Valve is Open!\n");
		}
		else
		{
			rt_kprintf("Slave Device Valve is Close!\n");
		}
		
	}
	else if (argc > 1)
	{
		uint8_t state = atoi(argv[1]);
		if ((0 == state)||(1 == state))
		{
			monitorData.SelfSlaveData.SlaveValveSwitch = state;
		}		
	}
}
MSH_CMD_EXPORT(SlaveDeviceValue, get SlaveDeviceValue state or set SlaveDeviceValue state[0 or 1]);

 

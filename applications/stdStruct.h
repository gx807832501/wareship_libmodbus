#ifndef __stdStruct_H_
#define __stdStruct_H_


/*-----------------------------------------����ͷ�ļ�----------------------------------------------------*/
#include <rtthread.h>
#include "board.h"
#include "drv_gpio.h"
/*------------------------------------------------------------------------------------------------------*/
#define Device_VERSION                      1L
#define Device_SUBVERSION                   4L
#define Device_REVISION                     0L
#define Device_VERSIONMODE									"A"
#define Device_BETA                     		1L


#define _MAX_DEVICE_						(50)
#define _MAX_SlaveDevice_				(10)
#define _MAX_MasterDevice_			(1)


#define LED0_PIN    GET_PIN(B, 1)
#define LED1_PIN    GET_PIN(C, 5)
#define LED2_PIN    GET_PIN(C, 3)
#define LED3_PIN    GET_PIN(C, 1)
#define LED4_PIN    GET_PIN(B, 0)
#define LED5_PIN    GET_PIN(C, 4)
#define LED6_PIN    GET_PIN(C, 2)
#define LED7_PIN    GET_PIN(C, 0)


//ϵͳ���ýṹ��
typedef struct
{
  uint8_t DeviceSeries;//�豸ϵ��   ȱʡֵΪ0
  uint8_t DeviceModel;//�豸�ͺ�    1��������   2���ֹܵ���   3���豸������
	
	uint8_t com_BaudRate;//������
	uint8_t com_Parity;//У��λ
	
  uint8_t ServiceHost[30];//Service��ַ ��һ�ֽڱ�ʾ�Ƿ���Ч 0x5A��ʾ��Ч
  uint8_t IpAddr[20];//Ip��ַ ��һ�ֽڱ�ʾ�Ƿ���Ч 0x5A��ʾ��Ч
  uint8_t GWAddr[20];//���ص�ַ ��һ�ֽڱ�ʾ�Ƿ���Ч 0x5A��ʾ��Ч
  uint8_t MSKAddr[20];//�������� ��һ�ֽڱ�ʾ�Ƿ���Ч 0x5A��ʾ��Ч

  uint8_t network;//�����
	uint8_t panel;//����豸��
	uint8_t loop;//��·��
  uint16_t addr;//�豸��ַ
	
//1������1  2������2  3������3  4������4  5������5  
//6������ѹ��  7���ֹܵ�ѹ�� 8�����ܵ�ѹ�� 9���Ƶ���ѹ�� 
//10�����ܵ���  11���ֹܵ���  12�����ڽ�����  13�����ڳ�����  14��������� 15���Ƶ�������
  struct
  {
    uint8_t ChannelSwitch;//ͨ������   0���ر�  1����
    uint8_t attr1;//�������ͺ� 1:KY-2N 2:HT-N2
    uint8_t attr2;//��������λ
    uint8_t attr3;//���������
    uint8_t attr4;
  }ChannelConfig[20];  

	uint8_t timeAuto;//ʱ���Զ��豸��־
	
	uint8_t DeviceState;//�豸״̬
	uint8_t Deviceprocess;//�豸����
	rt_bool_t DeviceSwitch;//�豸����
	rt_bool_t MasterValveSwitch;//���ܵ���ŷ�����
  rt_bool_t SlaveValveSwitch;//�ֹܵ���ŷ�����
	rt_bool_t valve1;//������ŷ�״̬
	rt_bool_t valve2;//������ŷ�״̬
	rt_bool_t valve3;//�������״̬	
  
  int32_t FirstRecordNum;//��һ����¼���
  int32_t LastRecordNum;//���һ����¼���
  int32_t TotalNum;//���� 
}Struct_Config;


typedef struct
{
  uint8_t static_val;//��̬ͼ��״ֵ̬
  uint8_t dynamic_val;//��̬ͼ��״ֵ̬

  uint8_t (*pFun)(void);//ҳ�ص�����
}Struct_PageData;

//ϵͳ���ݽṹ��
typedef struct
{
  Struct_Config Config;//��������
	rt_bool_t sign;//��ʼ����ɱ�־
  rt_bool_t link;
	rt_bool_t Flag_PageDatachange;//ҳˢ�¸ı��־
  Struct_PageData currentPageData;//��ǰҳ����	
}Struct_SysData;

typedef struct
{
	int8_t addr;//������ַ  
	int8_t type;//�������� 1�������Լ�� 2���䵪����  3����ŷ����Ƶ�������  4��ֹͣ����
	
//��ŷ�����
	int8_t No;//��� 1�������� 2�������� 3��������� 4���ֹܵ���  5�����ܵ���  6���Ƶ���
	int8_t val;//��ŷ�ֵ  1���� 0���ر�

//�䵪����	
	int16_t targetmode;//Ŀ�귽ʽ
	int16_t targetconc;//Ŀ�굪��Ũ��	
	int16_t targetminpre;//��Сѹ��ֵ
	int16_t targetmaxpre;//���ѹ��ֵ
	
//�����Լ�����
	int16_t mode;//���λ�� 1�����ܵ� 2���ֹܵ� 3���ַ�
	int16_t adv_pre;//Ԥ����ѹֵ
	int16_t start_pre;//��ʼ��ѹֵ
	int16_t end_pre;//������ѹֵ
	int16_t min_time;//��Сʱ��

//ֹͣ�������
	uint8_t cancelcode[21];//�������
	
	
	uint8_t taskcode[21];//�������
}Struct_Op;


typedef struct
{
	uint16_t panel;//���ܵ����
	
	rt_bool_t DeviceSwitch;//�豸����
	rt_bool_t MasterValveSwitch;//���ܵ���ŷ�����
	int16_t DevicePreVal;//�豸ѹ��ֵ������ x10 ���棩
	int16_t MasterPreVal;//���ܵ�ѹ��ֵ������ x10 ���棩
}Struct_MasterData;

typedef struct
{
	uint16_t panel;//���ܵ����
	uint16_t loop;//�ֹܵ����
	
	rt_bool_t SlaveValveSwitch;//�ֹܵ���ŷ�����
}Struct_SlaveData;

typedef struct
{
	uint16_t panel;//���ܵ����
	uint16_t loop;//�ֹܵ����
	uint16_t addr;//�豸���
	
	int16_t conc1;//����1Ũ��
	int16_t conc2;//����2Ũ��
	int16_t conc3;//����3Ũ��
	int16_t conc4;//����4Ũ��
	int16_t conc5;//����5Ũ��
	int16_t conc_avg;//ƽ��ֵ
	int16_t conc_low;//��Сֵ
	int16_t wasePre;//������ѹֵ
	uint8_t valve1;//������ŷ�״̬
	uint8_t valve2;//������ŷ�״̬
	uint8_t valve3;//�������״̬
	
	uint8_t conc1attr1;//�������ͺ� 1:KY-2N 2:HT-N2
	uint8_t conc2attr1;
	uint8_t conc3attr1;
	uint8_t conc4attr1;
	uint8_t conc5attr1;
	
	uint8_t conc1attr2;//��������λ 0:�м� 1���� 2������ 3���� 4������ 5���� 6������ 7���� 8������
	uint8_t conc2attr2;
	uint8_t conc3attr2;
	uint8_t conc4attr2;
	uint8_t conc5attr2;
	
	uint8_t conc1attr3;//��������� 0:���� 5��0.5m 10��1m 20��2m �ȵ�
	uint8_t conc2attr3;
	uint8_t conc3attr3;
	uint8_t conc4attr3;
	uint8_t conc5attr3;
}Struct_WareData;

//̽���������Ϣ
typedef struct
{
	rt_bool_t updatestate;//��������״̬  1�����ظ��� 0�����ز�����
	rt_bool_t online;//�豸����״̬
	
//����������ֻ�е������ǿ����豸ʱ��������	
	int8_t DeviceState[_MAX_DEVICE_];//�豸״̬ -1���豸���� 0��׼���� 1����ҵ��
	rt_bool_t busy;//æ��־
	int8_t first,end;
	Struct_Op op[5];//���֧��ͬʱ5������	
	
	Struct_SlaveData SelfSlaveData;//�����ֹܵ�ֵ
	Struct_WareData SelfWareData;//�����ַ�ֵ
	
	Struct_MasterData MasterData;
	Struct_SlaveData SlaveData[_MAX_SlaveDevice_];
	Struct_WareData WareData[_MAX_DEVICE_];
}Struct_monitorData;


//mqtt����
typedef struct
{
	uint8_t cmdtype;//��������  1����λ
	int8_t cmdaddr;//�����ַ	
	int16_t Param1;//�������1
	int16_t Param2;//�������2
}Struct_cmd;



//����ִ��ȷ��
typedef struct 
{
	Struct_Op task;	
//�����Ƿ�֧��		
	rt_bool_t Execution;
}mq_type1;

//����ִ���м����ݣ�ֻ������������ͳ䵪�������м����ݣ�
typedef struct 
{
	Struct_Op task;
	
//�������м�����
	int16_t count;//��ʱ��

//�䵪�м�����	
	Struct_MasterData MasterData;
	Struct_SlaveData SlaveData;
	Struct_WareData WareData;
}mq_type2;


//����ִ�н������
typedef struct 
{
	Struct_Op task;
//����ִ�����ս��	
	rt_bool_t state;
//����������
	int8_t errorcode;

//�����Լ�����
	int16_t count;//��ʱ��
	
//�䵪����	
	Struct_MasterData MasterData;
	Struct_SlaveData SlaveData;
	Struct_WareData WareData;
}mq_type3;


typedef struct
{
//1�����ܵ�����
//2���ֹܵ�����	
//3���ַ�����
//4��������Ϣ	
//5������ִ��ȷ��
//6������ִ���м����ݣ�ֻ������������ͳ䵪�������м����ݣ�
//7������ִ�н������
//8������	
	uint8_t type;
	union
	{
		Struct_MasterData data1;
		Struct_SlaveData data2;
		Struct_WareData data3;
		Struct_cmd cmd;
		Struct_Op data4;
		mq_type1 data5;
		mq_type2 data6;
		mq_type3 data7;
	}data;
	struct tm time;//ʱ��
	time_t unix_time;//ʱ���	
}mq_data;


#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/

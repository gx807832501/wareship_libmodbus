#ifndef __stdStruct_H_
#define __stdStruct_H_


/*-----------------------------------------����ͷ�ļ�----------------------------------------------------*/
#include <rtthread.h>
/*------------------------------------------------------------------------------------------------------*/

#define _MAX_WASE_						(50)
#define _MAX_SlaveDevice_			(10)
#define _MAX_MasterDevice_		(1)

//ϵͳ���ýṹ��
typedef struct
{
  uint8_t DeviceSeries;//�豸ϵ��   ȱʡֵΪ0
  uint8_t DeviceModel;//�豸�ͺ�    1��������   2���ֹܵ���   3���豸������

  uint8_t network;//�����
	uint8_t panel;//����豸��
	uint8_t loop;//��·��
  uint16_t addr;//�豸��ַ
	
//1������1  2������2  3������3  4������4  5������5  6������ѹ��  7���ֹܵ�ѹ�� 8�����ܵ�ѹ�� 9���Ƶ���ѹ�� 10�����ܵ���
//11���ֹܵ���  12�����ڽ�����  13�����ڳ�����  14��������� 15���Ƶ���
  struct
  {
    uint8_t ChannelSwitch;//ͨ������   0���ر�  1����
    uint8_t Param1;
    uint8_t Param2;
    uint8_t Param3;
    uint8_t Param4;
  }ChannelConfig[20];  

  uint8_t Op_Type;//�䵪����    0��ƽ��ֵ    1�����ֵ
  uint8_t Op_Mode;//�䵪��ʽ    0���߳����  1���ȳ����
  uint8_t Op_Val;//�䵪����Ũ��
  
  int32_t FirstRecordNum;//��һ����¼���
  int32_t LastRecordNum;//���һ����¼���
  int32_t TotalNum;//���� 
}Struct_Config;

//ϵͳ���ݽṹ��
typedef struct
{
  Struct_Config Config;//��������

}Struct_SysData;


typedef struct
{
	rt_bool_t DeviceSwitch;//�豸����
	rt_bool_t MasterValveSwitch;//���ܵ���ŷ�����
	int16_t MasterPreVal;//���ܵ�ѹ��ֵ������ x10 ���棩
}Struct_MasterData;

typedef struct
{
	rt_bool_t SlaveValveSwitch;//�ֹܵ���ŷ�����
	int16_t SlavePreVal;//�ֹܵ�ѹ��ֵ������ x10 ���棩
}Struct_SlaveData;

typedef struct
{
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
	struct tm time;//ʱ��
	time_t unix_time;//ʱ���	
}Struct_WareData;

//̽���������Ϣ
typedef struct
{
	rt_bool_t updatestate;//��������״̬  1�����ظ��� 0�����ز�����
	Struct_MasterData MasterData[_MAX_MasterDevice_];
	Struct_SlaveData SlaveData[_MAX_SlaveDevice_];
	Struct_WareData WareData[_MAX_WASE_];
}Struct_monitorData;

//���ܵ���Ϣ
typedef struct 
{
	uint16_t id;//���ܵ����
	rt_bool_t DeviceSwitch;//�豸����
	rt_bool_t MasterValveSwitch;//���ܵ���ŷ�����
	
	int16_t MasterPreVal;//���ܵ�ѹ��ֵ������ x10 ���棩
}mq_type1;

//�ֹܵ���Ϣ
typedef struct 
{
	uint16_t id;//�ֹܵ����
	
	rt_bool_t SlaveValveSwitch;//�ֹܵ���ŷ�����
	
	uint16_t SlavePreVal;//�ֹܵ�ѹ��ֵ
}mq_type2;

//�ַ���Ϣ
typedef struct 
{
	uint16_t id;//�ַ����
	uint16_t conc1;//����1Ũ��
	uint16_t conc2;//����2Ũ��
	uint16_t conc3;//����3Ũ��
	uint16_t conc4;//����4Ũ��
	uint16_t conc5;//����5Ũ��
	uint16_t conc_avg;//ƽ��ֵ
	uint16_t conc_low;//��Сֵ
	uint16_t pressure;//������ѹֵ
	uint8_t valve1;//������ŷ�״̬
	uint8_t valve2;//������ŷ�״̬
	uint8_t valve3;//�������״̬
}mq_type3;

//������Ϣ
typedef struct 
{
//1���趨����Ũ��ͨ������������
//3�����Ƶ�ŷ�����
//4�������Ƶ���
//5��ȡ������	
//6�����ظ�����������
	
//7���ڲ���λ����	
	uint8_t cmd;//��������
	
	int16_t Master_id;//���Ƶ�Ԫ���

//���ܵ���ŷ�
//�ֹܵ���ŷ�	
//�ַ���ŷ�
//�������		
	int16_t Slave_id_1;//�����ӵ�Ԫ���1
//�ַ�������ŷ�
//�ַ�������ŷ�	
	int16_t Slave_id_2;//�����ӵ�Ԫ���2
//����ֵ
	int16_t Param_1;
//��ǰֵ	
	int16_t Param_2;
//����ִ�����		
	int16_t Param_3;
//�����Ƿ�֧��	
	int16_t Param_4;
	
	uint8_t cancelTaskcode[21];
	
	uint8_t Taskcode[21];
}mq_type4;

typedef struct
{
//4������	
//8���ڲ�����	
	uint8_t type;
	union
	{
		mq_type1 data1;
		mq_type2 data2;
		mq_type3 data3;
		mq_type4 data4;
		mq_type4 data5;
		mq_type4 data6;
		mq_type4 data7;
		mq_type4 data8;//�豸�ڲ�����
	}data;
	struct tm time;//ʱ��
	time_t unix_time;//ʱ���	
}mq_data;

//����ִ��
typedef struct 
{
//1���趨����Ũ��ͨ�����������ͻ��Ƿ���±�������
//3�����Ƶ�ŷ�����
//4�������Ƶ���
//5��ȡ������	
//6�����ظ�����������	
	uint8_t cmd;//��������
	int8_t timeout;//��ʱ����
	
	uint16_t Master_id;//���Ƶ�Ԫ���
//���ܵ���ŷ�
//�ֹܵ���ŷ�	
//�ַ���ŷ�
//�������		
	uint16_t Slave_id_1;//�����ӵ�Ԫ���1
//�ַ�������ŷ�
//�ַ�������ŷ�	
	uint16_t Slave_id_2;//�����ӵ�Ԫ���2
//����ֵ
	uint8_t Param_1;
//��ǰֵ	
	uint8_t Param_2;
//����ִ�����		
	uint8_t Param_3;
//�����Ƿ�֧��		
	uint8_t Param_4;
	
	uint8_t cancelTaskcode[21];
	uint8_t Taskcode[21];
}cmd_Data;



#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/

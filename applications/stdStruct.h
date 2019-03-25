#ifndef __stdStruct_H_
#define __stdStruct_H_


/*-----------------------------------------引入头文件----------------------------------------------------*/
#include <rtthread.h>
/*------------------------------------------------------------------------------------------------------*/

#define _MAX_WASE_						(50)
#define _MAX_SlaveDevice_			(10)
#define _MAX_MasterDevice_		(1)

//系统配置结构体
typedef struct
{
  uint8_t DeviceSeries;//设备系列   缺省值为0
  uint8_t DeviceModel;//设备型号    1：仓内型   2：分管道型   3：设备控制型

  uint8_t network;//网络号
	uint8_t panel;//监控设备号
	uint8_t loop;//回路号
  uint16_t addr;//设备地址
	
//1：氮气1  2：氮气2  3：氮气3  4：氮气4  5：氮气5  6：仓内压力  7：分管道压力 8：主管道压力 9：制氮机压力 10：主管道阀
//11：分管道阀  12：仓内进气阀  13：仓内出气阀  14：环流风机 15：制氮机
  struct
  {
    uint8_t ChannelSwitch;//通道开关   0：关闭  1：打开
    uint8_t Param1;
    uint8_t Param2;
    uint8_t Param3;
    uint8_t Param4;
  }ChannelConfig[20];  

  uint8_t Op_Type;//充氮类型    0：平均值    1：最低值
  uint8_t Op_Mode;//充氮方式    0：边冲边排  1：先充后排
  uint8_t Op_Val;//充氮控制浓度
  
  int32_t FirstRecordNum;//第一条记录序号
  int32_t LastRecordNum;//最后一条记录序号
  int32_t TotalNum;//总数 
}Struct_Config;

//系统数据结构体
typedef struct
{
  Struct_Config Config;//配置数据

}Struct_SysData;


typedef struct
{
	rt_bool_t DeviceSwitch;//设备开关
	rt_bool_t MasterValveSwitch;//主管道电磁阀开关
	int16_t MasterPreVal;//主管道压力值（数据 x10 保存）
}Struct_MasterData;

typedef struct
{
	rt_bool_t SlaveValveSwitch;//分管道电磁阀开关
	int16_t SlavePreVal;//分管道压力值（数据 x10 保存）
}Struct_SlaveData;

typedef struct
{
	int16_t conc1;//氮气1浓度
	int16_t conc2;//氮气2浓度
	int16_t conc3;//氮气3浓度
	int16_t conc4;//氮气4浓度
	int16_t conc5;//氮气5浓度
	int16_t conc_avg;//平均值
	int16_t conc_low;//最小值
	int16_t wasePre;//仓内气压值
	uint8_t valve1;//进气电磁阀状态
	uint8_t valve2;//出气电磁阀状态
	uint8_t valve3;//环流风机状态
	struct tm time;//时间
	time_t unix_time;//时间戳	
}Struct_WareData;

//探测器监控信息
typedef struct
{
	rt_bool_t updatestate;//本机更新状态  1：本地更新 0：本地不更新
	Struct_MasterData MasterData[_MAX_MasterDevice_];
	Struct_SlaveData SlaveData[_MAX_SlaveDevice_];
	Struct_WareData WareData[_MAX_WASE_];
}Struct_monitorData;

//主管道信息
typedef struct 
{
	uint16_t id;//主管道编号
	rt_bool_t DeviceSwitch;//设备开关
	rt_bool_t MasterValveSwitch;//主管道电磁阀开关
	
	int16_t MasterPreVal;//主管道压力值（数据 x10 保存）
}mq_type1;

//分管道信息
typedef struct 
{
	uint16_t id;//分管道编号
	
	rt_bool_t SlaveValveSwitch;//分管道电磁阀开关
	
	uint16_t SlavePreVal;//分管道压力值
}mq_type2;

//仓房信息
typedef struct 
{
	uint16_t id;//仓房编号
	uint16_t conc1;//氮气1浓度
	uint16_t conc2;//氮气2浓度
	uint16_t conc3;//氮气3浓度
	uint16_t conc4;//氮气4浓度
	uint16_t conc5;//氮气5浓度
	uint16_t conc_avg;//平均值
	uint16_t conc_low;//最小值
	uint16_t pressure;//仓内气压值
	uint8_t valve1;//进气电磁阀状态
	uint8_t valve2;//出气电磁阀状态
	uint8_t valve3;//环流风机状态
}mq_type3;

//命令信息
typedef struct 
{
//1：设定氮气浓度通道传感器类型
//3：控制电磁阀开关
//4：控制制氮机
//5：取消命令	
//6：本地更新数据命令
	
//7：内部复位命令	
	uint8_t cmd;//命令类型
	
	int16_t Master_id;//控制单元编号

//主管道电磁阀
//分管道电磁阀	
//仓房电磁阀
//环流风机		
	int16_t Slave_id_1;//控制子单元编号1
//仓房进气电磁阀
//仓房出气电磁阀	
	int16_t Slave_id_2;//控制子单元编号2
//控制值
	int16_t Param_1;
//当前值	
	int16_t Param_2;
//任务执行情况		
	int16_t Param_3;
//命令是否支持	
	int16_t Param_4;
	
	uint8_t cancelTaskcode[21];
	
	uint8_t Taskcode[21];
}mq_type4;

typedef struct
{
//4：命令	
//8：内部命令	
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
		mq_type4 data8;//设备内部命令
	}data;
	struct tm time;//时间
	time_t unix_time;//时间戳	
}mq_data;

//命令执行
typedef struct 
{
//1：设定氮气浓度通道传感器类型或是否更新本地数据
//3：控制电磁阀开关
//4：控制制氮机
//5：取消命令	
//6：本地更新数据命令	
	uint8_t cmd;//命令类型
	int8_t timeout;//超时次数
	
	uint16_t Master_id;//控制单元编号
//主管道电磁阀
//分管道电磁阀	
//仓房电磁阀
//环流风机		
	uint16_t Slave_id_1;//控制子单元编号1
//仓房进气电磁阀
//仓房出气电磁阀	
	uint16_t Slave_id_2;//控制子单元编号2
//控制值
	uint8_t Param_1;
//当前值	
	uint8_t Param_2;
//任务执行情况		
	uint8_t Param_3;
//命令是否支持		
	uint8_t Param_4;
	
	uint8_t cancelTaskcode[21];
	uint8_t Taskcode[21];
}cmd_Data;



#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/

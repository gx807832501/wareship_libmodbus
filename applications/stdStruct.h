#ifndef __stdStruct_H_
#define __stdStruct_H_


/*-----------------------------------------引入头文件----------------------------------------------------*/
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


//系统配置结构体
typedef struct
{
  uint8_t DeviceSeries;//设备系列   缺省值为0
  uint8_t DeviceModel;//设备型号    1：仓内型   2：分管道型   3：设备控制型
	
	uint8_t com_BaudRate;//波特率
	uint8_t com_Parity;//校验位
	
  uint8_t ServiceHost[30];//Service地址 第一字节表示是否有效 0x5A表示有效
  uint8_t IpAddr[20];//Ip地址 第一字节表示是否有效 0x5A表示有效
  uint8_t GWAddr[20];//网关地址 第一字节表示是否有效 0x5A表示有效
  uint8_t MSKAddr[20];//子网掩码 第一字节表示是否有效 0x5A表示有效

  uint8_t network;//网络号
	uint8_t panel;//监控设备号
	uint8_t loop;//回路号
  uint16_t addr;//设备地址
	
//1：氮气1  2：氮气2  3：氮气3  4：氮气4  5：氮气5  
//6：仓内压力  7：分管道压力 8：主管道压力 9：制氮机压力 
//10：主管道阀  11：分管道阀  12：仓内进气阀  13：仓内出气阀  14：环流风机 15：制氮机开关
  struct
  {
    uint8_t ChannelSwitch;//通道开关   0：关闭  1：打开
    uint8_t attr1;//传感器型号 1:KY-2N 2:HT-N2
    uint8_t attr2;//传感器方位
    uint8_t attr3;//传感器深度
    uint8_t attr4;
  }ChannelConfig[20];  

	uint8_t timeAuto;//时间自动设备标志
	
	uint8_t DeviceState;//设备状态
	uint8_t Deviceprocess;//设备过程
	rt_bool_t DeviceSwitch;//设备开关
	rt_bool_t MasterValveSwitch;//主管道电磁阀开关
  rt_bool_t SlaveValveSwitch;//分管道电磁阀开关
	rt_bool_t valve1;//进气电磁阀状态
	rt_bool_t valve2;//出气电磁阀状态
	rt_bool_t valve3;//环流风机状态	
  
  int32_t FirstRecordNum;//第一条记录序号
  int32_t LastRecordNum;//最后一条记录序号
  int32_t TotalNum;//总数 
}Struct_Config;


typedef struct
{
  uint8_t static_val;//静态图标状态值
  uint8_t dynamic_val;//动态图标状态值

  uint8_t (*pFun)(void);//页回调函数
}Struct_PageData;

//系统数据结构体
typedef struct
{
  Struct_Config Config;//配置数据
	rt_bool_t sign;//初始化完成标志
  rt_bool_t link;
	rt_bool_t Flag_PageDatachange;//页刷新改变标志
  Struct_PageData currentPageData;//当前页数据	
}Struct_SysData;

typedef struct
{
	int8_t addr;//操作地址  
	int8_t type;//操作类型 1：气密性检查 2：充氮操作  3：电磁阀及制氮机操作  4：停止任务
	
//电磁阀参数
	int8_t No;//序号 1：进气阀 2：出气阀 3：环流风机 4：分管道阀  5：主管道阀  6：制氮机
	int8_t val;//电磁阀值  1：打开 0：关闭

//充氮参数	
	int16_t targetmode;//目标方式
	int16_t targetconc;//目标氮气浓度	
	int16_t targetminpre;//最小压力值
	int16_t targetmaxpre;//最大压力值
	
//气密性检测参数
	int16_t mode;//检测位置 1：主管道 2：分管道 3：仓房
	int16_t adv_pre;//预先气压值
	int16_t start_pre;//开始气压值
	int16_t end_pre;//结束气压值
	int16_t min_time;//最小时间

//停止任务参数
	uint8_t cancelcode[21];//任务编码
	
	
	uint8_t taskcode[21];//任务编码
}Struct_Op;


typedef struct
{
	uint16_t panel;//主管道编号
	
	rt_bool_t DeviceSwitch;//设备开关
	rt_bool_t MasterValveSwitch;//主管道电磁阀开关
	int16_t DevicePreVal;//设备压力值（数据 x10 保存）
	int16_t MasterPreVal;//主管道压力值（数据 x10 保存）
}Struct_MasterData;

typedef struct
{
	uint16_t panel;//主管道编号
	uint16_t loop;//分管道编号
	
	rt_bool_t SlaveValveSwitch;//分管道电磁阀开关
}Struct_SlaveData;

typedef struct
{
	uint16_t panel;//主管道编号
	uint16_t loop;//分管道编号
	uint16_t addr;//设备编号
	
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
	
	uint8_t conc1attr1;//传感器型号 1:KY-2N 2:HT-N2
	uint8_t conc2attr1;
	uint8_t conc3attr1;
	uint8_t conc4attr1;
	uint8_t conc5attr1;
	
	uint8_t conc1attr2;//传感器方位 0:中间 1：东 2：东南 3：南 4：西南 5：西 6：西北 7：北 8：东北
	uint8_t conc2attr2;
	uint8_t conc3attr2;
	uint8_t conc4attr2;
	uint8_t conc5attr2;
	
	uint8_t conc1attr3;//传感器深度 0:表面 5：0.5m 10：1m 20：2m 等等
	uint8_t conc2attr3;
	uint8_t conc3attr3;
	uint8_t conc4attr3;
	uint8_t conc5attr3;
}Struct_WareData;

//探测器监控信息
typedef struct
{
	rt_bool_t updatestate;//本机更新状态  1：本地更新 0：本地不更新
	rt_bool_t online;//设备在线状态
	
//这三个参数只有当主机是控制设备时才有意义	
	int8_t DeviceState[_MAX_DEVICE_];//设备状态 -1：设备离线 0：准备中 1：作业中
	rt_bool_t busy;//忙标志
	int8_t first,end;
	Struct_Op op[5];//最大支持同时5个任务	
	
	Struct_SlaveData SelfSlaveData;//本机分管道值
	Struct_WareData SelfWareData;//本机仓房值
	
	Struct_MasterData MasterData;
	Struct_SlaveData SlaveData[_MAX_SlaveDevice_];
	Struct_WareData WareData[_MAX_DEVICE_];
}Struct_monitorData;


//mqtt命令
typedef struct
{
	uint8_t cmdtype;//命令类型  1：复位
	int8_t cmdaddr;//命令地址	
	int16_t Param1;//命令参数1
	int16_t Param2;//命令参数2
}Struct_cmd;



//任务执行确认
typedef struct 
{
	Struct_Op task;	
//任务是否支持		
	rt_bool_t Execution;
}mq_type1;

//任务执行中间数据（只有气密性任务和充氮任务有中间数据）
typedef struct 
{
	Struct_Op task;
	
//气密性中间数据
	int16_t count;//计时器

//充氮中间数据	
	Struct_MasterData MasterData;
	Struct_SlaveData SlaveData;
	Struct_WareData WareData;
}mq_type2;


//任务执行结果数据
typedef struct 
{
	Struct_Op task;
//任务执行最终结果	
	rt_bool_t state;
//任务错误代码
	int8_t errorcode;

//气密性检测参数
	int16_t count;//计时器
	
//充氮参数	
	Struct_MasterData MasterData;
	Struct_SlaveData SlaveData;
	Struct_WareData WareData;
}mq_type3;


typedef struct
{
//1：主管道数据
//2：分管道数据	
//3：仓房数据
//4：任务信息	
//5：任务执行确认
//6：任务执行中间数据（只有气密性任务和充氮任务有中间数据）
//7：任务执行结果数据
//8：命令	
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
	struct tm time;//时间
	time_t unix_time;//时间戳	
}mq_data;


#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/

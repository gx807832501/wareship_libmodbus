/*********************************************************************************************************
//程序名：				mq_client.c
//编写人：				高宵
//编写时间：			2019年2月13日
//版本号：				1.0.0
//备注：				  None
*********************************************************************************************************/
/*-----------------------------------------引入头文件----------------------------------------------------*/
#include "mq_client.h"
/*------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------定义变量----------------------------------------------------*/
/* 定义 MQTT 客户端环境结构体 */
static MQTTClient client;
/*-------------------------------------------------------------------------------------------------------*/

/* MQTT 订阅事件自定义回调函数 */
static void mqtt_sub_callback(MQTTClient *c, MessageData *msg_data)
{

	
	*((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
	LOG_D("mqtt sub default callback: %.*s %.*s",
						 msg_data->topicName->lenstring.len,
						 msg_data->topicName->lenstring.data,
						 msg_data->message->payloadlen,
						 (char *)msg_data->message->payload);
	
	return;
}

/* MQTT 订阅事件默认回调函数 */
static void mqtt_sub_default_callback(MQTTClient *c, MessageData *msg_data)
{

	
	*((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
	LOG_D("mqtt sub default callback: %.*s %.*s",
						 msg_data->topicName->lenstring.len,
						 msg_data->topicName->lenstring.data,
						 msg_data->message->payloadlen,
						 (char *)msg_data->message->payload);
	
	return;
}

/* MQTT 连接事件回调函数 */
static void mqtt_connect_callback(MQTTClient *c)
{
	LOG_D("inter mqtt_connect_callback!");
}

/* MQTT 上线事件回调函数 */
static void mqtt_online_callback(MQTTClient *c)
{
	LOG_D("inter mqtt_online_callback!");
}

/* MQTT 下线事件回调函数 */
static void mqtt_offline_callback(MQTTClient *c)
{
	LOG_D("inter mqtt_offline_callback!");
}

/**
 * 这个函数创建并配置 MQTT 客户端。
 *
 * @param void
 *
 * @return none
 */
static void mq_start(void)
{
/* 使用 MQTTPacket_connectData_initializer 初始化 condata 参数 */
	MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;

	static int is_started = 0;
	if (is_started)
	{
			return;
	}

/* 配置MQTT 结构体内容参数 */
	{
/* 服务器地址 */
		client.uri = MQTT_URI;

/* 配置连接参数 */
		memcpy(&client.condata, &condata, sizeof(condata));
		client.condata.clientID.cstring = MQTT_CLIENTID;
		client.condata.keepAliveInterval = 60;
		client.condata.cleansession = 1;
		client.condata.username.cstring = MQTT_USERNAME;
		client.condata.password.cstring = MQTT_PASSWORD;
		
/* 配置 MQTT 遗言参数 */
		client.condata.willFlag = 0;
		client.condata.will.qos = 1;
		client.condata.will.retained = 0;
		client.condata.will.topicName.cstring = MQTT_PUBTOPIC;
		client.condata.will.message.cstring = MQTT_WILLMSG;

/* 分配缓冲区 */
		client.buf_size = client.readbuf_size = 1024;
		client.buf = malloc(client.buf_size);
		client.readbuf = malloc(client.readbuf_size);
		if (!(client.buf && client.readbuf))
		{
			LOG_E("no memory for MQTT client buffer!");
			goto _exit;
		}

/* 设置事件回调函数 */
		client.connect_callback = mqtt_connect_callback;
		client.online_callback = mqtt_online_callback;
		client.offline_callback = mqtt_offline_callback;

/* 设置订阅表和事件回调函数*/
		client.messageHandlers[0].topicFilter = MQTT_SUBTOPIC1;
		client.messageHandlers[0].callback = mqtt_sub_callback;
		client.messageHandlers[0].qos = QOS0;
		
		client.messageHandlers[1].topicFilter = MQTT_SUBTOPIC2;
		client.messageHandlers[1].callback = mqtt_sub_callback;
		client.messageHandlers[1].qos = QOS0;		

/* 设置默认的订阅主题*/
		client.defaultMessageHandler = mqtt_sub_default_callback;
	}

/* 运行 MQTT 客户端 */

	paho_mqtt_start(&client);
	is_started = 1;

_exit:
	return;
}

/**
 * 这个函数推送消息给特定的 MQTT 主题。
 *
 * @param send_str publish message
 *
 * @return none
 */
static void mq_publish(const char *topic,const char *send_str)
{
	MQTTMessage message;
	const char *msg_str = send_str;
	
	message.qos = QOS0; //消息等级
	message.retained = 0;
	message.payload = (void *)msg_str;
	message.payloadlen = strlen(message.payload);

	MQTTPublish(&client, topic, &message);

	return;
}

/*********************************************************************************************************
**函数名称：void mq_client_entry(void * parameter)
**函数描述：mq客户端入口函数
**输入参数：无
**返回值：  无
*********************************************************************************************************/
void mq_client_entry(void * parameter)
{
	mq_start();
	while(1)
	{
		rt_thread_delay(500);
	}
}

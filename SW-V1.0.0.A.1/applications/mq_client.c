/*********************************************************************************************************
//��������				mq_client.c
//��д�ˣ�				����
//��дʱ�䣺			2019��2��13��
//�汾�ţ�				1.0.0
//��ע��				  None
*********************************************************************************************************/
/*-----------------------------------------����ͷ�ļ�----------------------------------------------------*/
#include "mq_client.h"
/*------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------�������----------------------------------------------------*/
/* ���� MQTT �ͻ��˻����ṹ�� */
static MQTTClient client;
/*-------------------------------------------------------------------------------------------------------*/

/* MQTT �����¼��Զ���ص����� */
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

/* MQTT �����¼�Ĭ�ϻص����� */
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

/* MQTT �����¼��ص����� */
static void mqtt_connect_callback(MQTTClient *c)
{
	LOG_D("inter mqtt_connect_callback!");
}

/* MQTT �����¼��ص����� */
static void mqtt_online_callback(MQTTClient *c)
{
	LOG_D("inter mqtt_online_callback!");
}

/* MQTT �����¼��ص����� */
static void mqtt_offline_callback(MQTTClient *c)
{
	LOG_D("inter mqtt_offline_callback!");
}

/**
 * ����������������� MQTT �ͻ��ˡ�
 *
 * @param void
 *
 * @return none
 */
static void mq_start(void)
{
/* ʹ�� MQTTPacket_connectData_initializer ��ʼ�� condata ���� */
	MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;

	static int is_started = 0;
	if (is_started)
	{
			return;
	}

/* ����MQTT �ṹ�����ݲ��� */
	{
/* ��������ַ */
		client.uri = MQTT_URI;

/* �������Ӳ��� */
		memcpy(&client.condata, &condata, sizeof(condata));
		client.condata.clientID.cstring = MQTT_CLIENTID;
		client.condata.keepAliveInterval = 60;
		client.condata.cleansession = 1;
		client.condata.username.cstring = MQTT_USERNAME;
		client.condata.password.cstring = MQTT_PASSWORD;
		
/* ���� MQTT ���Բ��� */
		client.condata.willFlag = 0;
		client.condata.will.qos = 1;
		client.condata.will.retained = 0;
		client.condata.will.topicName.cstring = MQTT_PUBTOPIC;
		client.condata.will.message.cstring = MQTT_WILLMSG;

/* ���仺���� */
		client.buf_size = client.readbuf_size = 1024;
		client.buf = malloc(client.buf_size);
		client.readbuf = malloc(client.readbuf_size);
		if (!(client.buf && client.readbuf))
		{
			LOG_E("no memory for MQTT client buffer!");
			goto _exit;
		}

/* �����¼��ص����� */
		client.connect_callback = mqtt_connect_callback;
		client.online_callback = mqtt_online_callback;
		client.offline_callback = mqtt_offline_callback;

/* ���ö��ı���¼��ص�����*/
		client.messageHandlers[0].topicFilter = MQTT_SUBTOPIC1;
		client.messageHandlers[0].callback = mqtt_sub_callback;
		client.messageHandlers[0].qos = QOS0;
		
		client.messageHandlers[1].topicFilter = MQTT_SUBTOPIC2;
		client.messageHandlers[1].callback = mqtt_sub_callback;
		client.messageHandlers[1].qos = QOS0;		

/* ����Ĭ�ϵĶ�������*/
		client.defaultMessageHandler = mqtt_sub_default_callback;
	}

/* ���� MQTT �ͻ��� */

	paho_mqtt_start(&client);
	is_started = 1;

_exit:
	return;
}

/**
 * �������������Ϣ���ض��� MQTT ���⡣
 *
 * @param send_str publish message
 *
 * @return none
 */
static void mq_publish(const char *topic,const char *send_str)
{
	MQTTMessage message;
	const char *msg_str = send_str;
	
	message.qos = QOS0; //��Ϣ�ȼ�
	message.retained = 0;
	message.payload = (void *)msg_str;
	message.payloadlen = strlen(message.payload);

	MQTTPublish(&client, topic, &message);

	return;
}

/*********************************************************************************************************
**�������ƣ�void mq_client_entry(void * parameter)
**����������mq�ͻ�����ں���
**�����������
**����ֵ��  ��
*********************************************************************************************************/
void mq_client_entry(void * parameter)
{
	mq_start();
	while(1)
	{
		rt_thread_delay(500);
	}
}

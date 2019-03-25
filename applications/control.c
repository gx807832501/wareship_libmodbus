/*********************************************************************************************************
//��������				control.c
//��д�ˣ�				����
//��дʱ�䣺			2019��3��22��
//�汾�ţ�				1.0.0
//��ע��				  None
*********************************************************************************************************/
/*-----------------------------------------����ͷ�ļ�----------------------------------------------------*/
#include "control.h"
/*------------------------------------------------------------------------------------------------------*/

/* defined the LED0 pin: PB5 */
#define LED0_PIN    GET_PIN(B, 5)
/* defined the LED0 pin: PE5 */
#define LED1_PIN    GET_PIN(E, 5)
/* defined the RELAY0 pin: PB12 */
#define RELAY0_PIN    GET_PIN(B, 12)

//led��ʼ������
void led_init()
{
	/* set LED0 pin mode to output */
	rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	/* set LED1 pin mode to output */
	rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
	
	rt_pin_write(LED0_PIN,PIN_HIGH);
	
	rt_pin_write(LED1_PIN,PIN_HIGH);
}

//relay��ʼ������
void relay_init()
{
	/* set RELAY0 pin mode to output */
	rt_pin_mode(RELAY0_PIN, PIN_MODE_OUTPUT);
	
	rt_pin_write(RELAY0_PIN,PIN_HIGH);
}

//led0�򿪺���
void led0_open()
{
	rt_pin_write(LED0_PIN,PIN_LOW);
}

//led0�رպ���
void led0_close()
{
	rt_pin_write(LED0_PIN,PIN_HIGH);
}

//led0��ת����
void led0_toggle()
{
	rt_pin_write(LED0_PIN,!rt_pin_read(LED0_PIN));
}

//led1�򿪺���
void led1_open()
{
	rt_pin_write(LED1_PIN,PIN_LOW);
}

//led1�رպ���
void led1_close()
{
	rt_pin_write(LED1_PIN,PIN_HIGH);
}

//led1��ת����
void led1_toggle()
{
	rt_pin_write(LED1_PIN,!rt_pin_read(LED1_PIN));
}

//relay�򿪺���
void relay_open()
{
	rt_pin_write(RELAY0_PIN, PIN_LOW);
}

//relay�رպ���
void relay_close()
{
	rt_pin_write(RELAY0_PIN, PIN_HIGH);
}

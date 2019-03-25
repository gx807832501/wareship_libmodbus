/*********************************************************************************************************
//程序名：				control.c
//编写人：				高宵
//编写时间：			2019年3月22日
//版本号：				1.0.0
//备注：				  None
*********************************************************************************************************/
/*-----------------------------------------引入头文件----------------------------------------------------*/
#include "control.h"
/*------------------------------------------------------------------------------------------------------*/

/* defined the LED0 pin: PB5 */
#define LED0_PIN    GET_PIN(B, 5)
/* defined the LED0 pin: PE5 */
#define LED1_PIN    GET_PIN(E, 5)
/* defined the RELAY0 pin: PB12 */
#define RELAY0_PIN    GET_PIN(B, 12)

//led初始化函数
void led_init()
{
	/* set LED0 pin mode to output */
	rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	/* set LED1 pin mode to output */
	rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
	
	rt_pin_write(LED0_PIN,PIN_HIGH);
	
	rt_pin_write(LED1_PIN,PIN_HIGH);
}

//relay初始化函数
void relay_init()
{
	/* set RELAY0 pin mode to output */
	rt_pin_mode(RELAY0_PIN, PIN_MODE_OUTPUT);
	
	rt_pin_write(RELAY0_PIN,PIN_HIGH);
}

//led0打开函数
void led0_open()
{
	rt_pin_write(LED0_PIN,PIN_LOW);
}

//led0关闭函数
void led0_close()
{
	rt_pin_write(LED0_PIN,PIN_HIGH);
}

//led0翻转函数
void led0_toggle()
{
	rt_pin_write(LED0_PIN,!rt_pin_read(LED0_PIN));
}

//led1打开函数
void led1_open()
{
	rt_pin_write(LED1_PIN,PIN_LOW);
}

//led1关闭函数
void led1_close()
{
	rt_pin_write(LED1_PIN,PIN_HIGH);
}

//led1翻转函数
void led1_toggle()
{
	rt_pin_write(LED1_PIN,!rt_pin_read(LED1_PIN));
}

//relay打开函数
void relay_open()
{
	rt_pin_write(RELAY0_PIN, PIN_LOW);
}

//relay关闭函数
void relay_close()
{
	rt_pin_write(RELAY0_PIN, PIN_HIGH);
}

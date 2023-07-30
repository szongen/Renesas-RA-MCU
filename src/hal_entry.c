#include "hal_data.h"

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

#include "stdio.h"
#include "hal_tube.h"
#include "stdint.h"
#include "hal_dht11.h"
#include "multi_button.h"


//RTC变量
/* rtc_time_t is an alias for the C Standard time.h struct 'tm' */
rtc_time_t set_time =
{
    .tm_sec  = 50,      /* 秒，范围从 0 到 59 */
    .tm_min  = 59,      /* 分，范围从 0 到 59 */
    .tm_hour = 23,      /* 小时，范围从 0 到 23*/
    .tm_mday = 29,       /* 一月中的第几天，范围从 0 到 30*/
    .tm_mon  = 11,      /* 月份，范围从 0 到 11*/
    .tm_year = 123,     /* 自 1900 起的年数，2023为123*/
    .tm_wday = 6,       /* 一周中的第几天，范围从 0 到 6*/
//    .tm_yday=0,         /* 一年中的第几天，范围从 0 到 365*/
//    .tm_isdst=0;        /* 夏令时*/
};


//RTC闹钟变量
rtc_alarm_time_t set_alarm_time=
{
     .time.tm_sec  = 58,      /* 秒，范围从 0 到 59 */
     .time.tm_min  = 22,      /* 分，范围从 0 到 59 */
     .time.tm_hour = 19,      /* 小时，范围从 0 到 23*/
     .time.tm_mday = 30,       /* 一月中的第几天，范围从 1 到 31*/
     .time.tm_mon  = 7,      /* 月份，范围从 0 到 11*/
     .time.tm_year = 123,     /* 自 1900 起的年数，2023为123*/
     .time.tm_wday = 6,       /* 一周中的第几天，范围从 0 到 6*/

     .sec_match        =  1,//每次秒到达设置的进行报警
     .min_match        =  0,
     .hour_match       =  0,
     .mday_match       =  0,
     .mon_match        =  0,
     .year_match       =  0,
     .dayofweek_match  =  0,
    };

	//RTC回调函数
volatile bool rtc_flag = 0;//RTC延时1s标志位
volatile bool rtc_alarm_flag = 0;//RTC闹钟
/* Callback function */
void rtc_callback(rtc_callback_args_t *p_args)
{
    /* TODO: add your own code here */
    if(p_args->event == RTC_EVENT_PERIODIC_IRQ)
        rtc_flag=1;
    else if(p_args->event == RTC_EVENT_ALARM_IRQ)
        rtc_alarm_flag=1;
}

fsp_err_t err = FSP_SUCCESS;
volatile bool uart_send_complete_flag = false;
/* Callback function */
void user_uart_callback(uart_callback_args_t *p_args)
{
    /* TODO: add your own code here */
    if(p_args->event == UART_EVENT_TX_COMPLETE)
     {
         uart_send_complete_flag = true;
     }
}


/* 重定向 printf 输出 */
#if defined __GNUC__ && !defined __clang__
int _write(int fd, char *pBuffer, int size); //防止编译警告
int _write(int fd, char *pBuffer, int size)
{
   (void)fd;
   R_SCI_UART_Write(&g_uart4_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return size;
}
#else
int fputc(int ch, FILE *f)
{
   (void)f;
   R_SCI_UART_Write(&g_uart9_ctrl, (uint8_t *)&ch, 1);
   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return ch;
}
#endif

uint8_t show_mode ;//校验值
uint8_t humdity_integer;//湿度整数
uint8_t humdity_decimal;//湿度小数
uint8_t temp_integer ;//温度整数
uint8_t temp_decimal ;//温度小数
uint8_t dht11_check ;//校验值


uint8_t rtc_second= 0;      //秒
uint8_t rtc_minute =0;      //分
uint8_t rtc_hour =0;         //时
uint8_t rtc_day =0;          //日
uint8_t rtc_month =0;      //月
uint16_t rtc_year =0;        //年
uint8_t rtc_week =0;        //周
rtc_time_t get_time;


uint64_t uiNow = 0;


uint64_t ulltimecnt = 0;
uint64_t button_timecnt = 0;
uint8_t blinker_flag = 0;

void timer0_callback(timer_callback_args_t *p_args)
{
    /* TODO: add your own code here */
    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
		ulltimecnt++;
		button_timecnt++;
		
		if(button_timecnt>5)
		{
			button_timecnt = 0;
			button_ticks();
		}
		tube_show();
	}
		
}


enum Button_IDs {
	KEY1_id,
	KEY2_id,
	KEY3_id,
	KEY4_id,
};

struct Button Btn_KEY1;
struct Button Btn_KEY2;
struct Button Btn_KEY3;
struct Button Btn_KEY4;


uint8_t read_button_GPIO(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case KEY1_id:
			return (uint8_t)R_BSP_PinRead(KEY1);
		case KEY2_id:
			return (uint8_t)R_BSP_PinRead(KEY2);
		case KEY3_id:
			return (uint8_t)R_BSP_PinRead(KEY3);
		case KEY4_id:
			return (uint8_t)R_BSP_PinRead(KEY4);
		default:
			return 0;
	}
}

int8_t set_hour;
int8_t set_min;

void BTN1_SINGLE_Click_Handler(void* btn)
{
	//do something...
	show_mode++;
	if(show_mode>=4)
	{
		show_mode=0;
	}
	
	if(show_mode == 2)
	{
		R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &get_time);//获取RTC计数时间
		set_hour = get_time.tm_hour;
		set_min = get_time.tm_min;
	}
}

void BTN2_SINGLE_Click_Handler(void* btn)
{
	//do something...
//	show_mode++;
//	if(show_mode>=4)
//	{
//		show_mode=0;
//	}

	if(show_mode == 2)
	{
		set_hour--;
		if(set_hour < 0)
		{
			set_hour = 24;
		}
	}else if(show_mode == 3)
	{
		set_min--;
		if(set_min< 0)
		{
			set_min = 60;
		}
	}
	
	
}


void BTN3_SINGLE_Click_Handler(void* btn)
{
	//do something...
//	show_mode++;
//	if(show_mode>=4)
//	{
//		show_mode=0;
//	}

		if(show_mode == 2)
	{
		set_hour++;
		if(set_hour > 24)
		{
			set_hour = 0;
		}
	}else if(show_mode == 3)
	{
		set_min++;
		if(set_min > 60)
		{
			set_min = 0;
		}
	}
	
}


void BTN4_SINGLE_Click_Handler(void* btn)
{
	//do something...
//	show_mode++;
//	if(show_mode>=4)
//	{
//		show_mode=0;
//	}
	if(show_mode>=2 && show_mode<=3)
	{
		set_time.tm_hour = set_hour;
		set_time.tm_min = set_min;
		R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &set_time);
	}
}


void BTN2_LONG_PRESS_HOLD_Handler(void* btn)
{
	//do something...
//	show_mode++;
//	if(show_mode>=4)
//	{
//		show_mode=0;
//	}
	if(show_mode == 2)
	{
		set_hour--;
		if(set_hour < 0)
		{
			set_hour = 24;
		}
	}else if(show_mode == 3)
	{
		set_min--;
		if(set_min< 0)
		{
			set_min = 60;
		}
	}
}


void BTN3_LONG_PRESS_HOLD_Handler(void* btn)
{
	//do something...
	if(show_mode == 2)
	{
		set_hour++;
		if(set_hour > 24)
		{
			set_hour = 0;
		}
	}else if(show_mode == 3)
	{
		set_min++;
		if(set_min > 60)
		{
			set_min = 0;
		}
	}
}




/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    /* TODO: add your own code here */
	R_IOPORT_PinWrite(&g_ioport_ctrl, BUZZER, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_A, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_B, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_C, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_D, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_E, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_F, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_G, BSP_IO_LEVEL_HIGH);
//	R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_DP, BSP_IO_LEVEL_HIGH);
	err = R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);
       /* Handle any errors. This function should be defined by the user. */
   assert(FSP_SUCCESS == err);
	
   err = R_SCI_UART_Open(&g_uart9_ctrl, &g_uart9_cfg);
       assert(FSP_SUCCESS == err);
       /* Start the timer. */
   (void) R_GPT_Start(&g_timer0_ctrl);
	
	 err = R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Set the RTC clock source. Can be skipped if "Set Source Clock in Open" property is enabled. */
    R_RTC_ClockSourceSet(&g_rtc0_ctrl);

/* R_RTC_CalendarTimeSet must be called at least once to start the RTC */
//    R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &set_time);
    /* Set the periodic interrupt rate to 1 second */
    R_RTC_PeriodicIrqRateSet(&g_rtc0_ctrl, RTC_PERIODIC_IRQ_SELECT_1_SECOND);

   R_RTC_CalendarAlarmSet(&g_rtc0_ctrl, &set_alarm_time);
	show_mode = 0;
//ceshi_smg();
	
	
	button_init(&Btn_KEY1, read_button_GPIO, 0, KEY1_id);
	button_init(&Btn_KEY2, read_button_GPIO, 0, KEY2_id);
	button_init(&Btn_KEY3, read_button_GPIO, 0, KEY3_id);
	button_init(&Btn_KEY4, read_button_GPIO, 0, KEY4_id);

//	button_attach(&Btn_KEY1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
//	button_attach(&Btn_KEY1, PRESS_UP,         BTN1_PRESS_UP_Handler);
//	button_attach(&Btn_KEY1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler);
	button_attach(&Btn_KEY1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
//	button_attach(&Btn_KEY1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);
//	button_attach(&Btn_KEY1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);
//	button_attach(&Btn_KEY1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler);

//	button_attach(&Btn_KEY2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
//	button_attach(&Btn_KEY2, PRESS_UP,         BTN2_PRESS_UP_Handler);
//	button_attach(&Btn_KEY2, PRESS_REPEAT,     BTN2_PRESS_REPEAT_Handler);
	button_attach(&Btn_KEY2, SINGLE_CLICK,     BTN2_SINGLE_Click_Handler);
//	button_attach(&Btn_KEY2, DOUBLE_CLICK,     BTN2_DOUBLE_Click_Handler);
//	button_attach(&Btn_KEY2, LONG_PRESS_START, BTN2_LONG_PRESS_START_Handler);
//	button_attach(&Btn_KEY2, LONG_PRESS_HOLD,  BTN2_LONG_PRESS_HOLD_Handler);
//	
//	button_attach(&Btn_KEY3, PRESS_DOWN,       BTN3_PRESS_DOWN_Handler);
//	button_attach(&Btn_KEY3, PRESS_UP,         BTN3_PRESS_UP_Handler);
//	button_attach(&Btn_KEY3, PRESS_REPEAT,     BTN3_PRESS_REPEAT_Handler);
	button_attach(&Btn_KEY3, SINGLE_CLICK,     BTN3_SINGLE_Click_Handler);
//	button_attach(&Btn_KEY3, DOUBLE_CLICK,     BTN3_DOUBLE_Click_Handler);
//	button_attach(&Btn_KEY3, LONG_PRESS_START, BTN3_LONG_PRESS_START_Handler);
//	button_attach(&Btn_KEY3, LONG_PRESS_HOLD,  BTN3_LONG_PRESS_HOLD_Handler);
//	
//	button_attach(&Btn_KEY4, PRESS_DOWN,       BTN4_PRESS_DOWN_Handler);
//	button_attach(&Btn_KEY4, PRESS_UP,         BTN4_PRESS_UP_Handler);
//	button_attach(&Btn_KEY4, PRESS_REPEAT,     BTN4_PRESS_REPEAT_Handler);
	button_attach(&Btn_KEY4, SINGLE_CLICK,     BTN4_SINGLE_Click_Handler);
//	button_attach(&Btn_KEY4, DOUBLE_CLICK,     BTN4_DOUBLE_Click_Handler);
//	button_attach(&Btn_KEY4, LONG_PRESS_START, BTN4_LONG_PRESS_START_Handler);
//	button_attach(&Btn_KEY4, LONG_PRESS_HOLD,  BTN4_LONG_PRESS_HOLD_Handler);

	button_start(&Btn_KEY1);
	button_start(&Btn_KEY2);
	button_start(&Btn_KEY3);
	button_start(&Btn_KEY4);
	
	while(1)
	{
//		if(R_IOPORT_PinRead(&g_ioport_ctrl,KEY3,BSP_IO_LEVEL_LOW))
//		{
//			printf("test\n");
//		}
		
//		R_IOPORT_PinRead(&g_ioport_ctrl, KEY1, &state);
//		if(state == BSP_IO_LEVEL_LOW){
//			R_BSP_SoftwareDelay (20, BSP_DELAY_UNITS_MILLISECONDS);
//			R_IOPORT_PinRead(&g_ioport_ctrl, KEY1, &state);
//			if(state == BSP_IO_LEVEL_LOW){
//			show_mode++;
//			}
//		}
		
		
//		if(show_mode>=2)
//		{
//			show_mode=0;
//		}
		
		
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_A, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_B, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_C, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_D, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_E, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_F, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_G, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_DP, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG1, BSP_IO_LEVEL_LOW);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG2, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG3, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG4, BSP_IO_LEVEL_HIGH);
//		
//		R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_A, BSP_IO_LEVEL_LOW);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_B, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_C, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_D, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_E, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_F, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_G, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG_DP, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG1, BSP_IO_LEVEL_HIGH);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG2, BSP_IO_LEVEL_LOW);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG3, BSP_IO_LEVEL_LOW);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG4, BSP_IO_LEVEL_HIGH);
//		R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
		
//		R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MICROSECONDS);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG2, BSP_IO_LEVEL_LOW);
//		R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MICROSECONDS);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG3, BSP_IO_LEVEL_HIGH);
//		R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MICROSECONDS);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, SMG4, BSP_IO_LEVEL_HIGH);
//		R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MICROSECONDS);
//
//		R_BSP_SoftwareDelay (200, BSP_DELAY_UNITS_MILLISECONDS);

//		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_09, BSP_IO_LEVEL_LOW);
//		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_10, BSP_IO_LEVEL_HIGH);
//		R_BSP_SoftwareDelay (200, BSP_DELAY_UNITS_MILLISECONDS);

//		uiNow = ulltimecnt;
//		ceshi_smg();
//		printf("test\n");
//		R_SCI_UART_Write (&g_uart9_ctrl, "1", 1);

		if(uiNow == 0 || ulltimecnt - uiNow >500)
		{
			uiNow = ulltimecnt;
			DHT11_Read();
			R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &get_time);//获取RTC计数时间
			rtc_flag=0;
			rtc_second=get_time.tm_sec;//秒
			rtc_minute=get_time.tm_min;//分
			rtc_hour=get_time.tm_hour;//时
			rtc_day=get_time.tm_mday;//日
			rtc_month=get_time.tm_mon;//月
			rtc_year=get_time.tm_year; //年
			rtc_week=get_time.tm_wday;//周
			blinker_flag ++;
			if(blinker_flag > 1)
			{
				blinker_flag = 0;
			}
		}

		

//		//时间显示
//		num1=rtc_hour/10;
//		num2=rtc_hour%10;

//		num3=rtc_minute/10;
//		num4=rtc_minute%10;



//R_BSP_SoftwareDelay (200, BSP_DELAY_UNITS_MILLISECONDS);
//		R_BSP_SoftwareDelay (200, BSP_DELAY_UNITS_MILLISECONDS);


//		R_IOPORT_PinWrite(&g_ioport_ctrl, BUZZER, BSP_IO_LEVEL_HIGH);
//		
//		R_BSP_SoftwareDelay (40, BSP_DELAY_UNITS_MILLISECONDS);
//		
//		R_IOPORT_PinWrite(&g_ioport_ctrl, BUZZER, BSP_IO_LEVEL_LOW);
//		
//		R_BSP_SoftwareDelay (40, BSP_DELAY_UNITS_MILLISECONDS);

//		R_BSP_PinWrite(BSP_IO_PORT_01_PIN_04, BSP_IO_LEVEL_LOW);
//		R_BSP_PinWrite(BSP_IO_PORT_00_PIN_00, BSP_IO_LEVEL_HIGH);
//		R_BSP_PinWrite(BSP_IO_PORT_03_PIN_02, BSP_IO_LEVEL_HIGH);
		
	}
#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, &IOPORT_CFG_NAME);
    }
}

#if BSP_TZ_SECURE_BUILD

FSP_CPP_HEADER
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{

}
FSP_CPP_FOOTER

#endif

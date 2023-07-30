/*
 * smg.c
 *
 *  Created on: 2023年6月29日
 *      Author: a8456
 */

#include "hal_tube.h"
#include "hal_data.h"


void smg_num(int num)
{
    switch(num)
    {
        case 0:
        {
            SMG_A_OPEN;
            SMG_B_OPEN;
            SMG_C_OPEN;
            SMG_D_OPEN;
            SMG_E_OPEN;
            SMG_F_OPEN;
            SMG_G_CLOSE;
            SMG_DP_CLOSE;
            break;
        }
        case 1:
        {
            SMG_A_CLOSE;
            SMG_B_OPEN;
            SMG_C_OPEN;
            SMG_D_CLOSE;
            SMG_E_CLOSE;
            SMG_F_CLOSE;
            SMG_G_CLOSE;
            SMG_DP_CLOSE;
            break;
        }
        case 2:
        {
            SMG_A_OPEN;
            SMG_B_OPEN;
            SMG_C_CLOSE;
            SMG_D_OPEN;
            SMG_E_OPEN;
            SMG_F_CLOSE;
            SMG_G_OPEN;
            SMG_DP_CLOSE;
            break;
        }
        case 3:
        {
            SMG_A_OPEN;
            SMG_B_OPEN;
            SMG_C_OPEN;
            SMG_D_OPEN;
            SMG_E_CLOSE;
            SMG_F_CLOSE;
            SMG_G_OPEN;
            SMG_DP_CLOSE;
            break;
        }
        case 4:
        {
            SMG_A_CLOSE;
            SMG_B_OPEN;
            SMG_C_OPEN;
            SMG_D_CLOSE;
            SMG_E_CLOSE;
            SMG_F_OPEN;
            SMG_G_OPEN;
            SMG_DP_CLOSE;
            break;
        }
        case 5:
        {
            SMG_A_OPEN;
            SMG_B_CLOSE;
            SMG_C_OPEN;
            SMG_D_OPEN;
            SMG_E_CLOSE;
            SMG_F_OPEN;
            SMG_G_OPEN;
            SMG_DP_CLOSE;
            break;
        }
        case 6:
        {
            SMG_A_OPEN;
            SMG_B_CLOSE;
            SMG_C_OPEN;
            SMG_D_OPEN;
            SMG_E_OPEN;
            SMG_F_OPEN;
            SMG_G_OPEN;
            SMG_DP_CLOSE;
            break;
        }
        case 7:
        {
            SMG_A_OPEN;
            SMG_B_OPEN;
            SMG_C_OPEN;
            SMG_D_CLOSE;
            SMG_E_CLOSE;
            SMG_F_CLOSE;
            SMG_G_CLOSE;
            SMG_DP_CLOSE;
            break;
        }
        case 8:
        {
            SMG_A_OPEN;
            SMG_B_OPEN;
            SMG_C_OPEN;
            SMG_D_OPEN;
            SMG_E_OPEN;
            SMG_F_OPEN;
            SMG_G_OPEN;
            SMG_DP_CLOSE;
            break;
        }
        case 9:
        {
            SMG_A_OPEN;
            SMG_B_OPEN;
            SMG_C_OPEN;
            SMG_D_OPEN;
            SMG_E_CLOSE;
            SMG_F_OPEN;
            SMG_G_OPEN;
            SMG_DP_CLOSE;
            break;
        }




    }



}

void smg_1(int num)
{
    SMG_1_OPEN;
    SMG_2_CLOSE;
    SMG_3_CLOSE;
    SMG_4_CLOSE;

    smg_num(num);

}


void smg_2(int num)
{
    SMG_1_CLOSE;
    SMG_2_OPEN;
    SMG_3_CLOSE;
    SMG_4_CLOSE;
    smg_num(num);

}



void smg_3(int num)
{
    SMG_1_CLOSE;
    SMG_2_CLOSE;
    SMG_3_OPEN;
    SMG_4_CLOSE;
    smg_num(num);

}



void smg_4(int num)
{

    SMG_1_CLOSE;
    SMG_2_CLOSE;
    SMG_3_CLOSE;
    SMG_4_OPEN;
    smg_num(num);

}


void smg_1_p(void)
{
    SMG_1_OPEN;
    SMG_2_CLOSE;
    SMG_3_CLOSE;
    SMG_4_CLOSE;

    SMG_A_OPEN;
    SMG_B_OPEN;
    SMG_C_CLOSE;
    SMG_D_CLOSE;
    SMG_E_OPEN;
    SMG_F_OPEN;
    SMG_G_OPEN;
    SMG_DP_CLOSE;

}





void smg_1_close(void)
{
    SMG_1_OPEN;
    SMG_2_CLOSE;
    SMG_3_CLOSE;
    SMG_4_CLOSE;

    SMG_A_CLOSE;
    SMG_B_CLOSE;
    SMG_C_CLOSE;
    SMG_D_CLOSE;
    SMG_E_CLOSE;
    SMG_F_CLOSE;
    SMG_G_CLOSE;
    SMG_DP_CLOSE;

}


void smg_2_close(void)
{
    SMG_1_CLOSE;
    SMG_2_OPEN;
    SMG_3_CLOSE;
    SMG_4_CLOSE;

    SMG_A_CLOSE;
    SMG_B_CLOSE;
    SMG_C_CLOSE;
    SMG_D_CLOSE;
    SMG_E_CLOSE;
    SMG_F_CLOSE;
    SMG_G_CLOSE;
    SMG_DP_CLOSE;
}



void smg_3_close(void)
{
    SMG_1_CLOSE;
    SMG_2_CLOSE;
    SMG_3_OPEN;
    SMG_4_CLOSE;

    SMG_A_CLOSE;
    SMG_B_CLOSE;
    SMG_C_CLOSE;
    SMG_D_CLOSE;
    SMG_E_CLOSE;
    SMG_F_CLOSE;
    SMG_G_CLOSE;
    SMG_DP_CLOSE;
}



void smg_4_close(void)
{
    SMG_1_CLOSE;
    SMG_2_CLOSE;
    SMG_3_CLOSE;
    SMG_4_OPEN;

    SMG_A_CLOSE;
    SMG_B_CLOSE;
    SMG_C_CLOSE;
    SMG_D_CLOSE;
    SMG_E_CLOSE;
    SMG_F_CLOSE;
    SMG_G_CLOSE;
    SMG_DP_CLOSE;
}








void smg_maohao_open(int num)
{

    SMG_1_CLOSE;
    SMG_2_CLOSE;

    if(num)//开启冒号
    {
        SMG_1_OPEN;
        SMG_2_OPEN;
        SMG_A_CLOSE;
        SMG_B_CLOSE;
        SMG_C_CLOSE;
        SMG_D_CLOSE;
        SMG_E_CLOSE;
        SMG_F_CLOSE;
        SMG_G_CLOSE;
        SMG_DP_OPEN;


    }
    else
    {
        SMG_1_CLOSE;
        SMG_2_CLOSE;

        SMG_A_CLOSE;
        SMG_B_CLOSE;
        SMG_C_CLOSE;
        SMG_D_CLOSE;
        SMG_E_CLOSE;
        SMG_F_CLOSE;
        SMG_G_CLOSE;
        SMG_DP_CLOSE;

    }
}

void ceshi_smg(void)
{

  smg_1(1);
R_BSP_SoftwareDelay (2, BSP_DELAY_UNITS_MILLISECONDS);
  smg_2(2);
R_BSP_SoftwareDelay (2, BSP_DELAY_UNITS_MILLISECONDS);
  smg_3(3);
R_BSP_SoftwareDelay (2, BSP_DELAY_UNITS_MILLISECONDS);
  smg_4(4);
R_BSP_SoftwareDelay (2, BSP_DELAY_UNITS_MILLISECONDS);
    
 smg_maohao_open(1);
R_BSP_SoftwareDelay (2, BSP_DELAY_UNITS_MILLISECONDS);

}

extern uint64_t ulltimecnt;
extern uint8_t humdity_integer;//湿度整数
extern uint8_t humdity_decimal;//湿度小数
extern uint8_t temp_integer ;//温度整数
extern uint8_t temp_decimal ;//温度小数
extern uint8_t dht11_check ;//校验值
extern uint8_t show_mode;

extern rtc_time_t get_time;
extern int8_t set_hour;
extern int8_t set_min;

extern uint8_t blinker_flag;

uint64_t tube_timecnt = 0;
uint64_t tube_cnt = 0;

void tube_show()
{
	tube_cnt++;
	if(show_mode == 0)
	{
		switch(tube_cnt)
		{
			case 1:
				smg_1(temp_integer/10);break;
			case 2:
				smg_2(temp_integer%10);break;
			case 3:
				smg_3(humdity_integer/10);break;
			case 4:
				smg_4(humdity_integer%10);break;	
			case 5:
				smg_maohao_open(0);break;	
			default:
				tube_cnt = 0;
		}
		
	}else if(show_mode == 1)
	{
		switch(tube_cnt)
		{
			case 1:
				smg_1(get_time.tm_hour/10);break;
			case 2:
				smg_2(get_time.tm_hour%10);break;
			case 3:
				smg_3(get_time.tm_min/10);break;
			case 4:
				smg_4(get_time.tm_min%10);break;	
			case 5:
				smg_maohao_open(blinker_flag);break;	
			default:
				tube_cnt = 0;
		}
	}
	
	else if(show_mode == 2)
	{
		switch(tube_cnt)
		{
			case 1:
				smg_1(set_hour/10);break;
			case 2:
				smg_2(set_hour%10);break;
			case 3:
//				smg_3(get_time.tm_min/10);break;
			case 4:
//				smg_4(get_time.tm_min%10);break;	
			case 5:
				smg_maohao_open(0);break;	
			default:
				tube_cnt = 0;
		}
	}
	else if(show_mode == 3)
	{
		switch(tube_cnt)
		{
			case 1:
//				smg_1(get_time.tm_hour/10);break;
			case 2:
//				smg_2(get_time.tm_hour%10);break;
			case 3:
				smg_3(set_min/10);break;
			case 4:
				smg_4(set_min%10);break;	
			case 5:
				smg_maohao_open(0);break;	
			default:
				tube_cnt = 0;
		}
	}

		
	

}



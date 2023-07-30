/*
 * dht11.c
 *
 *  Created on: 2023��6��29��
 *      Author: a8456
 */
#include "hal_dht11.h"




extern fsp_err_t err ;



uint8_t DHT11_ReadByte(void)
{
    bsp_io_level_t state;
    uint8_t ans=0,ind;
    for (ind=0;ind<8;ind++)
    {
        ans<<=1;
        do
        {
            R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, &state);
        }while(state==BSP_IO_LEVEL_LOW);//ֱ��ʶ��͵�ƽ����ʼ��������
        R_BSP_SoftwareDelay(30, BSP_DELAY_UNITS_MICROSECONDS); //��ʱ54us
        R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, &state);
        if (state==BSP_IO_LEVEL_HIGH)
            ans |= 1;
        do{
            R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, &state);
        }while(state==BSP_IO_LEVEL_HIGH);
    }
    return ans;
}


extern uint8_t humdity_integer;//ʪ������
extern uint8_t humdity_decimal;//ʪ��С��
extern uint8_t temp_integer ;//�¶�����
extern uint8_t temp_decimal ;//�¶�С��
extern uint8_t dht11_check ;//У��ֵ
uint8_t DHT11_Read(void)
{
    uint8_t dht11_check=0;
    bsp_io_level_t state;
    uint16_t delay_dht=0xffff;

    //DHT����ʱ��DATA��Ϊ���״̬
    R_IOPORT_PinCfg(&g_ioport_ctrl,BSP_IO_PORT_03_PIN_01,((uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT | (uint32_t) IOPORT_CFG_PORT_OUTPUT_HIGH));
    //����18ms�͵�ƽ
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(18, BSP_DELAY_UNITS_MILLISECONDS); //��ʱ20ms
    //�����20-40us�ߵ�ƽ
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, BSP_IO_LEVEL_HIGH);
//    R_BSP_SoftwareDelay(30, BSP_DELAY_UNITS_MICROSECONDS); //��ʱ30us
    //DATA��Ϊ����״̬
    R_IOPORT_PinCfg(&g_ioport_ctrl,BSP_IO_PORT_03_PIN_01,((uint32_t) IOPORT_CFG_PORT_DIRECTION_INPUT));
    //�ȴ�DATA��Ӧ�͵�ƽ,���Ϊ�ߵ�ƽ
    do{
        delay_dht--;
        if (!delay_dht)
            return 0;
        R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, &state);
    }while(state==BSP_IO_LEVEL_HIGH);
    do{
        delay_dht--;
        if (!delay_dht)
            return 0;
        R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, &state);
    }while(state==BSP_IO_LEVEL_LOW);
    do{
        delay_dht--;
        if (!delay_dht)
            return 0;
        R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_01, &state);
    }while(state==BSP_IO_LEVEL_HIGH);

    humdity_integer = DHT11_ReadByte();
    humdity_decimal = DHT11_ReadByte();
    temp_integer = DHT11_ReadByte();
    temp_decimal = DHT11_ReadByte();
    dht11_check = DHT11_ReadByte();
    if (((humdity_integer+humdity_decimal+temp_integer+temp_decimal)&0xff)==dht11_check)
        return 1;
    else
        return 0;
}



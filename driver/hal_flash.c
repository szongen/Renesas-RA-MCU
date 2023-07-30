#include "hal_data.h"
#include "hal_flash.h"

volatile bool               interrupt_called;
volatile flash_event_t      flash_event;


void flash_callback (flash_callback_args_t * p_args)
{
    interrupt_called = true;
    flash_event      = p_args->event;
}


extern fsp_err_t err ;
/*FLASHд�����*/
void WriteFlashTest(uint32_t L,uint8_t Data[],uint32_t addr)
{


    interrupt_called = false;
    /* Erase 1 block of data flash starting at block 0. */
    err = R_FLASH_LP_Erase(&g_flash0_ctrl, FLASH_DF_BLOCK_0, 1);
    assert(FSP_SUCCESS == err);
    while (!interrupt_called)
    {
    ;
    }
    assert(FLASH_EVENT_ERASE_COMPLETE == flash_event);
    interrupt_called = false;
    flash_status_t status;
    /* Write 32 bytes to the first block of data flash. */
    err = R_FLASH_LP_Write(&g_flash0_ctrl, (uint32_t) Data, addr, L);
    assert(FSP_SUCCESS == err);

    /* Wait until the current flash operation completes. */
    do
    {
        err = R_FLASH_LP_StatusGet(&g_flash0_ctrl, &status);
    } while ((FSP_SUCCESS == err) && (FLASH_STATUS_BUSY == status));


    /* If the interrupt wasn't called process the error. */
    assert(interrupt_called);
    /* If the event wasn't a write complete process the error. */
    assert(FLASH_EVENT_WRITE_COMPLETE == flash_event);
    /* Verify the data was written correctly. */
    assert(0 == memcmp(Data, (uint8_t *) FLASH_DF_BLOCK_0, L));


}

extern int sec,min,hour;//����ʱ������
/*FLASH��ȡ��ӡ����*/
void PrintFlashTest(uint32_t addr)
{
//    hour=*(__IO uint8_t*)(addr);
//    min=*(__IO uint8_t*)(addr+1);

//    if(hour>=24)
//        hour=0;
//    if(min>=60)
//        min=0;
}





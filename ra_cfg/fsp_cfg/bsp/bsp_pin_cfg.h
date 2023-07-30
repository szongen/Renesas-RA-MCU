/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define SMG_G (BSP_IO_PORT_00_PIN_00) /* SMG_G */
#define SMG3 (BSP_IO_PORT_01_PIN_00) /* SMG3 */
#define SMG2 (BSP_IO_PORT_01_PIN_01) /* SMG2 */
#define SMG_F (BSP_IO_PORT_01_PIN_02) /* SMG_F */
#define SMG_A (BSP_IO_PORT_01_PIN_03) /* SMG_A */
#define SMG1 (BSP_IO_PORT_01_PIN_04) /* SMG1 */
#define KEY4 (BSP_IO_PORT_02_PIN_07) /* KEY4 */
#define KEY3 (BSP_IO_PORT_02_PIN_08) /* KEY3 */
#define SMG_DP (BSP_IO_PORT_02_PIN_12) /* SMG_DP */
#define SMG_C (BSP_IO_PORT_02_PIN_13) /* SMG_C */
#define DHT11_DATA (BSP_IO_PORT_03_PIN_01) /* DHT11_DATA */
#define BUZZER (BSP_IO_PORT_03_PIN_02) /* BUZZER */
#define SMG4 (BSP_IO_PORT_04_PIN_00) /* SMG4 */
#define SMG_E (BSP_IO_PORT_04_PIN_07) /* SMG_E */
#define SMG_D (BSP_IO_PORT_04_PIN_08) /* SMG_D */
#define SMG_B (BSP_IO_PORT_05_PIN_00) /* SMG_B */
#define KEY2 (BSP_IO_PORT_09_PIN_13) /* KEY2 */
#define KEY1 (BSP_IO_PORT_09_PIN_14) /* KEY1 */
extern const ioport_cfg_t g_bsp_pin_cfg; /* R7FA2E1A72DFL.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif /* BSP_PIN_CFG_H_ */

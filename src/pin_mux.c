/* SPDX-License-Identifier: MIT */
/* Manual configuration for FRDM-MCXA156 (from NXP SDK pin examples). */
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

void BOARD_InitBootPins(void) { BOARD_InitPins(); }

void BOARD_InitPins(void)
{
    /* Only the gates of used GPIO/PORT banks and UART/ADC are touched. */
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    CLOCK_EnableClock(kCLOCK_GatePORT0);
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    CLOCK_EnableClock(kCLOCK_GatePORT3);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kADC1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT3_RST_SHIFT_RSTn);

    const port_pin_config_t uart_pin = {
        kPORT_PullUp, kPORT_LowPullResistor, kPORT_FastSlewRate,
        kPORT_PassiveFilterDisable, kPORT_OpenDrainDisable,
        kPORT_LowDriveStrength, kPORT_NormalDriveStrength,
        kPORT_MuxAlt2, kPORT_InputBufferEnable, kPORT_InputNormal,
        kPORT_UnlockRegister
    };
    const port_pin_config_t analog_pin = {
        kPORT_PullDisable, kPORT_LowPullResistor, kPORT_FastSlewRate,
        kPORT_PassiveFilterDisable, kPORT_OpenDrainDisable,
        kPORT_LowDriveStrength, kPORT_NormalDriveStrength,
        kPORT_MuxAlt0, kPORT_InputBufferDisable, kPORT_InputNormal,
        kPORT_UnlockRegister
    };
    const port_pin_config_t gpio_out = {
        kPORT_PullDisable, kPORT_LowPullResistor, kPORT_FastSlewRate,
        kPORT_PassiveFilterDisable, kPORT_OpenDrainDisable,
        kPORT_LowDriveStrength, kPORT_NormalDriveStrength,
        kPORT_MuxAlt0, kPORT_InputBufferEnable, kPORT_InputNormal,
        kPORT_UnlockRegister
    };
    const port_pin_config_t gpio_in = {
        kPORT_PullUp, kPORT_LowPullResistor, kPORT_FastSlewRate,
        kPORT_PassiveFilterDisable, kPORT_OpenDrainDisable,
        kPORT_LowDriveStrength, kPORT_NormalDriveStrength,
        kPORT_MuxAlt0, kPORT_InputBufferEnable, kPORT_InputNormal,
        kPORT_UnlockRegister
    };
    const gpio_pin_config_t led = { .pinDirection=kGPIO_DigitalOutput, .outputLogic=0U };
    const gpio_pin_config_t sw = { .pinDirection=kGPIO_DigitalInput, .outputLogic=0U };

    PORT_SetPinConfig(PORT0, 2U, &uart_pin);   /* MCU-Link RX */
    PORT_SetPinConfig(PORT0, 3U, &uart_pin);   /* MCU-Link TX */
    PORT_SetPinConfig(PORT1, 10U, &analog_pin); /* A0 = ADC1_A8 */
    PORT_SetPinConfig(PORT1, 7U, &gpio_in);   /* SW2 = active low */
    PORT_SetPinConfig(PORT3, 1U, &gpio_out);  /* D2 activity */
    PORT_SetPinConfig(PORT3, 31U, &gpio_out); /* D4 alarm */
    GPIO_PinInit(GPIO1, 7U, &sw);
    GPIO_PinInit(GPIO3, 1U, &led);
    GPIO_PinInit(GPIO3, 31U, &led);
}

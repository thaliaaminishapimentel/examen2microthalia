/* SPDX-License-Identifier: MIT */
/* Based on NXP's FRDM-MCXA156 lpadc_polling BOARD_InitHardware setup. */
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_clock.h"
#include "fsl_reset.h"

void BOARD_InitHardware(void)
{
    /* ADC1 functional clock = FRO12M/1 (12 MHz). */
    RESET_ReleasePeripheralReset(kADC1_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kFRO12M_to_ADC1);
    CLOCK_SetClockDiv(kCLOCK_DivADC1, 1U);
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole(); /* initializes board VCOM clock */
}

# FRDM-MCXA156 | integrated ADC/GPIO/TIMER/UART application
# Uses the official NXP MCUXpresso SDK build system and Arm GNU toolchain.
cmake_minimum_required(VERSION 3.22.0)
include(${SdkRootDirPath}/cmake/extension/mcux.cmake)
project(analog_alarm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/lpadc/polling)
include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES
        src/lpadc_polling.c
        src/hardware_init.c
        src/pin_mux.c
        src/app.h
        src/pin_mux.h
        src/alarm_core.h
)
mcux_add_include(INCLUDES src)
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

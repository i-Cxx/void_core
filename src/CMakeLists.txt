# src/CMakeLists.txt – für void_universe_core

cmake_minimum_required(VERSION 3.14)

project(void_universe_core C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

# Webserver Subdirectory einbinden
add_subdirectory(webserver)

add_executable(void_universe_core 
    void_universe_core.c 
)

pico_set_program_name(void_universe_core "void_universe_core")
pico_set_program_version(void_universe_core "0.1")

# UART/USB wählen
pico_enable_stdio_uart(void_universe_core 0)
pico_enable_stdio_usb(void_universe_core 1)

# Standard-Includes
target_include_directories(void_universe_core PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Debug-Definitionen
add_compile_definitions($<$<CONFIG:Debug>:DEBUG=1>)
target_compile_options(void_universe_core PRIVATE -Wall -Wextra -O2)

# Bibliotheken linken
target_link_libraries(void_universe_core
    pico_stdlib
    hardware_spi
    hardware_gpio
    hardware_i2c
    hardware_timer

    # FreeRTOS Kernel (aus lib/freertos)
    freertos_kernel

    # Webserver und andere Module
    freertos_config
    lwip
    lwipcore
    lwipallapps
    #webserver
    tinyusb_device

    led_lib
    lcd1602_lib
    ssd1306_lib
)

pico_add_extra_outputs(void_universe_core)

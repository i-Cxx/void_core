# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.14) # Für FetchContent empfohlen

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

# Pico SDK einbinden
include(pico_sdk_import.cmake)
pico_sdk_init()

# FreeRTOS automatisch via FetchContent einbinden
include(FetchContent)
FetchContent_Declare(
  freertos_kernel
  GIT_REPOSITORY https://github.com/raspberrypi/FreeRTOS-Kernel.git
  GIT_TAG        main # Besser: Einen stabilen Tag oder Commit wählen!
)
FetchContent_MakeAvailable(freertos_kernel)

# FreeRTOS Port für RP2040: portable/ThirdParty/GCC/RP2040
set(FREERTOS_KERNEL_PATH ${freertos_kernel_SOURCE_DIR})
# set(ENV{FREERTOS_KERNEL_PATH} "${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel-RPi")


add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

# Webserver Subdirectory einbinden (erst hier, weil in src/)
add_subdirectory(webserver)

add_executable(void_universe_core 
    void_universe_core.c 
)

pico_set_program_name(void_universe_core "void_universe_core")
pico_set_program_version(void_universe_core "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(void_universe_core 0)
pico_enable_stdio_usb(void_universe_core 1)

# Standard-Includes
target_include_directories(void_universe_core PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040
    ${CMAKE_SOURCE_DIR}/include
)

# FreeRTOS-Quellcode und Portierungsdateien hinzufügen
target_sources(void_universe_core PRIVATE
    ${FREERTOS_KERNEL_PATH}/event_groups.c
    ${FREERTOS_KERNEL_PATH}/list.c
    ${FREERTOS_KERNEL_PATH}/queue.c
    ${FREERTOS_KERNEL_PATH}/stream_buffer.c
    ${FREERTOS_KERNEL_PATH}/tasks.c
    ${FREERTOS_KERNEL_PATH}/timers.c
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
    ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/port.c
)

# Bibliotheken linken
target_link_libraries(void_universe_core
    pico_stdlib
    hardware_spi
    hardware_gpio
    hardware_i2c
    hardware_timer
    freertos_config
    lwip
    lwipcore
    lwipallapps
    webserver
    tinyusb_device
    led_lib
    lcd1602_lib
    ssd1306_lib
)

add_compile_definitions($<$<CONFIG:Debug>:DEBUG=1>)
target_compile_options(void_universe_core PRIVATE -Wall -Wextra -O2)

pico_add_extra_outputs(void_universe_core)
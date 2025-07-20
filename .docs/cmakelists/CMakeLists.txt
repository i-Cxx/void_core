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



cmake_minimum_required(VERSION 3.13)

project(infinity_rts_core 
        LANGUAGES C CXX ASM
        DESCRIPTION "Pico C/C++ SDK Based FreeRTOS Firmware"
)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# set(ENV{PICO_SDK_PATH} "${CMAKE_SOURCE_DIR}/external/pico-sdk")
# set(ENV{PICO_FREERTOS_PATH} "${CMAKE_SOURCE_DIR}/external/FreeRTOS")
# set(ENV{PICO_LWIP_PATH} "${CMAKE_SOURCE_DIR}/external/lwip")
# set(ENV{PICO_TINYUSB_PATH} "${CMAKE_SOURCE_DIR}/external/tinyusb")




# Pico SDK, Extras  & Kernel importieren
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
include(FreeRTOS_Kernel_import.cmake)


pico_sdk_init()


# FreeRTOS, lwIP, TinyUSB als Subdirs einbinden
# Add FreeRTOS as a library
# add_library(FreeRTOS STATIC
#     ${FREERTOS_SRC_DIRECTORY}/event_groups.c
#     ${FREERTOS_SRC_DIRECTORY}/list.c
#     ${FREERTOS_SRC_DIRECTORY}/queue.c
#     ${FREERTOS_SRC_DIRECTORY}/stream_buffer.c
#     ${FREERTOS_SRC_DIRECTORY}/tasks.c
#     ${FREERTOS_SRC_DIRECTORY}/timers.c
#     ${FREERTOS_SRC_DIRECTORY}/portable/MemMang/heap_3.c
#     ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0/port.c
# )
# Build FreeRTOS
# target_include_directories(FreeRTOS PUBLIC
#     ${FREERTOS_CFG_DIRECTORY}/
#     ${FREERTOS_SRC_DIRECTORY}/include
#     ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
# )



add_subdirectory(external/FreeRTOS)
add_subdirectory(external/lwip)
add_subdirectory(external/tinyusb)

# Eigene Libraries einbinden
add_subdirectory(lib/led)
add_subdirectory(lib/divcheck)
add_subdirectory(lib/lcd1602)
add_subdirectory(lib/ssd1306)


# Hauptanwendung (src) einbinden
add_subdirectory(src)
cmake_minimum_required(VERSION 3.15)

# Erwartet: FREERTOS_PORT gesetzt, z.B. GCC_ARM_CM0
if(NOT DEFINED FREERTOS_PORT)
    set(FREERTOS_PORT GCC_ARM_CM0 CACHE STRING "FreeRTOS port name")
endif()

# freertos_config target bereitstellen (dein Projekt muss das mit include-Pfad zu FreeRTOSConfig.h machen)
if(NOT TARGET freertos_config)
    message(FATAL_ERROR "Bitte erstelle in deinem Projekt ein INTERFACE target 'freertos_config' mit Include-Pfad zur FreeRTOSConfig.h!")
endif()

add_library(freertos_kernel STATIC)

# FreeRTOS Kernelquellen
target_sources(freertos_kernel PRIVATE
    croutine.c
    event_groups.c
    list.c
    queue.c
    stream_buffer.c
    tasks.c
    timers.c
)

# Heap Implementation, wenn angegeben (1..5 oder Pfad)
if (DEFINED FREERTOS_HEAP)
    target_sources(freertos_kernel PRIVATE
        $<IF:$<BOOL:$<FILTER:${FREERTOS_HEAP},EXCLUDE,^[1-5]$>>,${FREERTOS_HEAP},portable/MemMang/heap_${FREERTOS_HEAP}.c>
    )
endif()

# Unterordner einbinden
add_subdirectory(include)

# Port abhängig vom FREERTOS_PORT
if(FREERTOS_PORT STREQUAL "GCC_ARM_CM0")
    add_subdirectory(portable/GCC/ARM_CM0)
else()
    message(FATAL_ERROR "Unbekannter FREERTOS_PORT '${FREERTOS_PORT}' - bitte anpassen")
endif()

# Bibliotheken und Abhängigkeiten
target_link_libraries(freertos_kernel
    PUBLIC
        freertos_config
        freertos_kernel_include
        freertos_kernel_port_headers
    PRIVATE
        freertos_kernel_port
)
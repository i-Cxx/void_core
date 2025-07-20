add_library(webserver STATIC
    webserver.c
    usb_rndis_task.c      
    webserver_task.c       
    tusb_lwip_glue.c
    usb_descriptors.c
    fsdata.c
    ${PROJECT_SOURCE_DIR}/external/tinyusb/lib/networking/dhserver.c
    ${PROJECT_SOURCE_DIR}/external/tinyusb/lib/networking/rndis_reports.c
)

target_include_directories(webserver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/arch
    ${PROJECT_SOURCE_DIR}/external/lwip/src/include
    ${PROJECT_SOURCE_DIR}/external/tinyusb/src
    ${PROJECT_SOURCE_DIR}/external/tinyusb/lib/networking
)

target_link_libraries(webserver
    pico_stdlib
    pico_unique_id
    tinyusb_device
    lwipcore
    lwipallapps
)

target_compile_definitions(webserver PUBLIC
    PICO_ENTER_USB_BOOT_ON_EXIT=1
)
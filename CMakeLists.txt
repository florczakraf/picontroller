cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(picontroller)

pico_sdk_init()

add_executable(con
    con.c
    usb_descriptors.c
)
target_include_directories(con PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(con PUBLIC pico_stdlib pico_unique_id tinyusb_device)
pico_enable_stdio_usb(con 0)
pico_enable_stdio_uart(con 0)
pico_add_extra_outputs(con)

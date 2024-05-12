cmake_minimum_required(VERSION 3.24)

include(pico_sdk_import.cmake)

project(picontroller)

pico_sdk_init()

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_executable(picontroller
    picontroller.c
    usb_descriptors.c
)
set_property(TARGET picontroller PROPERTY C_STANDARD 11)
target_compile_options(picontroller PRIVATE -Wall -Wextra)
target_include_directories(picontroller PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(picontroller PUBLIC pico_stdlib pico_unique_id tinyusb_device)
pico_enable_stdio_usb(picontroller 0)
pico_enable_stdio_uart(picontroller 0)
pico_add_extra_outputs(picontroller)

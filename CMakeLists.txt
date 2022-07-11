cmake_minimum_required(VERSION 3.13)

project(debhello VERSION 1.0.0)

configure_file(debhello_config.h.in debhello_config.h)

add_executable(debhello main.c)

target_include_directories(debhello PUBLIC "${PROJECT_BINARY_DIR}")

install(TARGETS debhello RUNTIME DESTINATION bin)

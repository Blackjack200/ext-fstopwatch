cmake_minimum_required(VERSION 3.25)
project(fstopwatch C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(fstopwatch.c
        fstopwatch_arginfo.h
        fstopwatch.stub.php
)


set(PHP_SOURCE ../php-8.2.7)

include_directories(${PHP_SOURCE}/main)
include_directories(${PHP_SOURCE}/Zend)
include_directories(${PHP_SOURCE}/sapi)
include_directories(${PHP_SOURCE}/pear)
include_directories(${PHP_SOURCE})

add_custom_target(makefile COMMAND make && make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

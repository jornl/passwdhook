cmake_minimum_required(VERSION 3.31)
project(PasswordRater)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(PasswordRater
        src/PasswordRater.cpp
        src/config/Config.cpp
        src/config/Config.h
        src/wordlist/Wordlist.cpp
        src/wordlist/Wordlist.h
)

# TODO: Check if this is necessary.
if(WIN32)
   set(BINARY_DIR "${CMAKE_BINARY_DIR}/$<CONFIG>")
else()
    set(BINARY_DIR "${CMAKE_BINARY_DIR}")
endif()

add_custom_target(PasswordRater-config
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/passwordrater.ini
        ${BINARY_DIR}/passwordrater.ini
)

add_dependencies(PasswordRater PasswordRater-config)

target_include_directories(PasswordRater PRIVATE ${CMAKE_SOURCE_DIR}/include)
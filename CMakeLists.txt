cmake_minimum_required(VERSION 3.31)
project(passwdhook)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(passwdhook
        src/PasswordHook.cpp
        src/config/Config.cpp
        src/config/Config.h
        src/wordlist/Wordlist.cpp
        src/wordlist/Wordlist.h
)

add_custom_target(passwdhook-config
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/config.ini
        ${CMAKE_BINARY_DIR}/config.ini
)

add_dependencies(passwdhook passwdhook-config)
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(ut61b
        VERSION 0.1.0.0
        DESCRIPTION "reads UT61B serial output"
        LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -p ${PROJECT_BINARY_DIR})
#set(CMAKE_CXX_CPPCHECK cppcheck)

include(CTest)
include(FetchContent)

FetchContent_Declare(hidapi FIND_PACKAGE_ARGS)
FetchContent_Declare(Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG "6e79e682b726f524310d55dec8ddac4e9c52fb5f" # v3.4.0
        FIND_PACKAGE_ARGS 3
)
FetchContent_MakeAvailable(hidapi Catch2)


add_library(ut61b_lib OBJECT)

target_sources(ut61b_lib PUBLIC src/fs9922_dmm3.hpp PRIVATE src/fs9922_dmm3.cpp)

target_compile_features(ut61b_lib PUBLIC cxx_std_20)
target_compile_options(ut61b_lib PUBLIC
    -Wall -Wextra -Wconversion -Wsign-conversion -Werror
    -pedantic -pedantic-errors
)


add_executable(ut61b)

target_link_libraries(ut61b PRIVATE ut61b_lib hidapi::libusb)
target_link_options(ut61b PRIVATE LINKER:--gc-sections)

target_sources(ut61b PUBLIC src/ch9325.hpp PRIVATE src/ch9325.cpp src/main.cpp)


add_executable(ut61b_test)

target_link_libraries(ut61b_test PRIVATE Catch2::Catch2WithMain ut61b_lib)

target_sources(ut61b_test PRIVATE src/fs9922_dmm3_test.cpp)

add_test(NAME unit_tests COMMAND $<TARGET_FILE:ut61b_test>)

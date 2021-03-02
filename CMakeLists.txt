cmake_minimum_required(VERSION 3.13)

project(ut61b VERSION 0.0.0.0 DESCRIPTION "" LANGUAGES CXX)


set(CMAKE_EXPORT_COMPILE_COMMANDS On)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -p ${PROJECT_BINARY_DIR})
#set(CMAKE_CXX_CPPCHECK cppcheck)


add_compile_options(-flto -Werror -Wall -Wextra -pedantic -pedantic-errors
                    -Wconversion -Wsign-conversion)


add_library(ut61b_lib OBJECT)

target_sources(ut61b_lib PUBLIC src/fs9922_dmm3.hpp PRIVATE src/fs9922_dmm3.cpp)

target_compile_features(ut61b_lib PUBLIC cxx_std_20)


add_executable(ut61b)

target_link_libraries(ut61b PRIVATE ut61b_lib hidapi-libusb)
target_link_options(ut61b PRIVATE LINKER:--gc-sections)

target_sources(ut61b PUBLIC src/ch9325.hpp PRIVATE src/ch9325.cpp src/main.cpp)


add_executable(ut61b_test)

target_link_libraries(ut61b_test PRIVATE ut61b_lib)

target_sources(ut61b_test PRIVATE src/test.cpp src/fs9922_dmm3_test.cpp)
